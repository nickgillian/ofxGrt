/*
 DTW Example
 
 This example shows you how to:
 - setup a gesture recognition pipeline
 - record your own dataset and save it to a file
 - load the dataset back from a file
 - train a DTW classification algorithm using the training dataset
 - use the trained DTW algorithm to predict the class of real-time data
 
 This example uses the 2-dimensional [x y] coordinates from your mouse as input, but you can easily change this to whatever sensor input
 you have access to.
 
 To compile this example:
 - use the Openframeworks project builder to create a new project
 - when you have created the new project, override the default ofApp.h, ofApp.cpp, and main.cpp files with the files from this example
 - open the project in your favorite IDE (XCode, Visual Studio, Code Blocks, etc.) and add the main GRT source folder to the project. You
   can find the main GRT source folder by looking for the folder called GRT in the directory you downloaded from google code. Most IDE's let
   you just drag and drop the entire GRT code folder into your project.
 - note that some IDE's make you specify the location of the GRT source code folder (for example Visual Studio). To do this, open the project's
   properties or setting pane and add the path to the GRT folder to your project's cpp Include section. In XCode you can just drag and drop the
   GRT folder directly from finder into your project.
 - compile openframeworks
 - compile this project
 
 When you have compiled this project, this is how you use it:
 - run the project
 - when you start the project, you will have no training data and the classifier will not be trained so you need to do three things: 
   (1) record some training data
   (2) train your pipeline
   (3) use the pipeline to predict the class of real-time data
 - Step 1:
   - to record some training data, first make sure the value beside the TrainingClassLabel is set to the class you want to record the data for
   - to change the training class label you can use the '[' and ']' keys, [ to decrease the label and ] to increase the label
   - press the 'r' key to start recording the training data
   - move your mouse to make a gesture (for example, you could draw the letter G)
   - press the 'r' key to stop recording the training data
   - repeat this a few times (e.g. 5-10 times)
   - change the training class label to a new label
   - press the 'r' key to start the recording, perform the next gesture, stop the recording
   - keep repeating these steps until you have recorded all the training data you want
   - when you have finished, press the 's' key to save the training data to a file
   - if you need to load the training data at a later stage, for instance when you next restart the program, press the 'l' key
 - Step 2:
   - after you have recorded your training data, you can now train your pipeline
   - to train your pipeline, press the 't' key
   - if the pipeline trained a classification model successfully then you will see the info message: Pipeline Trained, otherwise you will see the 
     warning message WARNING: Failed to train pipeline. If the training failed, then make sure you have successfully recorded the training data 
 - Step 3:
   - after you have trained the pipeline, you can now use the pipeline to predict the class of real-time data
   - if the pipeline was trained, it will automatically start to predict the class of real-time data
   - move your mouse around the screen and you should see the predicted class label change through the various classes you trained the model to predict
   - note that you might also see the predicted class label of 0. This is the special NULL GESTURE LABEL, which is output by the classifier when the 
     likelihood of a gesture is too low. See this tutorial for more info: http://www.nickgillian.com/wiki/pmwiki.php?n=GRT.AutomaticGestureSpotting
 */


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    
    //Initialize the training and info variables
    infoText = "";
    trainingClassLabel = 1;
    record = false;
    
    //The input to the training data will be the [x y] from the mouse, so we set the number of dimensions to 2
    trainingData.setNumDimensions( 2 );
    
    //Initialize the DTW classifier
    DTW dtw;
    
    //Turn on null rejection, this lets the classifier output the predicted class label of 0 when the likelihood of a gesture is low
    dtw.enableNullRejection( true );
    
    //Set the null rejection coefficient to 3, this controls the thresholds for the automatic null rejection
    //You can increase this value if you find that your real-time gestures are not being recognized
    //If you are getting too many false positives then you should decrease this value
    dtw.setNullRejectionCoeff( 3 );
    
    //Turn on the automatic data triming, this will remove any sections of none movement from the start and end of the training samples
    dtw.enableTrimTrainingData(true, 0.1, 90);
    
    //Offset the timeseries data by the first sample, this makes your gestures (more) invariant to the location the gesture is performed
    dtw.setOffsetTimeseriesUsingFirstSample(true);
    
    //Add the classifier to the pipeline (after we do this, we don't need the DTW classifier anymore)
    pipeline.setClassifier( dtw );
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //Grab the current mouse x and y position
    VectorDouble sample(2);
    sample[0] = mouseX;
    sample[1] = mouseY;
    
    //If we are recording training data, then add the current sample to the training data set
    if( record ){
        timeseries.push_back( sample );
    }
    
    //If the pipeline has been trained, then run the prediction
    if( pipeline.getTrained() ){
        pipeline.predict( sample );
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0, 0, 0);
    
    string text;
    int textX = 20;
    int textY = 20;
    
    //Draw the training info
    ofSetColor(255, 255, 255);
    text = "------------------- TrainingInfo -------------------";
    ofDrawBitmapString(text, textX,textY);
    
    if( record ) ofSetColor(255, 0, 0);
    else ofSetColor(255, 255, 255);
    textY += 15;
    text = record ? "RECORDING" : "Not Recording";
    ofDrawBitmapString(text, textX,textY);
    
    ofSetColor(255, 255, 255);
    textY += 15;
    text = "TrainingClassLabel: " + ofToString(trainingClassLabel);
    ofDrawBitmapString(text, textX,textY);
    
    textY += 15;
    text = "NumTrainingSamples: " + ofToString(trainingData.getNumSamples());
    ofDrawBitmapString(text, textX,textY);
    
    
    //Draw the prediction info
    textY += 30;
    text = "------------------- Prediction Info -------------------";
    ofDrawBitmapString(text, textX,textY);
    
    textY += 15;
    text =  pipeline.getTrained() ? "Model Trained: YES" : "Model Trained: NO";
    ofDrawBitmapString(text, textX,textY);
    
    textY += 15;
    text = "PredictedClassLabel: " + ofToString(pipeline.getPredictedClassLabel());
    ofDrawBitmapString(text, textX,textY);
    
    textY += 15;
    text = "Likelihood: " + ofToString(pipeline.getMaximumLikelihood());
    ofDrawBitmapString(text, textX,textY);
    
    textY += 15;
    text = "SampleRate: " + ofToString(ofGetFrameRate(),2);
    ofDrawBitmapString(text, textX,textY);
    
    
    //Draw the info text
    textY += 30;
    text = "InfoText: " + infoText;
    ofDrawBitmapString(text, textX,textY);
    
    //Draw the timeseries data
    if( record ){
        ofFill();
        for(UINT i=0; i<timeseries.getNumRows(); i++){
            double x = timeseries[i][0];
            double y = timeseries[i][1];
            double r = ofMap(i,0,timeseries.getNumRows(),0,255);
            double g = 0;
            double b = 255-r;
            
            ofSetColor(r,g,b);
            ofDrawEllipse(x,y,5,5);
        }
    }
    
    if( pipeline.getTrained() ){
        
        //Draw the data in the DTW input buffer
        DTW *dtw = pipeline.getClassifier< DTW >();
        
        if( dtw != NULL ){
            vector< VectorDouble > inputData = dtw->getInputDataBuffer();
            for(UINT i=0; i<inputData.size(); i++){
                double x = inputData[i][0];
                double y = inputData[i][1];
                double r = ofMap(i,0,inputData.size(),0,255);
                double g = 0;
                double b = 255-r;
                
                ofSetColor(r,g,b);
                ofDrawEllipse(x,y,5,5);
            }
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    infoText = "";
    
    switch ( key) {
        case 'r':
            record = !record;
            if( !record ){
                trainingData.addSample(trainingClassLabel, timeseries);
                
                //Clear the timeseries for the next recording
                timeseries.clear();
            }
            break;
        case '[':
            if( trainingClassLabel > 1 )
                trainingClassLabel--;
            break;
        case ']':
            trainingClassLabel++;
            break;
        case 't':
            if( pipeline.train( trainingData ) ){
                infoText = "Pipeline Trained";
            }else infoText = "WARNING: Failed to train pipeline";
            break;
        case 's':
            if( trainingData.saveDatasetToFile("TrainingData.txt") ){
                infoText = "Training data saved to file";
            }else infoText = "WARNING: Failed to save training data to file";
            break;
        case 'l':
            if( trainingData.loadDatasetFromFile("TrainingData.txt") ){
                infoText = "Training data saved to file";
            }else infoText = "WARNING: Failed to load training data from file";
            break;
        case 'c':
            trainingData.clear();
            infoText = "Training data cleared";
            break;
        default:
            break;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
/*
 See the README file for more info.
 */


#include "ofApp.h"
#define TEXTURE_RESOLUTION 512

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    largeFont.load("verdana.ttf", 12, true, true);
    largeFont.setLineHeight(14.0f);
    smallFont.load("verdana.ttf", 10, true, true);
    smallFont.setLineHeight(12.0f);
    drawInfo = true;
    
    //Initialize the training and info variables
    infoText = "";
    trainingClassLabel = 1;
    record = false;
    
    //The input to the training data will be the [x y] from the mouse, so we set the number of dimensions to 2
    trainingData.setNumDimensions( 2 );
    
    //Initialize the DTW classifier
    ANBC naiveBayes;
    
    //Turn on null rejection, this lets the classifier output the predicted class label of 0 when the likelihood of a gesture is low
    naiveBayes.enableNullRejection( false );
    
    //Set the null rejection coefficient to 3, this controls the thresholds for the automatic null rejection
    //You can increase this value if you find that your real-time gestures are not being recognized
    //If you are getting too many false positives then you should decrease this value
    naiveBayes.setNullRejectionCoeff( 3 );
    
    //Add the classifier to the pipeline (after we do this, we don't need the naiveBayes instance anymore)
    pipeline.setClassifier( naiveBayes );

    //Setup the colors for the 3 classes
    classColors.resize( 3 );
    classColors[0] = ofColor(255, 0, 0);
    classColors[1] = ofColor(0, 255, 0);
    classColors[2] = ofColor(0, 0, 255);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //Grab the current mouse x and y position
    VectorFloat sample(2);
    sample[0] = mouseX / double(ofGetWidth());
    sample[1] = mouseY / double(ofGetHeight());
    
    //If we are recording training data, then add the current sample to the training data set
    if( record ){
        record = false;
        trainingData.addSample( trainingClassLabel, sample );
    }
    
    //If the pipeline has been trained, then run the prediction
    if( pipeline.getTrained() ){
        pipeline.predict( sample );
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(225, 225, 225);

    //If the model has been trained, then draw the texture
    if( pipeline.getTrained() ){
        ofSetColor(255,255,255);
        ofFill();
        ofEnableAlphaBlending();
        texture.draw( 0, 0, ofGetWidth(), ofGetHeight() );
        ofDisableAlphaBlending();
    }
    
    //Draw the training data
    for(unsigned int i=0; i<trainingData.getNumSamples(); i++){
        float x = trainingData[i][0] * ofGetWidth();
        float y = trainingData[i][1] * ofGetHeight();
        ofSetColor( classColors[ trainingData[i].getClassLabel()-1 % classColors.size() ] );
        ofFill();
        ofDrawEllipse(x,y,5,5);
        ofSetColor(0,0,0);
        ofNoFill();
        ofDrawEllipse(x,y,5+1,5+1);
    }

    //Draw the info text
    if( drawInfo ){
        float textX = 10;
        float textY = 25;
        float textSpacer = smallFont.getLineHeight() * 1.5;

        ofFill();
        ofSetColor(100,100,100);
        ofDrawRectangle( 5, 5, 250, 225 );
        ofSetColor( 255, 255, 255 );

        largeFont.drawString( "GRT Naive Bayes Example", textX, textY ); textY += textSpacer*2;

        smallFont.drawString( "[i]: Toogle Info", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[r]: Record Sample", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[t]: Train Model", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[1,2,3]: Set Class Label", textX, textY ); textY += textSpacer;

        textY += textSpacer;
        smallFont.drawString( "Class Label: " + ofToString( trainingClassLabel ), textX, textY ); textY += textSpacer;
        smallFont.drawString( infoText, textX, textY ); textY += textSpacer;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    infoText = "";
    bool buildTexture = false;
    
    switch ( key) {
        case 'r':
            record = true;
            break;
        case '1':
            trainingClassLabel = 1;
            break;
        case '2':
            trainingClassLabel = 2;
            break;
        case '3':
            trainingClassLabel = 3;
            break;
        case 't':
            if( pipeline.train( trainingData ) ){
                infoText = "Pipeline Trained";
                buildTexture = true;
            }else infoText = "WARNING: Failed to train pipeline";
            break;
        case 's':
            if( trainingData.save("TrainingData.grt") ){
                infoText = "Training data saved to file";
            }else infoText = "WARNING: Failed to save training data to file";
            break;
        case 'l':
            if( trainingData.load("TrainingData.grt") ){
                infoText = "Training data saved to file";
            }else infoText = "WARNING: Failed to load training data from file";
            break;
        case 'c':
            trainingData.clear();
            infoText = "Training data cleared";
            break;
        case 'q':
            {
                ofImage img;
                img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
                img.save( ofToDataPath( "screenshot_" + Util::timeAsString() + ".png") );
            }
        break;
        default:
            break;
    }


    if( buildTexture ){
        const unsigned int rows = TEXTURE_RESOLUTION;
        const unsigned int cols = TEXTURE_RESOLUTION;
        const unsigned int size = rows*cols*4;
        vector<float> pixelData( size );
        ofFloatPixels pixels;

        unsigned int index = 0;
        unsigned int classLabel = 0;
        VectorFloat featureVector(2);
        VectorFloat likelihoods;
        float r,g,b,a;
        float maximumLikelihood;
        for(unsigned int j=0; j<cols; j++){
            for(unsigned int i=0; i<rows; i++){
                featureVector[0] = i/double(rows);
                featureVector[1] = j/double(cols);
                if( pipeline.predict( featureVector ) ){
                    classLabel = pipeline.getPredictedClassLabel();
                    maximumLikelihood = pipeline.getMaximumLikelihood();
                    likelihoods = pipeline.getClassLikelihoods();
                    switch( classLabel ){
                        case 1:
                            r = 1.0;
                            g = 0.0;
                            b = 0.0;
                            a = maximumLikelihood;
                            break;
                        case 2: 
                            r = 0.0;
                            g = 1.0;
                            b = 0.0;
                            a = maximumLikelihood;
                            break;
                        case 3: 
                            r = 0.0;
                            g = 0.0;
                            b = 1.0;
                            a = maximumLikelihood;
                            break;
                        break;
                        default:
                            r = 0;
                            g = 0;
                            b = 0;
                            a = 1;
                        break;
                    }
                    pixelData[ index++ ] = r;
                    pixelData[ index++ ] = g;
                    pixelData[ index++ ] = b;
                    pixelData[ index++ ] = a;
                }
            }
        }
        pixels.setFromExternalPixels(&pixelData[0],rows,cols,OF_PIXELS_RGBA);
        if(!texture.isAllocated()){
            texture.allocate( pixels, false );
            texture.setRGToRGBASwizzles(true);
        }
        texture.loadData( pixels );
        texture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
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
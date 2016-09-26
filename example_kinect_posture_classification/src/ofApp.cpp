/*
 See the README file for more info.
 */

#include "ofApp.h"

const ofColor backgroundPlotColor = ofColor(50,50,50,255);

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    largeFont.load("verdana.ttf", 12, true, true);
    largeFont.setLineHeight(14.0f);
    smallFont.load("verdana.ttf", 10, true, true);
    smallFont.setLineHeight(12.0f);
    hugeFont.load("verdana.ttf", 36, true, true);
    hugeFont.setLineHeight(38.0f);
    
    //Initialize the training and info variables
    infoText = "";
    trainingClassLabel = 1;
    predictedClassLabel = 0;
    trainingModeActive = false;
    recordTrainingData = false;
    predictionModeActive = false;
    drawInfo = true;
    leftHand.resize(3);
    rightHand.resize(3);
    
    //The input to the training data will be the [x y z] from the left and right hand, so we set the number of dimensions to 6
    trainingData.setNumDimensions( 6 );
    
    //set the default classifier
    ANBC naiveBayes;
    naiveBayes.enableNullRejection( true );
    naiveBayes.setNullRejectionCoeff( 5.0 );
    pipeline << naiveBayes;

    leftHandPlot.setup( 500, 3, "left hand" );
    leftHandPlot.setDrawGrid( true );
    leftHandPlot.setDrawInfoText( true );
    leftHandPlot.setFont( smallFont );
    leftHandPlot.setBackgroundColor( backgroundPlotColor );

    rightHandPlot.setup( 500, 3, "right hand" );
    rightHandPlot.setDrawGrid( true );
    rightHandPlot.setDrawInfoText( true );
    rightHandPlot.setFont( smallFont );
    rightHandPlot.setBackgroundColor( backgroundPlotColor );

    //Open the connection with Synapse
    synapseStreamer.openSynapseConnection();
    
    //Set which joints we want to track
    synapseStreamer.trackAllJoints(false);
    synapseStreamer.trackLeftHand(true);
    synapseStreamer.trackRightHand(true);

}

//--------------------------------------------------------------
void ofApp::update(){

    synapseStreamer.update();
    
    if( synapseStreamer.getNewMessage() ){
        leftHand = synapseStreamer.getLeftHandJointBody();
        rightHand = synapseStreamer.getRightHandJointBody();
        
        //Update the graphs
        leftHandPlot.update( leftHand );
        rightHandPlot.update( rightHand );
    
        //Update the training mode if needed
        if( trainingModeActive ){
            
            //Check to see if the countdown timer has elapsed, if so then start the recording
            if( !recordTrainingData ){
                if( trainingTimer.timerReached() ){
                    recordTrainingData = true;
                    trainingTimer.start( RECORDING_TIME );
                }
            }else{
                //We should be recording the training data - check to see if we should stop the recording
                if( trainingTimer.timerReached() ){
                    trainingModeActive = false;
                    recordTrainingData = false;
                }
            }
                        
            if( recordTrainingData ){
                //Add the current sample to the training data
                VectorFloat trainingSample(6);
                trainingSample[0] = leftHand[0];
                trainingSample[1] = leftHand[1];
                trainingSample[2] = leftHand[2];
                trainingSample[3] = rightHand[0];
                trainingSample[4] = rightHand[1];
                trainingSample[5] = rightHand[2];
                
                if( !trainingData.addSample(trainingClassLabel, trainingSample) ){
                    infoText = "WARNING: Failed to add training sample to training data!";
                }
            }
        }
        
        //Update the prediction mode if active
        if( predictionModeActive ){
            VectorFloat inputVector(6);
            inputVector[0] = leftHand[0];
            inputVector[1] = leftHand[1];
            inputVector[2] = leftHand[2];
            inputVector[3] = rightHand[0];
            inputVector[4] = rightHand[1];
            inputVector[5] = rightHand[2];
            if( pipeline.predict( inputVector ) ){
                predictedClassLabel = pipeline.getPredictedClassLabel();
                predictionPlot.update( pipeline.getClassLikelihoods() );
                
            }else{
                infoText = "ERROR: Failed to run prediction!";
            }
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(225, 225, 225);

    int marginX = 5;
    int marginY = 5;
    int graphX = marginX;
    int graphY = marginY;
    int graphW = ofGetWidth() - graphX*2;
    int graphH = 100;

    //Draw the info text
    if( drawInfo ){
        float infoX = marginX;
        float infoW = 250;
        float textX = 10;
        float textY = marginY;
        float textSpacer = smallFont.getLineHeight() * 1.5;

        ofFill();
        ofSetColor(100,100,100);
        ofDrawRectangle( infoX, 5, infoW, 225 );
        ofSetColor( 255, 255, 255 );

        largeFont.drawString( "GRT Classifier Example", textX, textY ); textY += textSpacer*2;

        smallFont.drawString( "[i]: Toogle Info", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[r]: Toggle Recording", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[t]: Train Model", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[1,2,3]: Set Class Label", textX, textY ); textY += textSpacer;

        textY += textSpacer;
        smallFont.drawString( "Class Label: " + ofToString( trainingClassLabel ), textX, textY ); textY += textSpacer;
        smallFont.drawString( "Recording: " + ofToString( recordTrainingData ), textX, textY ); textY += textSpacer;
        smallFont.drawString( "Num Samples: " + ofToString( trainingData.getNumSamples() ), textX, textY ); textY += textSpacer;
        smallFont.drawString( infoText, textX, textY ); textY += textSpacer;

        //Update the graph position
        graphX = infoX + infoW + 15;
        graphW = ofGetWidth() - graphX - 15;
    }

    //Draw the data graph
    leftHandPlot.draw( graphX, graphY, graphW, graphH ); graphY += graphH * 1.1;
    rightHandPlot.draw( graphX, graphY, graphW, graphH ); graphY += graphH * 1.1;

    if( trainingModeActive ){
        char strBuffer[1024];
        if( !recordTrainingData ){
            ofSetColor(255,150,0);
            sprintf(strBuffer, "Training Mode Active - Get Ready! Timer: %0.1f",trainingTimer.getSeconds());
        }else{
            ofSetColor(255,0,0);
            sprintf(strBuffer, "Training Mode Active - Recording! Timer: %0.1f",trainingTimer.getSeconds());
        }
        std::string txt = strBuffer;
        ofRectangle bounds = hugeFont.getStringBoundingBox( txt, 0, 0 );
        hugeFont.drawString(strBuffer, ofGetWidth()/2 - bounds.width*0.5, ofGetHeight() - bounds.height*3 );
    }

    //If the model has been trained, then draw the texture
    if( pipeline.getTrained() ){
        predictionPlot.draw( graphX, graphY, graphW, graphH ); graphY += graphH * 1.1;

        std::string txt = "Predicted Class: " + ofToString( predictedClassLabel );
        ofRectangle bounds = hugeFont.getStringBoundingBox( txt, 0, 0 );
        ofSetColor(0,0,255);
        hugeFont.drawString( txt, ofGetWidth()/2 - bounds.width*0.5, ofGetHeight() - bounds.height*3 );
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    infoText = "";
    bool buildTexture = false;
    
    switch ( key) {
        case 'r':
            predictionModeActive = false;
            trainingModeActive = true;
            recordTrainingData = false;
            trainingTimer.start( PRE_RECORDING_COUNTDOWN_TIME );
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
                std::cout << "getNumClasses: " << pipeline.getNumClasses() << std::endl;
                predictionPlot.setup( 500, pipeline.getNumClasses(), "prediction likelihoods" );
                predictionPlot.setDrawGrid( true );
                predictionPlot.setDrawInfoText( true );
                predictionPlot.setFont( smallFont );
                predictionPlot.setBackgroundColor( backgroundPlotColor );
                predictionModeActive = true;
            }else infoText = "WARNING: Failed to train pipeline";
            break;
        case 's':
            if( trainingData.save( ofToDataPath("TrainingData.grt") ) ){
                infoText = "Training data saved to file";
            }else infoText = "WARNING: Failed to save training data to file";
            break;
        case 'l':
            if( trainingData.load( ofToDataPath("TrainingData.grt") ) ){
                infoText = "Training data saved to file";
            }else infoText = "WARNING: Failed to load training data from file";
            break;
        case 'c':
            trainingData.clear();
            infoText = "Training data cleared";
            break;
        case 'i':
            drawInfo = !drawInfo;
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
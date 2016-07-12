/*
 See the README file for more info.
 */

#include "ofApp.h"

 const ofColor backgroundPlotColor = ofColor(50,50,50,255);

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    mappingParameter1 = 1.0f;
    mappingParameter2 = 0.0f;

    track1.load("moonlight_loop.wav");
    track1.setVolume( mappingParameter1 );
    track1.setLoop( true );

    track2.load("tellier_loop.wav");
    track2.setVolume( mappingParameter2 );
    track2.setLoop( true );

    largeFont.load("verdana.ttf", 12, true, true);
    largeFont.setLineHeight(14.0f);
    smallFont.load("verdana.ttf", 10, true, true);
    smallFont.setLineHeight(12.0f);
    hugeFont.load("verdana.ttf", 36, true, true);
    hugeFont.setLineHeight(38.0f);
    
    //Initialize the training and info variables
    infoText = "";
    trainingModeActive = false;
    recordTrainingData = false;
    predictionModeActive = false;
    drawInfo = true;
    rightHand.resize(3);
    
    //The input to the training data will be the [x y z] from the left and right hand, so we set the number of dimensions to 6
    trainingData.setInputAndTargetDimensions( 3, 2 );
    
    //setup the pipeline
    pipeline << Derivative(Derivative::FIRST_DERIVATIVE,1000/30,3,true);
    pipeline << MovementIndex( 30, 3 );
    pipeline << MultidimensionalRegression( LogisticRegression( true ), true );

    //Open the connection with Synapse
    synapseStreamer.openSynapseConnection();
    
    //Set which joints we want to track
    synapseStreamer.trackAllJoints(false);
    synapseStreamer.trackLeftHand(true);
    synapseStreamer.trackRightHand(true);

    ofSetVerticalSync(true);

    rightHandPlot.setup( 500, 3, "right hand" );
    rightHandPlot.setDrawGrid( true );
    rightHandPlot.setDrawInfoText( true );
    rightHandPlot.setFont( smallFont );
    rightHandPlot.setBackgroundColor( backgroundPlotColor );

    featurePlot.setup( 500, 3, "features" );
    featurePlot.setDrawGrid( true );
    featurePlot.setDrawInfoText( true );
    featurePlot.setFont( smallFont );
    featurePlot.setBackgroundColor( backgroundPlotColor );

    track1.play();
    track2.play();
}

//--------------------------------------------------------------
void ofApp::update(){

    synapseStreamer.update();
    //spacecraft.setRotation(1, 270 + ofGetElapsedTimef() * 60, 0, 0, 1);
    
    if( synapseStreamer.getNewMessage() ){
        rightHand = synapseStreamer.getRightHandJointBody();
        
        //Update the graphs
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
                VectorFloat trainingSample(3);
                trainingSample[0] = rightHand[0];
                trainingSample[1] = rightHand[1];
                trainingSample[2] = rightHand[2];

                VectorFloat targetVector(2);
                targetVector[0] = mappingParameter1;
                targetVector[1] = mappingParameter2;
                
                if( !trainingData.addSample(trainingSample,targetVector) ){
                    infoText = "WARNING: Failed to add training sample to training data!";
                }
            }
        }
        
        //Update the prediction mode if active
        if( predictionModeActive ){
            VectorFloat inputVector(3);
            inputVector[0] = rightHand[0];
            inputVector[1] = rightHand[1];
            inputVector[2] = rightHand[2];
            if( pipeline.predict( inputVector ) ){
                mappingParameter1 = pipeline.getRegressionData()[0];
                mappingParameter2 = pipeline.getRegressionData()[1];

                mappingParameter1 = Util::limit( mappingParameter1, 0.0, 1.0 );
                mappingParameter2 = Util::limit( mappingParameter2, 0.0, 1.0 );
            }else{
                infoText = "ERROR: Failed to run prediction!";
            }
        }
        
    }

    ofSoundUpdate();
    track1.setVolume( mappingParameter1 );
    track2.setVolume( mappingParameter2 );
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

    if( pipeline.getTrained() ){
        VectorFloat features = pipeline.getFeatureExtractionData();
        featurePlot.update( features );
    }

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
        smallFont.drawString( "Mapping Parameter 1: " + ofToString( mappingParameter1 ), textX, textY ); textY += textSpacer;
        smallFont.drawString( "Mapping Parameter 2: " + ofToString( mappingParameter2 ), textX, textY ); textY += textSpacer;
        smallFont.drawString( "Recording: " + ofToString( recordTrainingData ), textX, textY ); textY += textSpacer;
        smallFont.drawString( "Num Samples: " + ofToString( trainingData.getNumSamples() ), textX, textY ); textY += textSpacer;
        smallFont.drawString( infoText, textX, textY ); textY += textSpacer;

        //Update the graph position
        graphX = infoX + infoW + 15;
        graphW = ofGetWidth() - graphX - 15;
    }

    //Draw the data graph
    rightHandPlot.draw( graphX, graphY, graphW, graphH ); graphY += graphH * 1.1;
    featurePlot.draw( graphX, graphY, graphW, graphH ); graphY += graphH * 1.1;

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

        std::string txt = "Predicted Parameters: " + ofToString( mappingParameter1 ) + " " + ofToString( mappingParameter2 );
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
        case 't':
            if( pipeline.train( trainingData ) ){
                infoText = "Pipeline Trained";
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
        case '-':
            mappingParameter1+=0.1f;
            mappingParameter2-=0.1f;
            if( mappingParameter1 > 1.0f ) mappingParameter1 = 1.0f;
            if( mappingParameter2 < 0.0f ) mappingParameter2 = 0.0f;
            std::cout << "mapping parameters: " << mappingParameter1 << mappingParameter2 << std::endl;
            break;
        case '+':
            mappingParameter1-=0.1f;
            mappingParameter2+=0.1f;
            if( mappingParameter2 > 1.0f ) mappingParameter2 = 1.0f;
            if( mappingParameter1 < 0.0f ) mappingParameter1 = 0.0f;
            std::cout << "mapping parameters: " << mappingParameter1 << mappingParameter2 << std::endl;
            break;
        case '1':
            mappingParameter1=1.0f;
            mappingParameter2=0.0f;
            std::cout << "mapping parameters: " << mappingParameter1 << mappingParameter2 << std::endl;
            break;
        case '2':
            mappingParameter1=0.0f;
            mappingParameter2=1.0f;
            std::cout << "mapping parameters: " << mappingParameter1 << mappingParameter2 << std::endl;
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
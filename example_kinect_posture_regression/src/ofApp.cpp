/*
 See the README file for more info.
 */

#include "ofApp.h"

const ofColor backgroundPlotColor = ofColor(50,50,50,255);
GLfloat lightOnePosition[] = {40.0, 40, 100.0, 0.0};
GLfloat lightOneColor[] = {0.99, 0.99, 0.99, 1.0};

GLfloat lightTwoPosition[] = {-40.0, 40, 100.0, 0.0};
GLfloat lightTwoColor[] = {0.99, 0.99, 0.99, 1.0};

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
    trainingModeActive = false;
    recordTrainingData = false;
    predictionModeActive = false;
    drawInfo = true;
    leftHand.resize(3);
    rightHand.resize(3);
    
    //The input to the training data will be the [x y z] from the left and right hand, so we set the number of dimensions to 6
    trainingData.setInputAndTargetDimensions( 6, 1 );
    
    //setup the pipeline
    pipeline << LinearRegression( true );

    //Open the connection with Synapse
    synapseStreamer.openSynapseConnection();
    
    //Set which joints we want to track
    synapseStreamer.trackAllJoints(false);
    synapseStreamer.trackLeftHand(true);
    synapseStreamer.trackRightHand(true);

    ofSetVerticalSync(true);
    
    //some model / light stuff
    ofEnableDepthTest();
    glShadeModel (GL_SMOOTH);

    /* initialize lighting */
    glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor);
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT1, GL_POSITION, lightTwoPosition);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
    glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);
    
    //load the model - the 3ds and the texture file need to be in the same folder
    spacecraft.loadModel("model/Feisar_Ship.3ds", 2);
    spacecraft.setRotation(0, 70, 1, 0, 0);
    spacecraft.setRotation(1, 0, 0, 0, 1);
    spacecraft.setScale(0.9, 0.9, 0.9);
    spacecraft.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);

    rollRotationAngle = 0;
    pitchRotationAngle = 40;
}

//--------------------------------------------------------------
void ofApp::update(){

    synapseStreamer.update();
    //spacecraft.setRotation(1, 270 + ofGetElapsedTimef() * 60, 0, 0, 1);
    
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
                
                if( !trainingData.addSample(trainingSample,VectorFloat(1,rollRotationAngle)) ){
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
                rollRotationAngle = pipeline.getRegressionData()[0];
            }else{
                infoText = "ERROR: Failed to run prediction!";
            }
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(225, 225, 225);

    //Draw the 3D stuff first

    //If we are in prediction mode then just draw the image    
    glPushMatrix();
    
    //draw in middle of the screen
    glTranslatef(ofGetWidth()/2,ofGetHeight()/2,0);
    glRotatef(rollRotationAngle,0,0,1);
    glRotatef(pitchRotationAngle,1,0,0);
    glTranslatef(-ofGetWidth()/2,-ofGetHeight()/2,0);
    
    ofSetColor(255, 255, 255, 255);
    spacecraft.drawFaces();
    
    glPopMatrix();

    //Now draw any 2D stuff
    //ofDisableDepthTest();

    if( trainingModeActive ){
        if( !recordTrainingData ){
            string txt = "PREP";
            ofRectangle bounds = hugeFont.getStringBoundingBox(txt,0,0);
            hugeFont.drawString(txt,ofGetWidth()-25-bounds.width,ofGetHeight()-25-bounds.height);
        }else{
            string txt = "REC";
            ofRectangle bounds = hugeFont.getStringBoundingBox(txt,0,0);
            hugeFont.drawString(txt,ofGetWidth()-25-bounds.width,ofGetHeight()-25-bounds.height);
        }
    }

    //ofEnableDepthTest();
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
                infoText = "Training data file loaded";
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
            rollRotationAngle-=10;
            std::cout << "rollRotationAngle: " << rollRotationAngle << std::endl;
            break;
        case '+':
            rollRotationAngle+=10;
            std::cout << "rollRotationAngle: " << rollRotationAngle << std::endl;
            break;
        case '{':
            pitchRotationAngle-=10;
            std::cout << "pitchRotationAngle: " << pitchRotationAngle << std::endl;
            break;
        case '}':
            pitchRotationAngle+=10;
            std::cout << "pitchRotationAngle: " << pitchRotationAngle << std::endl;
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
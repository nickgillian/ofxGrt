/*
 See the README file for more info.
 */

#include "ofApp.h"
#define TEXTURE_RESOLUTION 1024

const ofColor backgroundPlotColor = ofColor(50,50,50,255);

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    largeFont.load("verdana.ttf", 12, true, true);
    largeFont.setLineHeight(14.0f);
    smallFont.load("verdana.ttf", 10, true, true);
    smallFont.setLineHeight(12.0f);
    
    //Initialize the training and info variables
    infoText = "";
    trainingClassLabel = 0;
    record = false;
    drawInfo = true;
    
    //The input to the training data will be the [x y z] from the accelerometer, so we set the number of dimensions to 3
    trainingData.setNumDimensions( 3 );

    //Build the pipeline, adding first the feature extraction, then the classifier
    pipeline << EnvelopeExtractor( 15, trainingData.getNumDimensions() );
    pipeline << ANBC();

    //Setup the gyro osc
    gyrosc.setup( 5000 );

    accDataPlot.setup( 500, 3, "acc" );
    accDataPlot.setDrawGrid( true );
    accDataPlot.setDrawInfoText( true );
    accDataPlot.setFont( smallFont );
    accDataPlot.setBackgroundColor( backgroundPlotColor );

    featurePlot.setup( 500, 3, "Envelope Extractor Features" );
    featurePlot.setDrawGrid( true );
    featurePlot.setDrawInfoText( true );
    featurePlot.setFont( smallFont );
    featurePlot.setBackgroundColor( backgroundPlotColor );

}

//--------------------------------------------------------------
void ofApp::update(){

    //Update the gyro osc module
    gyrosc.update();

    if( gyrosc.getNewDataReady() ){
        GRT::VectorFloat acc = gyrosc.getAcc();

        //Update the data graph
        accDataPlot.update( acc );

        //If we are recording training data, then add the current sample to the training data set
        if( record ){
            trainingData.addSample( trainingClassLabel, acc );
        }
        
        //If the pipeline has been trained, then run the prediction
        if( pipeline.getTrained() ){
            pipeline.predict( acc );
            predictionPlot.update( pipeline.getClassLikelihoods() );
        }else{
            pipeline.preProcessData( acc );
        }

        //Update the feature plot
        featurePlot.update( pipeline.getFeatureExtractionData() );
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

        largeFont.drawString( "Gyrosc Shake Detection Example", textX, textY ); textY += textSpacer*2;

        smallFont.drawString( "[i]: Toogle Info", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[r]: Toggle Recording", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[t]: Train Model", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[1,2,3]: Toggle Class Label", textX, textY ); textY += textSpacer;

        textY += textSpacer;
        smallFont.drawString( "Class Label: " + ofToString( trainingClassLabel ), textX, textY ); textY += textSpacer;
        smallFont.drawString( "Recording: " + ofToString( record ), textX, textY ); textY += textSpacer;
        smallFont.drawString( "Num Samples: " + ofToString( trainingData.getNumSamples() ), textX, textY ); textY += textSpacer;
        smallFont.drawString( infoText, textX, textY ); textY += textSpacer;

        //Update the graph position
        graphX = infoX + infoW + 15;
        graphW = ofGetWidth() - graphX - 15;
    }

    //Draw the data graph
    accDataPlot.draw( graphX, graphY, graphW, graphH ); graphY += graphH * 1.1;
    featurePlot.draw( graphX, graphY, graphW, graphH ); graphY += graphH * 1.1;

    //If the model has been trained, then draw the texture
    if( pipeline.getTrained() ){
        predictionPlot.draw( graphX, graphY, graphW, graphH ); graphY += graphH * 1.1;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    infoText = "";
    bool buildTexture = false;
    
    switch ( key) {
        case 'r':
            record = !record;
            break;
        case '1':
            if( trainingClassLabel != 1 ) trainingClassLabel = 1;
            else trainingClassLabel = 0;
            break;
        case '2':
            if( trainingClassLabel != 2 ) trainingClassLabel = 2;
            else trainingClassLabel = 0;
            break;
        case '3':
            if( trainingClassLabel != 3 ) trainingClassLabel = 3;
            else trainingClassLabel = 0;
            break;
        case 't':
            if( pipeline.train( trainingData ) ){
                infoText = "Pipeline Trained";
                predictionPlot.setup( 500, pipeline.getNumClasses(), "prediction likelihoods" );
                predictionPlot.setDrawGrid( true );
                predictionPlot.setDrawInfoText( true );
                predictionPlot.setFont( smallFont );
                predictionPlot.setBackgroundColor( backgroundPlotColor );
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
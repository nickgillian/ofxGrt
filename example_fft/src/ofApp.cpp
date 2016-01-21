/*
 
 */

#include "ofApp.h"

#define AUDIO_BUFFER_SIZE 512
#define AUDIO_SAMPLE_RATE 44100
#define FFT_WINDOW_SIZE 2048
#define FFT_HOP_SIZE AUDIO_BUFFER_SIZE

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    
    fft.init(FFT_WINDOW_SIZE,FFT_HOP_SIZE,1,FFT::RECTANGULAR_WINDOW,true,false);

    magnitudePlot.setup( FFT_WINDOW_SIZE/2, 1 );

    ofSoundStreamSetup(2, 1, this, AUDIO_SAMPLE_RATE, AUDIO_BUFFER_SIZE, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //Grab the current mouse x and y position
    VectorDouble sample(2);

    vector< FastFourierTransform > &results =  fft.getFFTResultsPtr();
    magnitudePlot.setData( results[0].getMagnitudeData() );
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0, 0, 0);
    
    float margin = 10;
    magnitudePlot.draw( margin, margin, ofGetWidth() - margin*2, 250 );
    
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){

    GRT::VectorDouble data(1);
    for (int i=0; i<bufferSize; i++) {
        data[0] = input[i];
        fft.update( data );
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
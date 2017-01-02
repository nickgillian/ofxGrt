
#include "ofApp.h"

#define NUM_DIMENSIONS 20

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(255);

    //Load the font for the graph
    font.load("verdana.ttf", 12, true, true);
    font.setLineHeight(14.0f);

    //Setup the plot
    plot1.setup( NUM_DIMENSIONS, "sine data");
    plot1.setRanges( -1.0, 1.0, true );
}


//--------------------------------------------------------------
void ofApp::update(){

    //Update the data
    vector< float > data( NUM_DIMENSIONS );
    for(unsigned int i=0; i<NUM_DIMENSIONS; i++){
        data[i] = sin( ofMap(i,0,NUM_DIMENSIONS-1,0.0,TWO_PI) );
    }
    plot1.update( data );

}

//--------------------------------------------------------------
void ofApp::draw(){

    int plotX = 10;
    int plotY = 10;
    int plotW = ofGetWidth() - plotX*2;
    int plotH = ofGetHeight() - 20;

    ofSetColor(255,255,255);
    ofFill();
	plot1.draw( plotX, plotY, plotW, plotH );
    ofSetColor(255,0,0);
    ofNoFill();
    ofDrawRectangle( plotX, plotY, plotW, plotH );
    plotX += 25 + plotW;

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch(key){
        case 'q':
            {
                ofImage img;
                img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
                img.save( ofToDataPath( "screenshot_" + Util::timeAsString() + ".png") );
            }
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

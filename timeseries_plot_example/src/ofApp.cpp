
#include "ofApp.h"

#define TIMESERIES_LENGTH 200

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(255);
    plot.setup( TIMESERIES_LENGTH, 2 );
    plot.setDrawGrid( true );
}

//--------------------------------------------------------------
void ofApp::update(){

    //Get the mouse data
    VectorDouble data(2);
    data[0] = ofGetMouseX();
    data[1] = ofGetMouseY();

    //Update the plot
    plot.update( data );
}

//--------------------------------------------------------------
void ofApp::draw(){

    int plotX = 20;
    int plotY = 20;
    int plotW = TIMESERIES_LENGTH;
    int plotH = 75;
	plot.draw( plotX, plotY, plotW, plotH );

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch( key ){
        case 'r':
            //Reset the data contents and ranges of the plot
            plot.reset();
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


#include "ofApp.h"

#define TIMESERIES_LENGTH 200
#define NUM_DIMENSIONS 2
float lastX = 0;
float lastY = 0;
float lastTic = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(255);

    //Load the font for the graph, we do this via a shared pointer as there seems to be an issue
    //with coping fonts
    std::shared_ptr< ofTrueTypeFont > font = std::make_shared< ofTrueTypeFont >();
    font->load("verdana.ttf", 12, true, true);
    font->setLineHeight(14.0f);

    //Setup the first plot
    plot1.setup( TIMESERIES_LENGTH, NUM_DIMENSIONS, "Mouse Data" );
    plot1.setDrawGrid( true );
    plot1.setDrawInfoText( true );
    plot1.setFont( font );

    //Setup the second plot
    plot2.setup( TIMESERIES_LENGTH, NUM_DIMENSIONS, "Mouse Derivative" );
    plot2.setDrawGrid( true );
    plot2.setDrawInfoText( true );
    plot2.setFont( font );
}


//--------------------------------------------------------------
void ofApp::update(){

    //Get the mouse data
    float mouseX = ofGetMouseX();
    float mouseY = ofGetMouseY();
    float tic = ofGetElapsedTimeMillis();
    float delta = tic-lastTic;

    VectorDouble data(2);

    //Update the 1st plot
    data[0] = mouseX;
    data[1] = mouseY;
    plot1.update( data );

    //Update the 2nd plot
    data[0] = lastX > 0 ? (mouseX-lastX) / delta : 0.0;
    data[1] = lastY > 0 ? (mouseY-lastY) / delta : 0.0;
    plot2.update( data );

    //Store the mouse values for the next update
    lastX = mouseX;
    lastY = mouseY;
    lastTic = tic;
}

//--------------------------------------------------------------
void ofApp::draw(){

    int plotX = 20;
    int plotY = 20;
    int plotW = ofGetWidth() - (plotX*2);
    int plotH = 200;

	plot1.draw( plotX, plotY, plotW, plotH );

    plotY += 25 + plotH;
    plot2.draw( plotX, plotY, plotW, plotH );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch( key ){
        case 'r':
            //Reset the data contents and ranges of the plot
            plot1.reset();
            plot2.reset();
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

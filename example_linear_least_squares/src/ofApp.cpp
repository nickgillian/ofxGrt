/*
 See the README file for more info.
 */

#include "ofApp.h"
#define NUM_POINTS 50

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    largeFont.load("verdana.ttf", 12, true, true);
    largeFont.setLineHeight(14.0f);
    smallFont.load("verdana.ttf", 10, true, true);
    smallFont.setLineHeight(12.0f);
    
    //Initialize the training and info variables
    infoText = "";
    drawInfo = true;

    Random random;
    float norm = 1.0/NUM_POINTS;
    float xNoise = 0.01;
    float yNoise = 0.1;
    for(UINT i=0; i<NUM_POINTS; i++){
        x.push_back( random.getGauss(i*norm,xNoise) );
        y.push_back( random.getGauss((NUM_POINTS-i)*norm,yNoise) );
    }

    lls.solve( x, y );

    x.save( ofToDataPath("x.csv") );
    y.save(ofToDataPath("y.csv") );
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(225, 225, 225);

    float textBoxX = 5;
    float textBoxY = 5;
    float textBoxW = 400;
    float textBoxH = 150;
    float plotX = 20;
    float plotY = textBoxY + textBoxH + 20;
    float plotW = 1200;
    float plotH = 500;
    float x1 = 0;
    float y1 = 0;
    float x2 = 0;
    float y2 = 0;

     //Get the LLS results
    const float m = lls.getM();
    const float b = lls.getB();
    const float r = lls.getR();

    ofPushMatrix();

    if( drawInfo ){
        float textX = textBoxX + 10;
        float textY = textBoxY + 25;
        float textSpacer = smallFont.getLineHeight() * 1.5;

        ofFill();
        ofSetColor(100,100,100);
        ofDrawRectangle( textBoxX, textBoxY, textBoxW, textBoxH );
        ofSetColor( 255, 255, 255 );

        largeFont.drawString( "GRT Linear Least Squares Example", textX, textY ); textY += textSpacer*2;

        smallFont.drawString( "[i]: Toogle Info", textX, textY ); textY += textSpacer;

        textY += textSpacer;
        smallFont.drawString( "Correlation Coeff: " + ofToString( r ), textX, textY ); textY += textSpacer;
        smallFont.drawString( infoText, textX, textY ); textY += textSpacer;
    }

    ofTranslate(plotX,plotY);

    //Draw the plot axis
    ofSetColor(0,0,0);
    ofFill();

    ofDrawLine(0,plotH,plotW,plotH); //X Axis
    ofDrawLine(0,0,0,plotH); //Y Axis

    ofSetColor(0,0,255);
    for(UINT i=0; i<NUM_POINTS; i++){
        x1 = x[i] * plotW;
        y1 = y[i] * plotH;
        ofDrawEllipse(x1,y1,10,10);
    }

    //Draw the estimated linear line of best fit
    x1 = 0.0;
    y1 = m*x1 + b;
    x2 = 1.0;
    y2 = m*x2 + b;
    ofSetColor(255,0,0);
    ofDrawLine(x1*plotW,y1*plotH,x2*plotW,y2*plotH);

    ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    infoText = "";
    ofImage img;
    
    switch ( key) {
        case 'i':
            drawInfo = !drawInfo;
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
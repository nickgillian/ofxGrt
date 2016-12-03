/*
  This example demonstrates how to use the GRT regression algorithms in openFrameworks. See the README file for more info.
 */

#pragma once

#include "ofMain.h"
#include "ofxGrt.h"

//State that we want to use the GRT namespace
using namespace GRT;

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //Create some variables for the demo
    RegressionData trainingData;      		//This will store our training data
    GestureRecognitionPipeline pipeline;        //This is a wrapper for our regression and any pre/post processing modules 
    bool record;                                //This is a flag that keeps track of when we should record training data
    bool drawInfo;
    GRT::VectorFloat targetVector;              //This will hold the current label for when we are training the classifier
    string infoText;                            //This string will be used to draw some info messages to the main app window
    ofTexture texture;
    ofTrueTypeFont largeFont;
    ofTrueTypeFont smallFont;

};

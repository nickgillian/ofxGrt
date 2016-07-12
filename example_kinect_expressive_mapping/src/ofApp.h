/*
  This example demonstrates how to use the GRT ANBC algorithm in openFrameworks. See the README file for more info.
 */

#pragma once

#include "ofMain.h"
#include "ofxGrt.h"
#include "SynapseStreamer.h"
#include <stdio.h>
#include "ofxAssimpModelLoader.h"

//State that we want to use the GRT namespace
using namespace GRT;

#define PRE_RECORDING_COUNTDOWN_TIME 4000
#define RECORDING_TIME 5000

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
    GestureRecognitionPipeline pipeline;        //This is a wrapper for our classifier and any pre/post processing modules 
    bool recordTrainingData;                                //This is a flag that keeps track of when we should record training data
    bool trainingModeActive;
    bool predictionModeActive;
    bool drawInfo;
    VectorFloat rightHand;
    string infoText;                            //This string will be used to draw some info messages to the main app window
    ofTrueTypeFont largeFont;
    ofTrueTypeFont smallFont;
    ofTrueTypeFont hugeFont;
    SynapseStreamer synapseStreamer;
    ofxGrtTimeseriesPlot rightHandPlot;
    ofxGrtTimeseriesPlot featurePlot;
    ofxGrtTimeseriesPlot predictionPlot;
    Timer trainingTimer;
    float mappingParameter1;
    float mappingParameter2;

    ofSoundPlayer track1;
    ofSoundPlayer track2;

};

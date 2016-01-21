/*
  This example demonstrates how to use the GRT FFT algorithm in openFrameworks
 */

#pragma once

#include "ofMain.h"
#include "ofxGRT.h"

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

    void audioIn(float * input, int bufferSize, int nChannels);
    
    //Create some variables for the demo
    FFT fft;
    ofxGrtTimeseriesPlot magnitudePlot;
};
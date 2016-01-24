/*
  This example demonstrates how to use the GRT Dynamic Time Warping algorithm in openFrameworks
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

    void drawTimeseries();
    void drawTrainingData();
    void drawDistanceMatrix();

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
    TimeSeriesClassificationData trainingData;      		//This will store our training data
    MatrixFloat timeseries;                                 //This will store a single training sample
    GestureRecognitionPipeline pipeline;                    //This is a wrapper for our classifier and any pre/post processing modules 
    bool record;                                            //This is a flag that keeps track of when we should record training data
    UINT trainingClassLabel;                                //This will hold the current label for when we are training the classifier
    string infoText;                                        //This string will be used to draw some info messages to the main app window
    ofTrueTypeFont font;
    ofShader shader;
    ofxGrtTimeseriesPlot predictedClassPlot;
    ofxGrtTimeseriesPlot classLikelihoodsPlot;
    Vector< ofxGrtTimeseriesPlot > trainingDataPlot;
    Vector< ofxGrtMatrixPlot > distanceMatrixPlots;
};
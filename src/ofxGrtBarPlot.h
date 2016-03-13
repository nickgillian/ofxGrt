#pragma once
#include "ofMain.h"
#include "GRT/GRT.h"

using namespace GRT;

#define BIG_POSITIVE_VALUE 99e+99
#define BIG_NEGATIVE_VALUE -99e+99

class ofxBarPlot{

public:
    ofxBarPlot();
    ~ofxBarPlot();
    
    bool init(UINT numDimensions);
    bool init(UINT numDimensions,vector< double > minRanges,vector< double > maxRanges,bool lockRanges = false,bool constrainValuesToGraph = true);
    bool resetAxisRanges();
    bool lockRanges(bool rangesLocked);
    bool setAxisRanges(vector<double> minRanges,vector<double> maxRanges,bool lockRanges = false);
    bool update(vector<double> data);
    bool draw(UINT x,UINT y,UINT w,UINT h);
    
    UINT numDimensions;
    vector< double > minRanges;
    vector< double > maxRanges;
    vector< double > data;
    bool initialized;
    bool rangesComputed;
    bool rangesLocked;
    bool constrainValuesToGraph;
    bool drawInfoText;
    bool drawGrid;
    vector< string > channelNames;
    ofColor backgroundColor;
    ofColor gridColor;
    ofColor barColor;
    
    WarningLog warningLog;
    
};
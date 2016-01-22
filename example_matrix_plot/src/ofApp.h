/**
* @file
* @brief ofxGrtTimeseriesPlot Example: this example demonstrates how to use the TimeseriesPlot class to plot mouse data over time
*/

#pragma once

#include "ofMain.h"
#include "ofxGrt.h"

using namespace GRT;

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	float gauss(float x,float mu,float sigma){
        return exp( -SQR(x-mu)/(2.0*SQR(sigma)) );
    }

	ofTrueTypeFont font;
	ofxGrtMatrixPlot plot1;
	ofxGrtMatrixPlot plot2;
	ofxGrtMatrixPlot plot3;
};

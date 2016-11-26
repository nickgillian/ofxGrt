
#include "ofApp.h"

#define TIMESERIES_LENGTH 200
#define NUM_DIMENSIONS 2
float lastX = 0;
float lastY = 0;
float lastTic = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(255);

    //Load the font for the graph
    font.load("verdana.ttf", 12, true, true);
    font.setLineHeight(14.0f);
    heatmap.load("heatmap");

    //Set the font and text for each plot
    plot1.setFont( font, ofColor( 255, 255, 255 ) );
    plot1.setTitle( "plot 1" );
    plot2.setFont( font, ofColor( 255, 255, 255 ) );
    plot2.setTitle( "plot 2" );
    plot3.setFont( font, ofColor( 255, 255, 255 ) );
    plot3.setTitle( "plot 3" );

    //Setup the first matrix, this will plot a sigmoid function
    {
        const unsigned int numRows = 250;
        const unsigned int numCols = 100;
        const float cx = numRows * 0.2f;
        const float cy = numCols * 0.5f;
        const float sigma = 10.0f;
        Matrix<float> mat( numRows, numCols );
        for(unsigned int i=0; i<numRows; i++){
            for(unsigned int j=0; j<numCols; j++){
                mat[i][j] = gauss( i, cx, sigma ) * gauss( j, cy, sigma );
            }
        }
        plot1.update( mat );
    }

    //Setup the second matrix, this will plot a ramp across the rows
    {
        const unsigned int numRows = 250;
        const unsigned int numCols = 100;
        Matrix<float> mat( numRows, numCols );
        for(unsigned int i=0; i<numRows; i++){
            for(unsigned int j=0; j<numCols; j++){
                mat[i][j] = i / float(numRows);
            }
        }
        plot2.update( mat );
    }

    //Setup the third matrix, this will plot a ramp across the cols
    {
        const unsigned int numRows = 250;
        const unsigned int numCols = 100;
        Matrix<float> mat( numRows, numCols );
        for(unsigned int i=0; i<numRows; i++){
            for(unsigned int j=0; j<numCols; j++){
                mat[i][j] = j / float(numCols);
            }
        }
        plot3.update( mat );
    }
}


//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

    int zoom = 2;
    int plotX = 10;
    int plotY = 10;
    int plotW = 0;
    int plotH = 0;
    int spacer = 10;

    vector< ofxGrtMatrixPlot* > plots;
    plots.push_back( &plot1 );
    plots.push_back( &plot2 );
    plots.push_back( &plot3 );

    //Draw the matrices
    for(auto mat : plots){
        ofSetColor(255,255,255);
        ofFill();
        plotW = mat->getWidth() * zoom;
        plotH = mat->getHeight() * zoom;
        mat->draw( plotX, plotY, plotW, plotH );
        plotX += spacer + plotW;
    }

    //Draw the same matrices but with the shader
    for(auto mat : plots){
        ofSetColor(255,255,255);
        ofFill();
        plotW = mat->getWidth() * zoom;
        plotH = mat->getHeight() * zoom;
        mat->draw( plotX, plotY, plotW, plotH, heatmap );
        plotX += spacer + plotW;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

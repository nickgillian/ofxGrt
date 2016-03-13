#include "ofxGrtBarPlot.h"
    
ofxGrtBarPlot::ofxGrtBarPlot(){
    numDimensions = 0;
    drawGrid = false;
    initialized = false;
    rangesComputed = false;
    lockRanges = false;
    drawInfoText = true;
    constrainValuesToGraph = true;
    backgroundColor[0] = 0;
	backgroundColor[1] = 0;
	backgroundColor[2] = 0;
	gridColor[0] = 100;
	gridColor[1] = 100;
	gridColor[2] = 100;
	barColor[0] = 255;
	barColor[1] = 0;
	barColor[2] = 0;
    
    warningLog.setProceedingText("WARNING ofxGrtBarPlot");
}

ofxGrtBarPlot::~ofxGrtBarPlot(){
}

bool ofxGrtBarPlot::setup(unsigned int numDimensions,const std::string title){

    std::unique_lock<std::mutex> lock( mtx );

    initialized = true;
    this->numDimensions = numDimensions;
    this->title = title;
    lockRanges = false;
    rangesComputed = false;
    constrainValuesToGraph = true;
    data.resize(numDimensions,0);
    minRanges.resize(numDimensions,0);
    maxRanges.resize(numDimensions,0);
    return true;
}

bool ofxGrtBarPlot::resetAxisRanges(){

    std::unique_lock<std::mutex> lock( mtx );

    if( !lockRanges ){
        rangesComputed = false;
        return true;
    }
    return false;
}

bool ofxGrtBarPlot::setRanges( const float minY, const float maxY, const bool lockRanges, const bool linkRanges, const bool dynamicScale ){

    std::unique_lock<std::mutex> lock( mtx );

    for(unsigned int i=0; i<numDimensions; i++){
        minRanges[i] = minY;
        maxRanges[i] = maxY;
    }
    this->lockRanges = lockRanges;
    return true;
}

bool ofxGrtBarPlot::setRanges( const vector< GRT::MinMax > &ranges, const bool lockRanges, const bool linkRanges, const bool dynamicScale ){

    std::unique_lock<std::mutex> lock( mtx );

    for(unsigned int i=0; i<numDimensions; i++){
        minRanges[i] = ranges[i].minValue;
        maxRanges[i] = ranges[i].maxValue;
    }
    this->lockRanges = lockRanges;
    this->linkRanges = linkRanges;
    this->dynamicScale = dynamicScale;
    return true;
}

bool ofxGrtBarPlot::update( const vector< double > &data ){

    std::unique_lock<std::mutex> lock( mtx );
    
    if( !initialized ){
        warningLog << "update( const vector< double > &data ) the plot has not been initialized!" << endl;
        return false;
    }

    for(unsigned int n=0; n<numDimensions; n++){

        this->data[n] = data[n];

        if( !lockRanges ){
            if( this->data[n] < minRanges[n] ){ minRanges[n] = this->data[n]; }
            else if( this->data[n] > maxRanges[n] && this->data[n] != maxRanges[n] ){ maxRanges[n] = this->data[n]; }
        }
        
    }
    
    //Flag that the ranges have been computed
    rangesComputed = true;
       
    return true;   
}

bool ofxGrtBarPlot::update( const vector< float > &data ){

    std::unique_lock<std::mutex> lock( mtx );
    
    if( !initialized ){
        warningLog << "update( const vector< float > &data ) the plot has not been initialized!" << endl;
        return false;
    }

    for(unsigned int n=0; n<numDimensions; n++){

        this->data[n] = data[n];

        if( !lockRanges ){
            if( this->data[n] < minRanges[n] ){ minRanges[n] = this->data[n]; }
            else if( this->data[n] > maxRanges[n] && this->data[n] != maxRanges[n] ){ maxRanges[n] = this->data[n]; }
        }
        
    }
    
    //Flag that the ranges have been computed
    rangesComputed = true;
       
    return true;   
}


bool ofxGrtBarPlot::draw(unsigned int x,unsigned int y,unsigned int w,unsigned int h){

    std::unique_lock<std::mutex> lock( mtx );
    
    if( !initialized ) return false;
    
    ofPushMatrix();
    ofEnableAlphaBlending();
    ofTranslate(x, y);
    
    //Draw the background
    ofFill();
    ofSetColor(backgroundColor[0],backgroundColor[1],backgroundColor[2]);
    ofDrawRectangle(0,0,w,h);
    
    //Draw the grid if required
    if( drawGrid ){
        ofSetColor(gridColor[0],gridColor[1],gridColor[2]);
        unsigned int numVLines = 20;
        unsigned int numHLines = 10;
        
        //Draw the horizontal lines
        for(unsigned int i=0; i<numHLines; i++){
            float xStart = 0;
            float xEnd = w;
            float yStart = ofMap(i,0,numHLines,0,h);
            float yEnd = yStart;
            ofLine(xStart,yStart,xEnd,yEnd);
        }
        
        //Draw the vertical lines
        for(unsigned int i=0; i<numVLines; i++){
            float xStart = ofMap(i,0,numVLines,0,w);
            float xEnd = xStart+1;
            float yStart = 0;
            float yEnd = h;
            ofDrawLine(xStart,yStart,xEnd,yEnd);
        }
    }
    
    float barWidth = floor(w/(numDimensions+1.0));
    float barSpacer = (w-(barWidth*numDimensions))/numDimensions;
    
    //Draw the axis lines
    ofSetColor(255,255,255);
    ofDrawLine(-5,h,w+5,h); //X Axis
    ofDrawLine(0,-5,0,h+5); //Y Axis
    
    //Draw the bars
    ofSetColor(barColor[0],barColor[1],barColor[2]);
    ofFill();
    float x1 = barSpacer/2.0;
    float x2 = 0;
    float y1 = 0;
    float y2 = 0;
    float barHeight = 0;
    for(unsigned int n=0; n<numDimensions; n++){
        if( minRanges[n] != maxRanges[n] ){
            barHeight = ofMap(data[n],minRanges[n],maxRanges[n],1,h-1,constrainValuesToGraph);
            x2 = barWidth;
            y1 = 0 + h-barHeight-1;
            y2 = barHeight; 
            ofDrawRectangle(x1,y1,x2,y2);
        }
        
        x1 += barWidth + barSpacer;
    }
    
    ofDisableAlphaBlending();
    ofPopMatrix();
    
    return true;
}

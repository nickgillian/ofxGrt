#include "ofxGrtTimeseriesPlot.h"

GRT_BEGIN_NAMESPACE
    
ofxGrtTimeseriesPlot::ofxGrtTimeseriesPlot(){
    plotTitle = "";
    font = NULL;
    initialized = false;
    lockRanges = false;
    minY = 0;
    maxY = 0;
    constrainValuesToGraph = true;
    drawInfoText = false;
    drawGrid = false;    
    textColor[0] = 225;
    textColor[1] = 225;
    textColor[2] = 225;
    backgroundColor[0] = 0;
    backgroundColor[1] = 0;
    backgroundColor[2] = 0;
    gridColor[0] = 255;
    gridColor[1] = 255;
    gridColor[2] = 255;
    gridColor[3] = 100;
    
    errorLog.setProceedingText("[ERROR ofxGrtTimeseriesPlot]");
}

ofxGrtTimeseriesPlot::~ofxGrtTimeseriesPlot(){
}

bool ofxGrtTimeseriesPlot::setup(unsigned int timeseriesLength,unsigned int numDimensions,std::string title){
    
    initialized = false;
    
    //Cleanup the old memory
    dataBuffer.clear();
    
    if( timeseriesLength == 0 || numDimensions == 0 ) return false;
    
    //Setup the memory for the new buffer
    this->timeseriesLength = timeseriesLength;
    this->numDimensions = numDimensions;
    this->plotTitle = title;
    dataBuffer.resize(timeseriesLength, VectorFloat(numDimensions,0));
    
    //Fill the buffer with empty values
    for(unsigned int i=0; i<timeseriesLength; i++)
        dataBuffer.push_back(VectorFloat(numDimensions,0));
    
    lockRanges = false;
    minY = 0;
    maxY = 0;
    channelNames.resize(numDimensions,"");
    
    colors.resize(numDimensions);
    //Setup the default colors
    if( numDimensions >= 1 ) colors[0] = ofColor(255,0,0); //red
    if( numDimensions >= 2 ) colors[1] = ofColor(0,255,0); //green
    if( numDimensions >= 3 ) colors[2] = ofColor(0,0,255); //blue
    //Randomize the remaining colors
    for(unsigned int n=3; n<numDimensions; n++){
        colors[n][0] = ofRandom(50,255);
        colors[n][1] = ofRandom(50,255);
        colors[n][2] = ofRandom(50,255);
    }

    channelVisible.resize(numDimensions,true);
    initialized = true;
    
    return true;    
}

bool ofxGrtTimeseriesPlot::reset(){
        
    if( !initialized ) return false;
    
    if( !lockRanges ){
        minY = 0;
        maxY = 0;
    }

    //Clear the buffer
    dataBuffer.setAllValues(VectorFloat(numDimensions,0));
    
    return true;
}
    
bool ofxGrtTimeseriesPlot::setRanges(float minY,float maxY,bool lockRanges){
    if( minY == maxY ){
        return false;
    }
    this->minY = minY;
    this->maxY = maxY;
    this->lockRanges = lockRanges;
    return true;
}
    
bool ofxGrtTimeseriesPlot::setData( const Vector< VectorFloat > &data ){
    
    const unsigned int M = (unsigned int)data.size();
    dataBuffer.reset();
    
    for(unsigned int i=0; i<M; i++){
        if( data[i].size() != numDimensions ){
            return false;
        }
        update( data[i] );
    }
    
    return true;
}
    
bool ofxGrtTimeseriesPlot::setData( const MatrixFloat &data ){
    
    const unsigned int M = data.getNumRows();
    const unsigned int N = data.getNumCols();
    
    if( N != numDimensions ){
        errorLog << "setData( const MatrixFloat &data ) - The number of dimensions in the data does not match the number of dimensions in the graph!" << endl;
        return false;
    }
    
    dataBuffer.reset();
    
    for(unsigned int i=0; i<M; i++){
        update( data.getRow(i) );
    }
    
    return true;
}

bool ofxGrtTimeseriesPlot::update(){
    
    //If the buffer has not been initialised then return false, otherwise update the buffer
    if( !initialized ) return false;
    
    //Repeat the previos value
    dataBuffer.push_back( dataBuffer[timeseriesLength-1] );
    
    return true;
}

bool ofxGrtTimeseriesPlot::update( const VectorFloat &data ){

    const unsigned int N = data.getSize();
    
    //If the buffer has not been initialised then return false, otherwise update the buffer
    if( !initialized || N != numDimensions ) return false;
    
    //Add the new value to the buffer
    dataBuffer.push_back( data );
    
    //Check the min and max values
    if( !lockRanges ){
        for(unsigned int n=0; n<numDimensions; n++){
            if( data[n] < minY ){ minY = data[n]; }
            else if( data[n] > maxY ){ maxY = data[n]; }
        }
    }
    
    return true;
    
}
    
bool ofxGrtTimeseriesPlot::draw(unsigned int x,unsigned int y,unsigned int w,unsigned int h){
    
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
        ofSetColor(gridColor[0],gridColor[1],gridColor[2],gridColor[3]);
        unsigned int numVLines = 20;
        unsigned int numHLines = 10;
        
        //Draw the horizontal lines
        for(unsigned int i=0; i<numHLines; i++){
            float xStart = 0;
            float xEnd = w;
            float yStart = ofMap(i,0,numHLines,0,h);
            float yEnd = yStart;
            ofDrawLine(xStart,yStart,xEnd,yEnd);
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
    
    //Draw the axis lines
    ofSetColor(255,255,255);
    ofDrawLine(-5,h,w+5,h); //X Axis
    ofDrawLine(0,-5,0,h+5); //Y Axis
   
    //Draw the timeseries
    if( minY != maxY ){
        float xPos = 0;
        float xStep = w / (float)timeseriesLength;
        unsigned int index = 0;
        ofNoFill();
        for(unsigned int n=0; n<numDimensions; n++){
            xPos = 0;
            index = 0;
            if( channelVisible[n] ){
                ofSetColor( colors[n][0],colors[n][1],colors[n][2] );
                ofBeginShape();
                for(unsigned int i=0; i<timeseriesLength; i++){
                    ofVertex( xPos, ofMap(dataBuffer[i][n], minY, maxY, h, 0, constrainValuesToGraph) );
                    xPos += xStep;
                }
                ofEndShape(false);
            }
        }
    }

    //Disabled alpha blending before we draw any text
    ofDisableAlphaBlending();

    //Only draw the text if the font has been loaded
    if( font ){

        if( !font->isLoaded() ) return false;
        
        ofRectangle bounds = font->getStringBoundingBox(plotTitle, 0, 0);
        int textX = 10;
        int textY = bounds.height + 5;
        int textSpacer = bounds.height + 5;

        if( plotTitle != "" ){
            ofSetColor(textColor[0],textColor[1],textColor[2]);
            font->drawString( plotTitle, textX, textY );
            textY += textSpacer;
        }
        
        if( drawInfoText ){
            std::stringstream info;
            for(unsigned int n=0; n<numDimensions; n++){
                if( channelVisible[n] ){
                    ofSetColor(colors[n][0],colors[n][1],colors[n][2]);
                    info.str("");
                    info << "[" << n+1 << "]: " << channelNames[n] << " ";
                    info << dataBuffer[timeseriesLength-1][n] << " [" << minY << " " << maxY << "]" << endl;
                    bounds = font->getStringBoundingBox(info.str(), 0, 0);
                    font->drawString(info.str(),textX,textY);
                    textY += bounds.height + 5;
                }
            }
        }
    }

    ofPopMatrix();
    
    return true;
}

GRT_END_NAMESPACE


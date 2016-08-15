#include "ofxGrtTimeseriesPlot.h"

using namespace GRT;
    
ofxGrtTimeseriesPlot::ofxGrtTimeseriesPlot(){
    plotTitle = "";
    font = NULL;
    initialized = false;
    lockRanges = false;
    linkRanges = false;
    dynamicScale = false;
    drawOrderInverted = false;
    globalMin =  std::numeric_limits<float>::max();
    globalMax =  -std::numeric_limits<float>::max();
    constrainValuesToGraph = true;
    drawInfoText = true;
    drawPlotTitle = true;
    drawPlotValues = true;
    drawGrid = true;    
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

bool ofxGrtTimeseriesPlot::setup( const unsigned int timeseriesLength, const unsigned int numChannels, const std::string title ){

    std::unique_lock<std::mutex> lock( mtx );
    
    initialized = false;
    
    //Cleanup the old memory
    dataBuffer.clear();
    highlightBuffer.clear();
    labelBuffer.clear();

    if( timeseriesLength == 0 || numChannels == 0 ) return false;

    //Setup the memory for the new buffer
    this->timeseriesLength = timeseriesLength;
    this->numChannels = numChannels;
    this->plotTitle = title;

    dataBuffer.resize(timeseriesLength, vector<float>(numChannels,0));
    highlightBuffer.resize(timeseriesLength, 0);
    labelBuffer.resize(timeseriesLength, "");

    //Fill the buffer with empty values
    for(unsigned int i=0; i<timeseriesLength; i++) {
        dataBuffer.push_back(vector<float>(numChannels,0));
        highlightBuffer.push_back(0);
        labelBuffer.push_back("");
    }

    lockRanges = false;
    linkRanges = false;
    dynamicScale = false;
    globalMin =  std::numeric_limits<float>::max();
    globalMax =  -std::numeric_limits<float>::max();
    channelRanges.resize( numChannels, std::pair<float,float>(globalMin,globalMax) );
    channelNames.resize(numChannels,"");
    
    colors.resize(numChannels);
    //Setup the default colors
    if( numChannels >= 1 ) colors[0] = ofColor(255,0,0); //red
    if( numChannels >= 2 ) colors[1] = ofColor(0,255,0); //green
    if( numChannels >= 3 ) colors[2] = ofColor(0,0,255); //blue
    if( numChannels >= 4 ) colors[3] = ofColor(255,255,0); //yellow
    if( numChannels >= 5 ) colors[4] = ofColor(255,0,255); //purple
    if( numChannels >= 6 ) colors[5] = ofColor(255,255,255); //white

    //Randomize the remaining colors
    for(unsigned int n=6; n<numChannels; n++){
        colors[n][0] = ofRandom(100,255);
        colors[n][1] = ofRandom(100,255);
        colors[n][2] = ofRandom(100,255);
    }

    channelVisible.resize(numChannels,true);
    initialized = true;
    
    return true;    
}

bool ofxGrtTimeseriesPlot::reset(){

    std::unique_lock<std::mutex> lock( mtx );
        
    if( !initialized ) return false;
    
    if( !lockRanges ){
        globalMin =  std::numeric_limits<float>::max();
        globalMax =  -std::numeric_limits<float>::max();
        for(size_t i=0; i<channelRanges.size(); i++){
            channelRanges[i].first = globalMin;
            channelRanges[i].second = globalMax;
        }
    }

    //Clear the buffer
    dataBuffer.setAllValues(vector<float>(numChannels,0));
    highlightBuffer.setAllValues(0);
    labelBuffer.setAllValues("");
    
    return true;
}
    
bool ofxGrtTimeseriesPlot::setRanges( const float globalMin, const float globalMax, const bool lockRanges, const bool linkRanges, const bool dynamicScale ){
    std::unique_lock<std::mutex> lock( mtx );

    if( globalMin == globalMax ){
        return false;
    }
    this->globalMin = globalMin;
    this->globalMax = globalMax;
    this->lockRanges = lockRanges;
    this->linkRanges = linkRanges;
    this->dynamicScale = dynamicScale;
    for(size_t i=0; i<channelRanges.size(); i++){
        channelRanges[i].first = globalMin;
        channelRanges[i].second = globalMax;
    }
    return true;
}

bool ofxGrtTimeseriesPlot::setRanges( const vector< GRT::MinMax > &ranges, const bool lockRanges, const bool linkRanges, const bool dynamicScale ){

    std::unique_lock<std::mutex> lock( mtx );

    if( ranges.size() != channelRanges.size() ){
        return false;
    }

    this->lockRanges = lockRanges;
    this->linkRanges = linkRanges;
    this->dynamicScale = dynamicScale;

    globalMin =  std::numeric_limits<float>::max();
    globalMax =  -std::numeric_limits<float>::max();
    
    for(size_t i=0; i<channelRanges.size(); i++){
        channelRanges[i].first = ranges[i].minValue;
        channelRanges[i].second = ranges[i].maxValue;

        if( channelRanges[i].first < globalMin ){ globalMin = channelRanges[i].first; }
        else if( channelRanges[i].second > globalMax ){ globalMax = channelRanges[i].second; }
    }

    return true;
}

bool ofxGrtTimeseriesPlot::setData( const vector<float> &data ){

    std::unique_lock<std::mutex> lock( mtx );

    const unsigned int M = (unsigned int)data.size();

    if( numChannels != 1 ) return false;
    if( M != timeseriesLength ) return false;

    dataBuffer.reset(); highlightBuffer.reset(); labelBuffer.reset();

    if( !lockRanges ){
        globalMin =  std::numeric_limits<float>::max();
        globalMax =  -std::numeric_limits<float>::max();
        for(size_t i=0; i<channelRanges.size(); i++){
            channelRanges[i].first = globalMin;
            channelRanges[i].second = globalMax;
        }
    }
    
    for(size_t i=0; i<M; i++){
        dataBuffer(i)[0] = data[i];

        //Check the min and max values
        if( !lockRanges ){
            //Update the global min/max
            if( data[i] < globalMin ){ globalMin = data[i]; }
            else if( data[i] > globalMax ){ globalMax = data[i]; }

            //Update the channel min/max
            for(size_t j=0; j<channelRanges.size(); j++){
                if( data[i] < channelRanges[j].first ){ channelRanges[j].first = data[i]; }
                else if( data[i] > channelRanges[j].second ){ channelRanges[j].second = data[i]; }
            }
        }
    }

    return true;
}

bool ofxGrtTimeseriesPlot::setData( const vector<double> &data ){

    std::unique_lock<std::mutex> lock( mtx );

    const unsigned int M = (unsigned int)data.size();

    if( numChannels != 1 ) return false;
    if( M != timeseriesLength ) return false;

    dataBuffer.reset(); highlightBuffer.reset(); labelBuffer.reset();

    if( !lockRanges ){
        globalMin =  std::numeric_limits<double>::max();
        globalMax =  -std::numeric_limits<double>::max();
        for(size_t i=0; i<channelRanges.size(); i++){
            channelRanges[i].first = globalMin;
            channelRanges[i].second = globalMax;
        }
    }
    
    for(unsigned int i=0; i<M; i++){
        dataBuffer(i)[0] = data[i];
        highlightBuffer[i] = 0;

        //Check the min and max values
        if( !lockRanges ){
            //Update the global min/max
            if( data[i] < globalMin ){ globalMin = data[i]; }
            else if( data[i] > globalMax ){ globalMax = data[i]; }

            //Update the channel min/max
            for(size_t j=0; j<channelRanges.size(); j++){
                if( data[i] < channelRanges[j].first ){ channelRanges[j].first = data[i]; }
                else if( data[i] > channelRanges[j].second ){ channelRanges[j].second = data[i]; }
            }
        }
    }

    return true;
}
    
bool ofxGrtTimeseriesPlot::setData( const vector< vector<float> > &data ){

    std::unique_lock<std::mutex> lock( mtx );
    
    const unsigned int M = (unsigned int)data.size();

    if( numChannels != 1 ) return false;
    if( M != timeseriesLength ) return false;

    dataBuffer.reset(); highlightBuffer.reset(); labelBuffer.reset();

    if( !lockRanges ){
        globalMin =  std::numeric_limits<float>::max();
        globalMax =  -std::numeric_limits<float>::max();
        for(size_t i=0; i<channelRanges.size(); i++){
            channelRanges[i].first = globalMin;
            channelRanges[i].second = globalMax;
        }
    }
    
    for(size_t i=0; i<M; i++){
        if( data[i].size() != numChannels ){
            return false;
        }
        for(size_t j=0; j<numChannels; j++){
            dataBuffer(i)[j] = data[i][j];
            highlightBuffer[i] = 0;

            //Check the min and max values
            if( !lockRanges ){
                //Update the global min/max
                if( data[i][j] < globalMin ){ globalMin = data[i][j]; }
                else if( data[i][j] > globalMax ){ globalMax = data[i][j]; }

                //Update the channel min/max
                if( data[i][j] < channelRanges[j].first ){ channelRanges[j].first = data[i][j]; }
                else if( data[i][j] > channelRanges[j].second ){ channelRanges[j].second = data[i][j]; }
            }
        }
    }
    
    return true;
}
    
bool ofxGrtTimeseriesPlot::setData( const Matrix<float> &data ){

    std::unique_lock<std::mutex> lock( mtx );
    
    const unsigned int M = data.getNumRows();
    const unsigned int N = data.getNumCols();
    
    if( N != numChannels ){
        errorLog << "setData( const MatrixFloat &data ) - The number of dimensions in the data does not match the number of dimensions in the graph!" << endl;
        return false;
    }
    
    dataBuffer.reset(); highlightBuffer.reset(); labelBuffer.reset();

    if( !lockRanges ){
        globalMin =  std::numeric_limits<float>::max();
        globalMax =  -std::numeric_limits<float>::max();
        for(size_t i=0; i<channelRanges.size(); i++){
            channelRanges[i].first = globalMin;
            channelRanges[i].second = globalMax;
        }
    }

    for(unsigned int i=0; i<M; i++){
        for(unsigned int j=0; j<numChannels; j++){
            dataBuffer(i)[j] = data[i][j];
            highlightBuffer[i] = 0;

            //Check the min and max values
            if( !lockRanges ){
                //Update the global min/max
                if( data[i][j] < globalMin ){ globalMin = data[i][j]; }
                else if( data[i][j] > globalMax ){ globalMax = data[i][j]; }

                //Update the channel min/max
                if( data[i][j] < channelRanges[j].first ){ channelRanges[j].first = data[i][j]; }
                else if( data[i][j] > channelRanges[j].second ){ channelRanges[j].second = data[i][j]; }
            }
        }
    }
    
    return true;
}

bool ofxGrtTimeseriesPlot::setData( const Matrix<double> &data ){

    std::unique_lock<std::mutex> lock( mtx );

    const unsigned int M = data.getNumRows();
    const unsigned int N = data.getNumCols();
    
    if( N != numChannels ){
        errorLog << "setData( const MatrixDouble &data ) - The number of dimensions in the data does not match the number of dimensions in the graph!" << endl;
        return false;
    }
    dataBuffer.reset(); highlightBuffer.reset(); labelBuffer.reset();

    if( !lockRanges ){
        globalMin =  std::numeric_limits<double>::max();
        globalMax =  -std::numeric_limits<double>::max();
        for(size_t i=0; i<channelRanges.size(); i++){
            channelRanges[i].first = globalMin;
            channelRanges[i].second = globalMax;
        }
    }
    
    for(unsigned int i=0; i<M; i++){
        for(unsigned int j=0; j<numChannels; j++){
            dataBuffer(i)[j] = data[i][j];
            highlightBuffer[i] = 0;

            //Check the min and max values
            if( !lockRanges ){
                //Update the global min/max
                if( data[i][j] < globalMin ){ globalMin = data[i][j]; }
                else if( data[i][j] > globalMax ){ globalMax = data[i][j]; }

                //Update the channel min/max
                if( data[i][j] < channelRanges[j].first ){ channelRanges[j].first = data[i][j]; }
                else if( data[i][j] > channelRanges[j].second ){ channelRanges[j].second = data[i][j]; }
            }
        }
    }
    
    return true;
}

bool ofxGrtTimeseriesPlot::update(){

    std::unique_lock<std::mutex> lock( mtx );
    
    //If the buffer has not been initialised then return false, otherwise update the buffer
    if( !initialized ) return false;
    
    //Repeat the previos value
    dataBuffer.push_back( dataBuffer[timeseriesLength-1] );
    highlightBuffer.push_back( highlightBuffer[timeseriesLength - 1] );
    labelBuffer.push_back( labelBuffer[timeseriesLength - 1] );

    return true;
}

bool ofxGrtTimeseriesPlot::update( const vector<float> &data, bool highlight, std::string label ){

    std::unique_lock<std::mutex> lock( mtx );

    const unsigned int N = data.size();
    
    //If the buffer has not been initialised then return false, otherwise update the buffer
    if( !initialized || N != numChannels ) return false;
    
    //Add the new value to the buffer
    dataBuffer.push_back( data );
    highlightBuffer.push_back( highlight );
    labelBuffer.push_back( label );
    
    //Check the min and max values
    if( !lockRanges ){
        for(unsigned int j=0; j<channelRanges.size(); j++){
            //Update the global min/max
            if( data[j] < globalMin ){ globalMin = data[j]; }
            else if( data[j] > globalMax ){ globalMax = data[j]; }

            //Update the channel min/max
            if( data[j] < channelRanges[j].first ){ channelRanges[j].first = data[j]; }
            else if( data[j] > channelRanges[j].second ){ channelRanges[j].second = data[j]; }
        }
    }
    
    return true;
    
}

bool ofxGrtTimeseriesPlot::update( const vector<double> &data, bool highlight, std::string label ){

    const size_t N = data.size();
    vector<float> tmp(N);
    for(size_t i=0; i<N; i++){
        tmp[i] = data[i];
    }
    return update( tmp, highlight, label );
}
    
bool ofxGrtTimeseriesPlot::draw( const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h ){

    std::unique_lock<std::mutex> lock( mtx );
    
    if( !initialized ) return false;

    float minY = 0;
    float maxY = 0;

    if( dynamicScale ){
        globalMin =  std::numeric_limits<double>::max();
        globalMax =  -std::numeric_limits<double>::max();
        for(size_t i=0; i<channelRanges.size(); i++){
            channelRanges[i].first = globalMin;
            channelRanges[i].second = globalMax;
        }
        for(unsigned int i=0; i<timeseriesLength; i++){
            for(unsigned int j=0; j<numChannels; j++){
                //Update the global min/max
                if( dataBuffer[i][j] < globalMin ){ globalMin = dataBuffer[i][j]; }
                else if( dataBuffer[i][j] > globalMax ){ globalMax = dataBuffer[i][j]; }

                //Update the channel min/max
                if( dataBuffer[i][j] < channelRanges[j].first ){ channelRanges[j].first = dataBuffer[i][j]; }
                else if( dataBuffer[i][j] > channelRanges[j].second ){ channelRanges[j].second = dataBuffer[i][j]; }
            }
        }

        //Add a small percentage to the min/max values so the plot sits nicely in the graph
        for(unsigned int j=0; j<numChannels; j++){
            float range = channelRanges[j].first - channelRanges[j].second;
            if( range != 0 ){
                range = range * 0.1;
                channelRanges[j].first -= range;
                channelRanges[j].second += range;
            }
            
        }
    }

    //Bad things happen if the min and max values are the NAN or the same (as we can't scale the plots correctly)
    //So add a small value to the max if needed
    if( grt_isnan(globalMin) || grt_isinf(globalMin) ){
        globalMin = 0;
    }
    if( grt_isnan(globalMax) || grt_isinf(globalMax) ){
        globalMax = 1;
    }
    if( globalMin == globalMax ){
        globalMax += 1.0e-10;
    }
    if( !linkRanges ){
        for(size_t i=0; i<channelRanges.size(); i++){
            if( grt_isnan(channelRanges[i].first) || grt_isinf(channelRanges[i].first) ){
                channelRanges[i].first = 0;
            }
            if( grt_isnan(channelRanges[i].second) || grt_isinf(channelRanges[i].second) ){
                channelRanges[i].second = 0;
            }
            if( channelRanges[i].first == channelRanges[i].second ){
                channelRanges[i].second += 1.0e-10;
            }
        }
    }
    
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
    if( globalMin != globalMax ){
        float xPos = 0;
        float xStep = w / (float)timeseriesLength;
        ofSetColor(32);
        for(unsigned int i=0; i<highlightBuffer.getNumValuesInBuffer(); i++){
            if (highlightBuffer[i]) ofDrawRectangle( xPos, 0, xStep, h );
            xPos += xStep;
        }
        std::string label = "";
        xPos = 0;
        ofSetColor(255);
        ofFill();
        for(unsigned int i=0; i<highlightBuffer.getNumValuesInBuffer(); i++){
            if (highlightBuffer[i]) {
                if (labelBuffer[i] != label) {
                    ofDrawBitmapString(labelBuffer[i], xPos, h);
                    label = labelBuffer[i];
                }
            } else {
                label = "";
            }
            xPos += xStep;
        }
        unsigned int index = 0;
        unsigned int channelIndex = 0;
        ofNoFill();
        for(unsigned int n=0; n<numChannels; n++){
            xPos = 0;
            index = 0;
            channelIndex = drawOrderInverted ? numChannels-1-n : n;
            if( channelVisible[ channelIndex ] ){
                minY = linkRanges ? globalMin : channelRanges[ channelIndex ].first;
                maxY = linkRanges ? globalMax : channelRanges[ channelIndex ].second;
                ofSetColor( colors[ channelIndex ][0],colors[ channelIndex ][1],colors[ channelIndex ][2] );
                ofBeginShape();
                for(unsigned int i=0; i<timeseriesLength; i++){
                    ofVertex( xPos, ofMap(dataBuffer[i][ channelIndex ], minY, maxY, h, 0, constrainValuesToGraph) );
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

        if( drawInfoText ){
            ofRectangle bounds = font->getStringBoundingBox(plotTitle, 0, 0);
            int textX = 10;
            int textY = bounds.height + 5;
            int textSpacer = bounds.height + 5;

            if( plotTitle != "" && drawPlotTitle ){
                ofSetColor(textColor[0],textColor[1],textColor[2]);
                font->drawString( plotTitle, textX, textY );
                textY += textSpacer;
            }
            
            if( drawPlotValues ){
                std::stringstream info;
                info.precision( 2 );
                for(unsigned int n=0; n<numChannels; n++){
                    if( channelVisible[n] ){
                        minY = linkRanges ? globalMin : channelRanges[n].first;
                        maxY = linkRanges ? globalMax : channelRanges[n].second;
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
    }

    ofPopMatrix();
    
    return true;
}




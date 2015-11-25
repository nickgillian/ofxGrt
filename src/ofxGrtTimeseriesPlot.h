/*
 GRT MIT License
 Copyright (c) <2012> <Nicholas Gillian, Media Lab, MIT>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 and associated documentation files (the "Software"), to deal in the Software without restriction, 
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial 
 portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once
#include "ofMain.h"
#include "GRT/GRT.h"

namespace GRT{

class ofxGrtTimeseriesPlot{
public:
    ofxGrtTimeseriesPlot();
    ~ofxGrtTimeseriesPlot();
    
    /**
     @brief setups a plot, setting the length of the timeseries (the number of points that will be stored in the plots internal buffer) and
     the number of dimensions in the data.
     @note you must call this before updating or drawing the plot
     @param timeseriesLength: sets the size of the timeseries buffer
     @param numDimensions: sets the number of dimensions in the input data
     @param title: sets the title of the plot, an empty string will stop the title from being drawn
     @return returns true if the plot was setup successfully, false otherwise
    */
    bool setup(unsigned int timeseriesLength,unsigned int numDimensions,string title="");

    /**
     @brief updates the plot using the last input data, this is useful if you have no data but still want to update the graph.
     @return returns true if the plot was updated successfully, false otherwise
    */
    bool update();
    
    /**
     @brief updates the plot pushing the input data into the plots internal buffer. The size of the input vector must match the number of dimensions in the plot.
     @return returns true if the plot was updated successfully, false otherwise
    */
    bool update( const VectorDouble &data );

    /**
     @brief draws the plot.     
     @return returns true if the plot was drawn successfully, false otherwise
    */
    bool draw(unsigned int x,unsigned int y,unsigned int w,unsigned int h);
    
    bool reset();
    bool setData( const vector< VectorDouble > &data );
    bool setData( const MatrixDouble &data );
    bool setRanges(float minY,float maxY,bool lockRanges = false);
    bool setDrawGrid( bool drawGrid ){ this->drawGrid = drawGrid; return true; }
    bool setFont( const ofTrueTypeFont &font ){ this->font = &font; return this->font->isLoaded(); }
    bool setLockRanges(bool lockRanges){ this->lockRanges = lockRanges; return true; }
    bool setDrawInfoText(bool drawInfoText){ this->drawInfoText = drawInfoText; return true; }
    
protected:
    unsigned int numDimensions;
    unsigned int timeseriesLength;
    float minY;
    float maxY;
    string plotTitle;
    vector< string > channelNames;
    vector< bool > channelVisible;
    CircularBuffer< VectorDouble > dataBuffer;
    
    bool initialized;
    bool lockRanges;
    bool constrainValuesToGraph;
    bool drawInfoText;
    bool drawGrid;
    
    ofColor textColor;
    ofColor gridColor;
    ofColor backgroundColor;
    vector< ofColor > colors;
    ErrorLog errorLog;
    const ofTrueTypeFont *font;
    
};

}//End of namespace GRT

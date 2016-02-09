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

using namespace GRT;

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
     @param title: sets the title of the plot, an empty std::string will stop the title from being drawn
     @return returns true if the plot was setup successfully, false otherwise
    */
    bool setup(unsigned int timeseriesLength,unsigned int numDimensions,std::string title="");

    /**
     @brief updates the plot using the last input data, this is useful if you have no data but still want to update the graph.
     @return returns true if the plot was updated successfully, false otherwise
    */
    bool update();
    
    /**
     @brief updates the plot pushing the input data into the plots internal buffer. The size of the input Vector must match the number of dimensions in the plot.
     @return returns true if the plot was updated successfully, false otherwise
    */
    bool update( const vector<float> &data );
    
    /**
     @brief updates the plot pushing the input data into the plots internal buffer. The size of the input Vector must match the number of dimensions in the plot.
     @return returns true if the plot was updated successfully, false otherwise
    */
    bool update( const vector<double> &data );

    /**
     @brief draws the plot.     
     @return returns true if the plot was drawn successfully, false otherwise
    */
    bool draw(unsigned int x,unsigned int y,unsigned int w,unsigned int h);
    
    /**
     @brief resets the plot, this resets the axis min/max values and sets all the values in the plot buffer to zero
     @return returns true if the plot was reset successfully, false otherwise
    */
    bool reset();

    /**
     @brief directly fills the plot buffer with the data, this differs from update as instead of update(...) pushing data into the plot buffer, the setData function directly fills
     the entire plot buffer.  This function should only be called if the plot has been setup with a dimensionality of 1 and the size of the input data matches the length of the plot.
     @param data: the data that will be used to fill the plot buffer, the length of this vector should match the timeseries length of the plot
     @return returns true if the data was set successfully, false otherwise
    */
    bool setData( const vector<float> &data );

    /**
     @brief directly fills the plot buffer with the data, this differs from update as instead of update(...) pushing data into the plot buffer, the setData function directly fills
     the entire plot buffer.  This function should only be called if the plot has been setup with a dimensionality of 1 and the size of the input data matches the length of the plot.
     @param data: the data that will be used to fill the plot buffer, the length of this vector should match the timeseries length of the plot
     @return returns true if the data was set successfully, false otherwise
    */
    bool setData( const vector<double> &data );

    /**
     @brief directly fills the plot buffer with the data, this differs from update as instead of update(...) pushing data into the plot buffer, the setData function directly fills
     the entire plot buffer.  This function should only be called if the plot has been setup with a dimensionality of N (where N is the size of the inner vector) and the size of the outter data vector
     matches the length of the plot.
     @param data: the data that will be used to fill the plot buffer, the size of this outer vector should match the timeseries length of the plot, the size of the inner vector should match the number of dimensions
     @return returns true if the data was set successfully, false otherwise
    */
    bool setData( const vector< vector<float> > &data );

    /**
     @brief directly fills the plot buffer with the data, this differs from update as instead of update(...) pushing data into the plot buffer, the setData function directly fills
     the entire plot buffer.  This function should only be called if the plot has been setup with a dimensionality of N (where N is the number of columns in the matrix) with a timeseries length of M (where M is the number of rows in the matrix)
     @param data: the data that will be used to fill the plot buffer, the number of rows in the matrix should match the timeseries length of the plot, the number of columns in the matrix should match the number of dimensions in the plot
     @return returns true if the data was set successfully, false otherwise
    */
    bool setData( const Matrix<float> &data );

    /**
     @brief directly fills the plot buffer with the data, this differs from update as instead of update(...) pushing data into the plot buffer, the setData function directly fills
     the entire plot buffer.  This function should only be called if the plot has been setup with a dimensionality of N (where N is the number of columns in the matrix) with a timeseries length of M (where M is the number of rows in the matrix)
     @param data: the data that will be used to fill the plot buffer, the number of rows in the matrix should match the timeseries length of the plot, the number of columns in the matrix should match the number of dimensions in the plot
     @return returns true if the data was set successfully, false otherwise
    */
    bool setData( const Matrix<double> &data );
    bool setRanges(float minY,float maxY,bool lockRanges = false, bool linkRanges = false, bool dynamicScale = false);
    bool setDrawGrid( bool drawGrid ){ this->drawGrid = drawGrid; return true; }
    bool setFont( const ofTrueTypeFont &font ){ this->font = &font; return this->font->isLoaded(); }
    bool setLockRanges(const bool lockRanges){ this->lockRanges = lockRanges; return true; }
    bool setLinkRanges(const bool linkRanges){ this->linkRanges = linkRanges; return true; }
    bool setDynamicScale(const bool dynamicScale){ this->dynamicScale = dynamicScale; return true; }
    bool setDrawInfoText(const bool drawInfoText){ this->drawInfoText = drawInfoText; return true; }

    bool setChannelColors( const vector< ofColor > &colors ){
        if( colors.size() != numDimensions ) return false;
        this->colors = colors;
        return true;
    }

    /**
     @brief gets the global min and max range information.
     @returns returns the range information (minimum to maximum) in std::pair.
    */
    std::pair<float, float> getRanges() const { return std::make_pair(globalMin, globalMax); }

    /**
     @brief gets a vector containing the colors used to plot each channel (a.k.a. dimension) in the data
     @returns returns a vector containing the colors used to plot each channel (a.k.a. dimension) in the data
    */
    vector< ofColor > getChannelColors() const { return colors; }

protected:
    unsigned int numDimensions;
    unsigned int timeseriesLength;
    float globalMin;
    float globalMax;
    std::string plotTitle;
    vector< std::string > channelNames;
    vector< bool > channelVisible;
    vector< std::pair<float,float> > channelRanges;
    CircularBuffer< vector<float> > dataBuffer;
    
    bool initialized;
    bool lockRanges; ///< If true, then the min/max values for the plots will not be updated
    bool linkRanges; ///< If true, then the min/max values for the plots will be based on the global min/max values across all channels, if false then the min/max values will be based per channel
    bool dynamicScale; ///< If true, then the min/max values for the plots will be based on the current min/max values in the buffer, if false then the min/max values will be based over all values seen (not just the values in the current buffer)
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



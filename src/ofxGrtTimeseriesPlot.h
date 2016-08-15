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
     the number of dimensions (channels) in the data.
     @note you must call this before updating or drawing the plot
     @param timeseriesLength: sets the size of the timeseries buffer
     @param numChannels: sets the number of dimensions in the input data
     @param title: sets the title of the plot, an empty std::string will stop the title from being drawn
     @return returns true if the plot was setup successfully, false otherwise
    */
    bool setup( const unsigned int timeseriesLength, const unsigned int numChannels, const std::string title="");

    /**
     @brief updates the plot using the last input data, this is useful if you have no data but still want to update the graph.
     @return returns true if the plot was updated successfully, false otherwise
    */
    bool update();
    
    /**
     @brief updates the plot pushing the input data into the plots internal buffer. The size of the input Vector must match the number of dimensions in the plot.
     @param highlight whether or not to highlight the newly added data point (default false)
     @param label the label associated with the highlight
     @return returns true if the plot was updated successfully, false otherwise
    */
    bool update( const vector<float> &data, bool highlight = false, std::string label = "" );

    /**
     @brief updates the plot pushing the input data into the plots internal buffer. The size of the input Vector must match the number of dimensions in the plot.
     @param highlight whether or not to highlight the newly added data point (default false)
     @param label the label associated with the highlight
     @return returns true if the plot was updated successfully, false otherwise
    */
    bool update( const vector<double> &data, bool highlight = false, std::string label = "" );

    /**
     @brief draws the plot.     
     @return returns true if the plot was drawn successfully, false otherwise
    */
    bool draw( const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h );
    
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

    /**
     @brief sets the Y axis ranges used to scale the data for plotting.  The minY and maxY values set the min/max range of the plot
     @param minY: the minimum range for the Y axis
     @param maxY: the maximum range for the Y axis
     @param lockRanges: if true, then the ranges of the plot will be fixed to minY/maxY, if false then the min/max ranges of the plot will be updated based on the min/max values observed in the data
     @param linkRanges: if true, then the channels of the plot will all be scaled using a global min/max value (updated across all ranges). If false, then each channel will have it's own min/max value and be updated independently from the other channels
     @param dynamicScale: if true, then the contents of the plot will be scaled by on the current min/max values found in the timeseries buffer
     @return returns true if the parameters were update successfully, false otherwise
    */
    bool setRanges( const float minY, const float maxY, const bool lockRanges = false, const bool linkRanges = false, const bool dynamicScale = false );

    /**
     @brief sets the Y axis ranges used to scale the data for plotting.  The minY and maxY values set the min/max range of the plot
     @param ranges: a vector containing the min/max values for each channel
     @param lockRanges: if true, then the ranges of the plot will be fixed to minY/maxY, if false then the min/max ranges of the plot will be updated based on the min/max values observed in the data
     @param linkRanges: if true, then the channels of the plot will all be scaled using a global min/max value (updated across all ranges). If false, then each channel will have it's own min/max value and be updated independently from the other channels
     @param dynamicScale: if true, then the contents of the plot will be scaled by on the current min/max values found in the timeseries buffer
     @return returns true if the parameters were update successfully, false otherwise
    */
    bool setRanges( const vector< GRT::MinMax > &ranges, const bool lockRanges = false, const bool linkRanges = false, const bool dynamicScale = false );

    /**
     @brief controls if a background grid pattern will be rendered behind the main graph
     @param drawGrid: if true, then the grid will be drawn
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setDrawGrid( const bool drawGrid ){ 
        std::unique_lock<std::mutex> lock( mtx );
        this->drawGrid = drawGrid; 
        return true; 
    }

    /**
     @brief sets the font used to draw text on the plot, note that the timeseries plot instance stores a pointer to the font, you must therefore ensure the font instance remains valid until the timeseries plot instance is destroyed
     @param font: a reference to the font that will be used to draw text on the plot
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setFont( const ofTrueTypeFont &font ){ 
        std::unique_lock<std::mutex> lock( mtx );
        this->font = &font; 
        return this->font->isLoaded(); 
    }

    /**
     @brief controls if the Y axis plot ranges should be locked, if locked the min/max values used to scale data for plotting will be fixed
     @param lockRanges: if true, then the plot ranges will be locked
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setLockRanges( const bool lockRanges ){ 
        std::unique_lock<std::mutex> lock( mtx );
        this->lockRanges = lockRanges; 
        return true; 
    }

    /**
     @brief controls if the Y axis plot ranges should be linked across all channels, if linked then global min/max values will be used to scale data for plotting, otherwise each channel will have its own min/max value
     @param linkRanges: if true, then the plot ranges will all be scaled using a global min/max value
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setLinkRanges( const bool linkRanges ){ 
        std::unique_lock<std::mutex> lock( mtx );
        this->linkRanges = linkRanges; 
        return true; 
    }

    /**
     @brief controls if the Y axis plot ranges should be dynamically scaled based on the current contents of the timeseries buffer
     @param dynamicScale: if true, then the plot ranges will all be dynamicall scaled using the current min/max values of the contents of the timeseries buffer
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setDynamicScale( const bool dynamicScale ){ 
        std::unique_lock<std::mutex> lock( mtx );
        this->dynamicScale = dynamicScale; 
        return true; 
    }

    /**
     @brief controls if the plot should render text (plot title,latest values,etc.), if false then no text will be rendered, if true then the text rendered will depend on the state of the other drawText variables (i.e. drawPlotTitle, drawPlotValues)
     @param drawInfoText: if true, then the plot will render the info text
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setDrawInfoText( const bool drawInfoText ){ 
        std::unique_lock<std::mutex> lock( mtx );
        this->drawInfoText = drawInfoText; 
        return true; 
    }

    /**
     @brief controls if the plot title should rendered, if true, the title will only be shown if drawInfoText is true and the plot font has been set
     @param drawPlotTitle: if true, then the plot title will be rendered
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setDrawPlotTitle( const bool drawPlotTitle ){ 
        std::unique_lock<std::mutex> lock( mtx );
        this->drawPlotTitle = drawPlotTitle; 
        return true; 
    }

    /**
     @brief controls if the plot values should rendered, these show the most recent data value added for each channel. If true, the values will only be shown if drawInfoText is true and the plot font has been set
     @param drawPlotValues: if true, then the plot values will be rendered
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setDrawPlotValue( const bool drawPlotValues ) { 
        std::unique_lock<std::mutex> lock( mtx );
        this->drawPlotValues = drawPlotValues; 
        return true; 
    }

    /**
     @brief sets the colors used for each channel
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setChannelColors( const vector< ofColor > &colors ) {
        std::unique_lock<std::mutex> lock( mtx );
        if( colors.size() != numChannels ) return false;
        this->colors = colors;
        return true;
    }

    /**
     @brief sets the text used for each channel
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setChannelNames( const vector< string > &channelNames ) {
        std::unique_lock<std::mutex> lock( mtx );
        if( channelNames.size() != numChannels ) return false;
        this->channelNames = channelNames;
        return true;
    }

    /**
     @brief sets if the order in which channels are drawn is inverted, if true, then the first channel will be drawn last (which will drawn it on 'top' of the other channels)
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setDrawOrderInverted( const bool drawOrderInverted ){
        std::unique_lock<std::mutex> lock( mtx );
        this->drawOrderInverted = drawOrderInverted;
        return true;
    }

    /**
     @brief sets the background color of the plot
     @return returns true if the parameter was update successfully, false otherwise
    */
    bool setBackgroundColor( const ofColor &backgroundColor ){
        std::unique_lock<std::mutex> lock( mtx );
        this->backgroundColor = backgroundColor;
        return true;
    }

    /**
     @brief gets the global min and max range information.
     @returns returns the range information (minimum to maximum) in std::pair.
    */
    std::pair<float, float> getRanges() const { 
        std::unique_lock<std::mutex> lock( mtx );
        return std::make_pair(globalMin, globalMax); 
    }

    /**
     @brief gets a vector containing the colors used to plot each channel (a.k.a. dimension) in the data
     @returns returns a vector containing the colors used to plot each channel (a.k.a. dimension) in the data
    */
    vector< ofColor > getChannelColors() const { 
        std::unique_lock<std::mutex> lock( mtx );
        return colors; 
    }

protected:
    mutable std::mutex mtx;
    unsigned int numChannels;
    unsigned int timeseriesLength;
    float globalMin;
    float globalMax;
    std::string plotTitle;
    vector< std::string > channelNames;
    vector< bool > channelVisible;
    vector< std::pair<float,float> > channelRanges;
    CircularBuffer< vector<float> > dataBuffer;
    CircularBuffer< int > highlightBuffer;
    CircularBuffer< std::string > labelBuffer;

    bool initialized;
    bool lockRanges; ///< If true, then the min/max values for the plots will not be updated
    bool linkRanges; ///< If true, then the min/max values for the plots will be based on the global min/max values across all channels, if false then the min/max values will be based per channel
    bool dynamicScale; ///< If true, then the min/max values for the plots will be based on the current min/max values in the buffer, if false then the min/max values will be based over all values seen (not just the values in the current buffer)
    bool constrainValuesToGraph;
    bool drawInfoText;
    bool drawPlotTitle;
    bool drawPlotValues;
    bool drawGrid;
    bool drawOrderInverted;
    
    ofColor textColor;
    ofColor gridColor;
    ofColor backgroundColor;
    vector< ofColor > colors;
    ErrorLog errorLog;
    const ofTrueTypeFont *font;
    
};



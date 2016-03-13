#pragma once
#include "ofMain.h"
#include "GRT/GRT.h"

using namespace GRT;

#define BIG_POSITIVE_VALUE 99e+99
#define BIG_NEGATIVE_VALUE -99e+99

class ofxGrtBarPlot{

public:
    ofxGrtBarPlot();
    ~ofxGrtBarPlot();
    
    /**
     @brief sets up a plot, setting the number of dimensions (bars) in the bar plot
     @note you must call this before updating or drawing the plot
     @param numDimensions: sets the number of dimensions in the input data
     @param title: sets the title of the plot, an empty std::string will stop the title from being drawn
     @return returns true if the plot was setup successfully, false otherwise
    */
    bool setup(unsigned int numDimensions,const std::string title="");

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
    bool draw( const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h );

    bool resetAxisRanges();

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


protected:
    mutable std::mutex mtx;
    UINT numDimensions;
    vector< float > minRanges;
    vector< float > maxRanges;
    vector< float > data;
    bool initialized;
    bool rangesComputed;
    bool lockRanges;
    bool linkRanges;
    bool dynamicScale;
    bool constrainValuesToGraph;
    bool drawInfoText;
    bool drawGrid;
    vector< string > channelNames;
    ofColor backgroundColor;
    ofColor gridColor;
    ofColor barColor;
    string title;
    
    WarningLog warningLog;
    const ofTrueTypeFont *font;
    
};
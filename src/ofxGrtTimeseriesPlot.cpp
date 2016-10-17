#include "ofxGrtTimeseriesPlot.h"
#include <algorithm>

using namespace GRT;
    
ofxGrtTimeseriesPlot::ofxGrtTimeseriesPlot(){
    config = ofxGrtSettings::GetInstance().get();
    plotTitle = "";
    font = &config.get()->fontNormal;
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
    textColor = config->activeTextColor;
    backgroundColor = config->backgroundColor;
    gridColor = config->gridColor;
    axisColor = config->axisColor;
    
    errorLog.setProceedingText("[ERROR ofxGrtTimeseriesPlot]");
    xAxisInfo = "X";
    yAxisInfo = "   Y";
    insetPlotByInfoMarginX = true;
    insetPlotByInfoMarginY = true;
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

    dataBuffer.resize(timeseriesLength, vector<float>(numChannels,-1));
    highlightBuffer.resize(timeseriesLength, 0);
    labelBuffer.resize(timeseriesLength, "");

    //Fill the buffer with empty values
    for(unsigned int i=0; i<timeseriesLength; i++) {
        dataBuffer.push_back(vector<float>(numChannels,-1));
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
    
//    labelPlotcolors

    channelVisible.resize(numChannels,true);
    
    initialized = true;
    
    labelPlotColors.resize(10);
    
    labelPlotColors[0].background = {16, 16, 16, 40};
    labelPlotColors[0].label = {0, 0, 0, 255};
    
    labelPlotColors[1].background = {16, 16, 16, 40};
    labelPlotColors[1].label = {0, 0, 0, 255};
    
    labelPlotColors[2].background = {16, 16, 16, 40};
    labelPlotColors[2].label = {0, 0, 0, 255};
    
    labelPlotColors[3].background = {16, 16, 16, 40};
    labelPlotColors[3].label = {0, 0, 0, 255};
    
    labelPlotColors[4].background = {16, 16, 16, 40};
    labelPlotColors[4].label = {0, 0, 0, 255};
    
    labelPlotColors[5].background = {16, 16, 16, 40};
    labelPlotColors[5].label = {0, 0, 0, 255};
    
    labelPlotColors[6].background = {16, 16, 16, 40};
    labelPlotColors[6].label = {0, 0, 0, 255};
    
    labelPlotColors[7].background = {16, 16, 16, 40};
    labelPlotColors[7].label = {0, 0, 0, 255};
    
    labelPlotColors[8].background = {16, 16, 16, 40};
    labelPlotColors[8].label = {0, 0, 0, 255};
    
    labelPlotColors[9].background = {16, 16, 16, 40};
    labelPlotColors[9].label = {0, 0, 0, 255};
    
    labelPlotColors = {
        {config.get()->activeTextColor,{0, 0, 0, 255}},
        {{255, 127, 0, 255},{255,255,255,255}},
        {{31, 120, 180, 255},{255,255,255,255}},
        {{210, 189, 26, 255},{255,255,255,255}},
        {{227, 26, 28, 255},{255,255,255,255}},
        {{80, 160, 44, 255},{255,255,255,255}},
        {{106, 61, 154, 255},{255,255,255,255}},
        {{177, 89, 40, 255},{255,255,255,255}},
        {{9, 152, 146, 255},{255,255,255,255}},
        {{227, 26, 150, 255},{255,255,255,255}}
        };

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
    int i=0;
    for(auto &c:colors)
    {
        c=labelPlotColors[i].background;
        i++;
    }
    
    return true;    
}

void ofxGrtTimeseriesPlot::setAxisTitle(const std::string x, const std::string y)
{
    xAxisInfo = x;
    yAxisInfo = y;
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
    dataBuffer.setAllValues(vector<float>(numChannels,-1));
    highlightBuffer.setAllValues(0);
    labelBuffer.setAllValues("");
    
    return true;
}
    
bool ofxGrtTimeseriesPlot::setRanges( const float globalMin, const float globalMax, const vector<labelPlotColor> labelPlotColors, const bool lockRanges, const bool linkRanges, const bool dynamicScale ){
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
    
    if(labelPlotColors.size()>0)
        this->labelPlotColors = labelPlotColors;
    
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

bool ofxGrtTimeseriesPlot::setRanges( const vector< GRT::MinMax > &ranges, const vector<labelPlotColor> labelPlotColors, const bool lockRanges, const bool linkRanges, const bool dynamicScale ){

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
    
    if(labelPlotColors.size()>0)
        this->labelPlotColors = labelPlotColors;


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
    
/*
bool ofxGrtTimeseriesPlot::setData( const vector< vector<float> > &data, const bool rowsAreChannels ){

	std::unique_lock<std::mutex> lock( mtx );
	
	if( rowsAreChannels ){
		//The outer vector (rows) should contain the channel data
		//The inner vector (cols) should contain the timeseries data for channel n
		const unsigned int N = (unsigned int)data.size();

    		if( N != numChannels ) return false;
		
		dataBuffer.reset();

		if( !lockRanges ){
			globalMin =  std::numeric_limits<float>::max();
			globalMax =  -std::numeric_limits<float>::max();
			for(size_t i=0; i<channelRanges.size(); i++){
            			channelRanges[i].first = globalMin;
            			channelRanges[i].second = globalMax;
        		}
    		}
    
    		for(unsigned int i=0; i<N; i++){
        		if( data[i].size() != timeseriesLength ){
            			return false;
        		}
        		for(unsigned int j=0; j<timeseriesLength; j++){
            			dataBuffer(j)[i] = data[i][j];

            			//Check the min and max values
            			if( !lockRanges ){
                			//Update the global min/max
                			if( data[i][j] < globalMin ){ globalMin = data[i][j]; }
                			else if( data[i][j] > globalMax ){ globalMax = data[i][j]; }

                			//Update the channel min/max
                			if( data[i][j] < channelRanges[i].first ){ channelRanges[i].first = data[i][j]; }
                			else if( data[i][j] > channelRanges[i].second ){ channelRanges[i].second = data[i][j]; }
            			}
        		}		
    		}
	}else{



	}
	
    return true;
    */

bool ofxGrtTimeseriesPlot::setData( const vector< vector<float> > &data, const bool rowsAreChannels ){

    std::unique_lock<std::mutex> lock( mtx );
    
    const unsigned int M = (unsigned int)data.size();

    dataBuffer.reset(); highlightBuffer.reset(); labelBuffer.reset();

    if( !lockRanges ){
        globalMin =  std::numeric_limits<float>::max();
        globalMax =  -std::numeric_limits<float>::max();
        const size_t numChannels = channelRanges.size();
        for(size_t i=0; i<numChannels; i++){
            channelRanges[i].first = globalMin;
            channelRanges[i].second = globalMax;
        }
    }

    if( rowsAreChannels ){
        //The outer vector (rows) should contain the channel data
        //The inner vector (cols) should contain the timeseries data for channel n

        const size_t numRows = data.size();

        if( numRows != numChannels ) return false;

        for(size_t i=0; i<numRows; i++){
            if( data[i].size() != timeseriesLength ){
                return false;
            }
            for(unsigned int j=0; j<timeseriesLength; j++){
                dataBuffer(j)[i] = data[i][j];

                //Check the min and max values
                if( !lockRanges ){
                    //Update the global min/max
                    if( data[i][j] < globalMin ){ globalMin = data[i][j]; }
                    else if( data[i][j] > globalMax ){ globalMax = data[i][j]; }

                    //Update the channel min/max
                    if( data[i][j] < channelRanges[i].first ){ channelRanges[i].first = data[i][j]; }
                    else if( data[i][j] > channelRanges[i].second ){ channelRanges[i].second = data[i][j]; }
                }
            }       
        }
        
        return true;
    }else{
        //The outer vector (rows) should contain the timeseries data for channel n
        //The inner vector (cols) should contain the channel data

        const size_t numRows = data.size();

        if( numRows != timeseriesLength ) return false;
    
        for(size_t i=0; i<numRows; i++){
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

    return false;
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

bool ofxGrtTimeseriesPlot::setChannelVisible(const int idx, bool visible)
{
    try{
        channelVisible.at(idx) = visible;
        return true;
    }catch(const std::exception& e)
    {
        std::cout << " a standard exception was caught, with message '"
        << e.what() << "'\n";
        return false;
    }
    
}

bool ofxGrtTimeseriesPlot::setNamesForChannels(const vector<std::string> names)
{
    if(names.size()==channelNames.size())
    {
        channelNames=names;
        return true;
    }
    return false;
    
}

vector< std::string > ofxGrtTimeseriesPlot::getChannelNames()
{
    return channelNames;
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

bool ofxGrtTimeseriesPlot::update( const vector<float> &data, std::string label )
{
    std::unique_lock<std::mutex> lock( mtx );
    
    const unsigned int N = data.size();
    
    //If the buffer has not been initialised then return false, otherwise update the buffer
    if( !initialized || N != numChannels ) return false;
    
    //Add the new value to the buffer
    dataBuffer.push_back( data );
    highlightBuffer.push_back( false );
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

bool ofxGrtTimeseriesPlot::update( const vector<double> &data, std::string label )
{
    const size_t N = data.size();
    vector<float> tmp(N);
    for(size_t i=0; i<N; i++){
        tmp[i] = data[i];
    }
    
    return update( tmp, label );
    
}
    
bool ofxGrtTimeseriesPlot::draw( int x, int y, int w, int h ){
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
    
    if (!insetPlotByInfoMarginX) {
        x -= config->info_margin;
        w += config->info_margin;
    }

    if (!insetPlotByInfoMarginY) {
        y -= config->info_margin;
        h += config->info_margin;
    }

    ofPushMatrix();
    ofEnableAlphaBlending();
    ofTranslate(x, y);
    
    //Draw the background
    ofFill();
    ofSetColor(backgroundColor);
    ofDrawRectangle(config->info_margin,0,w-config->info_margin,h-config->info_margin);
    
    //Draw the grid if required
    if( drawGrid ){
        
        ofSetColor(gridColor);
        unsigned int numVLines = 20;
        unsigned int numHLines = 10;
        
        //Draw the horizontal lines
        for(unsigned int i=0; i<=numHLines; i++){
            float xStart = config->info_margin;
            float xEnd = w;
            float yStart = ofMap(i,0,numHLines,0,h-config->info_margin);
            float yEnd = yStart;
            ofDrawLine(xStart,yStart,xEnd,yEnd);
        }
        
        //Draw the vertical lines
        for(unsigned int i=0; i<=numVLines; i++){
            float xStart = ofMap(i,0,numVLines,config->info_margin,w);
            float xEnd = xStart;
            float yStart = 0;
            float yEnd = h-config->info_margin;
            ofDrawLine(xStart,yStart,xEnd,yEnd);
        }
    }
    
    //Draw the axis lines
    ofSetColor(axisColor);
    ofDrawLine(-5+config->info_margin,h-config->info_margin,w+5,h-config->info_margin); //X Axis
    ofDrawLine(0+config->info_margin,-5,0+config->info_margin,h+5-config->info_margin); //Y Axis
    
    ofSetColor(textColor[0],textColor[1],textColor[2]);
    //Draw axis info
    if(font)
    {

        const float posX = -5+config->info_margin;
        const float posY = h;
        
        font->drawString(xAxisInfo, posX, posY);
        
        ofPushMatrix();
        {
            ofRotateZ(-90.0f);
            
            const float posY = -float(h)+font->stringWidth(yAxisInfo)-config->info_margin;
            const float posX = font->stringHeight(yAxisInfo);
            font->drawString(yAxisInfo, posY, posX);
        }
        ofPopMatrix();
        
    }
    
    //draw axis ticks
    {
        ofSetColor(axisColor);
        unsigned int numVTicks = 20;
        unsigned int numHTicks = 10;
        
        //Draw the horizontal lines
        for(unsigned int i=0; i<=numHTicks; i++){
            float xStart = -config->axisTicksSize+config->info_margin;
            float xEnd = config->axisTicksSize+config->info_margin;
            float yStart = ofMap(i,0,numHTicks,0,h-config->info_margin);
            float yEnd = yStart;
            ofDrawLine(xStart,yStart,xEnd,yEnd);
            
        }
        
        //Draw the vertical lines
        for(unsigned int i=0; i<=numVTicks; i++){
            float xStart = ofMap(i,0,numVTicks,config->info_margin,w);
            float xEnd = xStart;
            float yStart = h-config->info_margin;
            float yEnd = h+5-config->info_margin;
            ofDrawLine(xStart,yStart,xEnd,yEnd);
        }
    }
    
    //Draw the timeseries
    if( globalMin != globalMax ){
        float xPos = config->info_margin;
        float xStep = (w-config->info_margin) / (float)timeseriesLength;
        ofSetColor(32);
        for(unsigned int i=0; i<highlightBuffer.getNumValuesInBuffer(); i++){
            if (highlightBuffer[i]) ofDrawRectangle( xPos, 0, xStep, h-config->info_margin );
            xPos += xStep;
        }
        std::string label = "";
        xPos = config->info_margin;
        ofSetColor(255);
        ofFill();
        for(unsigned int i=0; i<highlightBuffer.getNumValuesInBuffer(); i++){
            if (highlightBuffer[i]) {
                if (labelBuffer[i] != label) {
                    ofDrawBitmapString(labelBuffer[i], xPos, h-config->info_margin);
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
            xPos = config->info_margin;
            index = 0;
            channelIndex = drawOrderInverted ? numChannels-1-n : n;
            if( channelVisible[ channelIndex ] ){
                minY = linkRanges ? globalMin : channelRanges[ channelIndex ].first;
                maxY = linkRanges ? globalMax : channelRanges[ channelIndex ].second;
                ofSetColor( colors[ channelIndex ][0],colors[ channelIndex ][1],colors[ channelIndex ][2] );
                ofBeginShape();
                for(unsigned int i=0; i<timeseriesLength; i++){
                    ofVertex( xPos, ofMap(dataBuffer[i][ channelIndex ], minY, maxY, h-config->info_margin, 0, constrainValuesToGraph) );
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
            int textX = config->info_margin;
            int textY = -config->titleTextSpacer;
//            int textSpacer = bounds.height + 5;
            
            if( plotTitle != "" && drawPlotTitle ){
                ofSetColor(textColor);
                font->drawString( plotTitle, textX, textY );
                
//
            }
            textY += font->getLineHeight();
            
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

bool ofxGrtTimeseriesPlot::drawLabeledGraph( const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h, const int chanNum){
    
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
    ofDrawRectangle(config->info_margin,config->info_margin,w-config->info_margin,h-config->info_margin*2);
    
    //Draw the grid if required
    if( drawGrid ){
        
        ofSetColor(gridColor[0],gridColor[1],gridColor[2],gridColor[3]);
        unsigned int numVLines = 20;
        unsigned int numHLines = 10;
        
        //Draw the horizontal lines
        for(unsigned int i=0; i<=numChannels; i++){
            float xStart = config->info_margin;
            float xEnd = w;
            float yStart = ofMap(i,0,numChannels,config->info_margin,h-config->info_margin);
            float yEnd = yStart;
            ofDrawLine(xStart,yStart,xEnd,yEnd);
        }
        
        //Draw the vertical lines
        for(unsigned int i=0; i<=numVLines; i++){
            float xStart = ofMap(i,0,numVLines,config->info_margin,w);
            float xEnd = xStart;
            float yStart = config->info_margin;
            float yEnd = h-config->info_margin;
            ofDrawLine(xStart,yStart,xEnd,yEnd);
        }
    }
    
    //Draw the axis lines
    ofSetColor(axisColor);
    ofDrawLine(-5+config->info_margin,h-config->info_margin,w+5,h-config->info_margin); //X Axis
    ofDrawLine(0+config->info_margin,-5+config->info_margin,0+config->info_margin,h+5-config->info_margin); //Y Axis
    
    ofSetColor(textColor);
    //Draw axis info
    if(font)
    {
        
        const float posX = -5+config->info_margin;
        const float posY = h;
        
        font->drawString(xAxisInfo, posX, posY);
        
        ofPushMatrix();
        {
            
            
            const float posY = -float(h)+font->stringWidth(yAxisInfo)+config->info_margin/2;
            const float posX = font->stringHeight(yAxisInfo);
            ofRotateZ(-90.0f);
            font->drawString(yAxisInfo, posY, posX);
        }
        ofPopMatrix();
    }
    
    //draw axis ticks
    {
        ofSetColor(gridColor);
        unsigned int numVTicks = 20;
        unsigned int numHTicks = 1;
        
        //Draw the horizontal lines
        for(unsigned int i=0; i<=numHTicks; i++){
            float xStart = -config->axisTicksSize+config->info_margin;
            float xEnd = 0+config->info_margin;
            float yStart = ofMap(i,0,numHTicks,config->info_margin,h-config->info_margin);
            float yEnd = yStart;
            ofDrawLine(xStart,yStart,xEnd,yEnd);
            
        }
        
        //Draw the vertical lines
        for(unsigned int i=0; i<=numVTicks; i++){
            float xStart = ofMap(i,0,numVTicks,config->info_margin,w);
            float xEnd = xStart;
            float yStart = h-config->info_margin;
            float yEnd = h+config->axisTicksSize-config->info_margin;
            ofDrawLine(xStart,yStart,xEnd,yEnd);
        }
    }
    
    //Draw the timeseries
    if( globalMin != globalMax ){
        float xPos = config->info_margin;
        const float yPos = config->info_margin;
        const float xStep = (w-config->info_margin) / (float)timeseriesLength;
        ofSetColor(32);
        
        vector<string> labels;
        vector<ofPoint> positions;
        vector<ofColor> colors;
        
        for(unsigned int i=0; i<labelBuffer.getNumValuesInBuffer(); i++)
        {
            const int colorIdx = dataBuffer[i][chanNum];
            if(colorIdx<0)
            {
                ofSetColor(0,0,0,0);
                ofDrawRectangle( xPos, yPos, xStep, h-config->info_margin*2 );
            }
            else
            {
                ofSetColor(labelPlotColors[colorIdx].background);
                ofDrawRectangle( xPos, yPos, xStep, h-config->info_margin*2 );
                const int prevIdx = max<int>(0,i-1);
                if(dataBuffer[i][chanNum]!=dataBuffer[prevIdx][chanNum] || i==0)
                {
                    labels.push_back(to_string((int)dataBuffer[i][chanNum]));
                    positions.push_back(ofPoint(xPos+2, config->info_margin+(h-config->info_margin*2+font->stringHeight(to_string((int)dataBuffer[i][chanNum])))*0.5));
                    colors.push_back(labelPlotColors[dataBuffer[i][chanNum]].label);
                }
            }
            
            xPos += xStep;
        }
        
        for(int i=0;i<labels.size();i++)
        {
            ofSetColor( colors[i] );
            if(font)
                font->drawString(labels[i], positions[i].x,positions[i].y);
            else
                ofDrawBitmapString(labels[i], positions[i].x,positions[i].y);
        }
        
        
        
    }
    
    //Only draw the text if the font has been loaded
    if( font ){
        
        if( !font->isLoaded() ) return false;
        
        if( drawInfoText ){
            ofRectangle bounds = font->getStringBoundingBox(plotTitle, 0, 0);
            int textX = config->info_margin;
            int textY = bounds.height;
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




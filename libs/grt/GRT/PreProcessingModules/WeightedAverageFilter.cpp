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

#include "WeightedAverageFilter.h"

GRT_BEGIN_NAMESPACE
    
//Register the WeightedAverageFilter module with the PreProcessing base class
RegisterPreProcessingModule< WeightedAverageFilter > WeightedAverageFilter::registerModule("WeightedAverageFilter");

WeightedAverageFilter::WeightedAverageFilter(UINT filterSize,UINT numDimensions){
    
    classType = "WeightedAverageFilter";
    preProcessingType = classType;
    debugLog.setProceedingText("[DEBUG WeightedAverageFilter]");
    errorLog.setProceedingText("[ERROR WeightedAverageFilter]");
    warningLog.setProceedingText("[WARNING WeightedAverageFilter]");
    init(filterSize,numDimensions);
}
    
WeightedAverageFilter::WeightedAverageFilter(const WeightedAverageFilter &rhs){
    
    classType = "WeightedAverageFilter";
    preProcessingType = classType;
    debugLog.setProceedingText("[DEBUG WeightedAverageFilter]");
    errorLog.setProceedingText("[ERROR WeightedAverageFilter]");
    warningLog.setProceedingText("[WARNING WeightedAverageFilter]");
    
    //Zero this instance
    this->filterSize = 0;
    this->inputSampleCounter = 0;
    
    //Copy the settings from the rhs instance
    *this = rhs;
}
    
WeightedAverageFilter::~WeightedAverageFilter(){

}
    
WeightedAverageFilter& WeightedAverageFilter::operator=(const WeightedAverageFilter &rhs){
    if(this!=&rhs){
        //Clear this instance
        this->filterSize = 0;
        this->inputSampleCounter = 0;
        this->dataBuffer.clear();
        this->weights = rhs.weights;
    
        //Copy from the rhs instance
        if( rhs.initialized ){
            this->init( rhs.filterSize, rhs.numInputDimensions );
            this->dataBuffer = rhs.dataBuffer;
        }
        
        //Copy the preprocessing base variables
        copyBaseVariables( (PreProcessing*)&rhs );
    }
    return *this;
}
    
bool WeightedAverageFilter::deepCopyFrom(const PreProcessing *preProcessing){
    
    if( preProcessing == NULL ) return false;
    
    if( this->getPreProcessingType() == preProcessing->getPreProcessingType() ){
        
        //Call the equals operator
        *this = *(WeightedAverageFilter*)preProcessing;
        
	return true;
    }
    
    errorLog << "clone(const PreProcessing *preProcessing) -  PreProcessing Types Do Not Match!" << std::endl;
    
    return false;
}

    
bool WeightedAverageFilter::process(const VectorFloat &inputVector){
    
    if( !initialized ){
        errorLog << "process(const VectorFloat &inputVector) - The filter has not been initialized!" << std::endl;
        return false;
    }

    if( inputVector.size() != numInputDimensions ){
        errorLog << "process(const VectorFloat &inputVector) - The size of the inputVector (" << inputVector.size() << ") does not match that of the filter (" << numInputDimensions << ")!" << std::endl;
        return false;
    }
    
    filter( inputVector );
    
    if( processedData.size() == numOutputDimensions ) return true;

    return false;
}

bool WeightedAverageFilter::reset(){
    if( initialized ) return init(filterSize,numInputDimensions);
    return false;
}

bool WeightedAverageFilter::saveModelToFile( std::fstream &file ) const{
    
    if( !file.is_open() ){
        errorLog << "saveModelToFile(fstream &file) - The file is not open!" << std::endl;
        return false;
    }
    
    file << "GRT_MOVING_AVERAGE_FILTER_FILE_V1.0" << std::endl;
    
    file << "NumInputDimensions: " << numInputDimensions << std::endl;
    file << "NumOutputDimensions: " << numOutputDimensions << std::endl;
    file << "FilterSize: " << filterSize << std::endl;
    
    return true;
}

bool WeightedAverageFilter::loadModelFromFile( std::fstream &file ){
    
    if( !file.is_open() ){
        errorLog << "loadModelFromFile(fstream &file) - The file is not open!" << std::endl;
        return false;
    }
    
    std::string word;
    
    //Load the header
    file >> word;
    
    if( word != "GRT_MOVING_AVERAGE_FILTER_FILE_V1.0" ){
        errorLog << "loadModelFromFile(fstream &file) - Invalid file format!" << std::endl;
        return false;     
    }
    
    //Load the number of input dimensions
    file >> word;
    if( word != "NumInputDimensions:" ){
        errorLog << "loadModelFromFile(fstream &file) - Failed to read NumInputDimensions header!" << std::endl;
        return false;     
    }
    file >> numInputDimensions;
    
    //Load the number of output dimensions
    file >> word;
    if( word != "NumOutputDimensions:" ){
        errorLog << "loadModelFromFile(fstream &file) - Failed to read NumOutputDimensions header!" << std::endl;
        return false;     
    }
    file >> numOutputDimensions;
    
    //Load the filter factor
    file >> word;
    if( word != "FilterSize:" ){
        errorLog << "loadModelFromFile(fstream &file) - Failed to read FilterSize header!" << std::endl;
        return false;     
    }
    file >> filterSize;
    
    //Init the filter module to ensure everything is initialized correctly
    return init(filterSize,numInputDimensions);  
}

bool WeightedAverageFilter::init(UINT filterSize,UINT numDimensions){
    
    //Cleanup the old memory
    initialized = false;
    inputSampleCounter = 0;
    
    if( filterSize == 0 ){
        errorLog << "init(UINT filterSize,UINT numDimensions) - Filter size can not be zero!" << std::endl;
        return false;
    }
    
    if( numDimensions == 0 ){
        errorLog << "init(UINT filterSize,UINT numDimensions) - The number of dimensions must be greater than zero!" << std::endl;
        return false;
    }
    
    //Resize the filter
    this->filterSize = filterSize;
    this->numInputDimensions = numDimensions;
    this->numOutputDimensions = numDimensions;
    processedData.clear();
    weights.clear();
    processedData.resize(numDimensions,0);
    weights.resize(filterSize);
    initialized = dataBuffer.resize( filterSize, VectorFloat(numInputDimensions,0) );
    
    const Float norm = 1.0 / filterSize;
    for(UINT i=0; i<filterSize; i++){
        weights[i] = (i+1)*norm;
    }

    if( !initialized ){
        errorLog << "init(UINT filterSize,UINT numDimensions) - Failed to resize dataBuffer!" << std::endl;
    }
    
    return initialized;
}

Float WeightedAverageFilter::filter(const Float x){
    
    //If the filter has not been initialised then return 0, otherwise filter x and return y
    if( !initialized ){
        errorLog << "filter(const Float x) - The filter has not been initialized!" << std::endl;
        return 0;
    }
    
    VectorFloat y = filter(VectorFloat(1,x));
    
    if( y.size() == 0 ) return 0;
    return y[0];
}
    
VectorFloat WeightedAverageFilter::filter(const VectorFloat &x){
    
    //If the filter has not been initialised then return 0, otherwise filter x and return y
    if( !initialized ){
        errorLog << "filter(const VectorFloat &x) - The filter has not been initialized!" << std::endl;
        return VectorFloat();
    }
    
    if( x.size() != numInputDimensions ){
        errorLog << "filter(const VectorFloat &x) - The size of the input vector (" << x.size() << ") does not match that of the number of dimensions of the filter (" << numInputDimensions << ")!" << std::endl;
        return VectorFloat();
    }
    
    if( ++inputSampleCounter > filterSize ) inputSampleCounter = filterSize;
    
    //Add the new value to the buffer
    dataBuffer.push_back( x );
    
    Float weightSum = 0;
    for(unsigned int j=0; j<numInputDimensions; j++){
        processedData[j] = 0;
        weightSum = 0;
        for(unsigned int i=0; i<inputSampleCounter; i++) {
            processedData[j] += dataBuffer[i][j] * weights[i];
            weightSum += weights[i];
        }
        if( weightSum != 0.0 ) processedData[j] /= weightSum;
    }
    
    return processedData;
}

GRT_END_NAMESPACE


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

#include "MovingAverageFilter.h"

GRT_BEGIN_NAMESPACE
    
//Register the MovingAverageFilter module with the PreProcessing base class
RegisterPreProcessingModule< MovingAverageFilter > MovingAverageFilter::registerModule("MovingAverageFilter");

MovingAverageFilter::MovingAverageFilter(UINT filterSize,UINT numDimensions){
    
    classType = "MovingAverageFilter";
    preProcessingType = classType;
    debugLog.setProceedingText("[DEBUG MovingAverageFilter]");
    errorLog.setProceedingText("[ERROR MovingAverageFilter]");
    warningLog.setProceedingText("[WARNING MovingAverageFilter]");
    init(filterSize,numDimensions);
}
    
MovingAverageFilter::MovingAverageFilter(const MovingAverageFilter &rhs){
    
    classType = "MovingAverageFilter";
    preProcessingType = classType;
    debugLog.setProceedingText("[DEBUG MovingAverageFilter]");
    errorLog.setProceedingText("[ERROR MovingAverageFilter]");
    warningLog.setProceedingText("[WARNING MovingAverageFilter]");
    
    //Zero this instance
    this->filterSize = 0;
    this->inputSampleCounter = 0;
    
	//Copy the settings from the rhs instance
	*this = rhs;
}
    
MovingAverageFilter::~MovingAverageFilter(){

}
    
MovingAverageFilter& MovingAverageFilter::operator=(const MovingAverageFilter &rhs){
    if(this!=&rhs){
        //Clear this instance
        this->filterSize = 0;
        this->inputSampleCounter = 0;
        this->dataBuffer.clear();
        
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
    
bool MovingAverageFilter::deepCopyFrom(const PreProcessing *preProcessing){
    
    if( preProcessing == NULL ) return false;
    
    if( this->getPreProcessingType() == preProcessing->getPreProcessingType() ){
        
        //Call the equals operator
        *this = *(MovingAverageFilter*)preProcessing;
        
		return true;
    }
    
    errorLog << "clone(const PreProcessing *preProcessing) -  PreProcessing Types Do Not Match!" << std::endl;
    
    return false;
}

    
bool MovingAverageFilter::process(const VectorFloat &inputVector){
    
    if( !initialized ){
        errorLog << "process(const VectorFloat &inputVector) - The filter has not been initialized!" << std::endl;
        return false;
    }

    if( inputVector.getSize() != numInputDimensions ){
        errorLog << "process(const VectorFloat &inputVector) - The size of the inputVector (" << inputVector.getSize() << ") does not match that of the filter (" << numInputDimensions << ")!" << std::endl;
        return false;
    }
    
    filter( inputVector );
    
    if( processedData.getSize() == numOutputDimensions ) return true;

    return false;
}

bool MovingAverageFilter::reset(){
    if( initialized ) return init(filterSize,numInputDimensions);
    return false;
}
    
bool MovingAverageFilter::saveModelToFile(std::string filename) const{
    
    if( !initialized ){
        errorLog << "saveModelToFile(string filename) - The MovingAverageFilter has not been initialized" << std::endl;
        return false;
    }
    
    std::fstream file; 
    file.open(filename.c_str(), std::ios::out);
    
    if( !saveModelToFile( file ) ){
        file.close();
        return false;
    }
    
    file.close();
    
    return true;
}

bool MovingAverageFilter::saveModelToFile(std::fstream &file) const{
    
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

bool MovingAverageFilter::loadModelFromFile(std::string filename){
    
    std::fstream file; 
    file.open(filename.c_str(), std::ios::in);
    
    if( !loadModelFromFile( file ) ){
        file.close();
        initialized = false;
        return false;
    }
    
    file.close();
    
    return true;
}

bool MovingAverageFilter::loadModelFromFile(std::fstream &file){
    
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

bool MovingAverageFilter::init(UINT filterSize,UINT numDimensions){
    
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
    processedData.resize(numDimensions,0);
    initialized = dataBuffer.resize( filterSize, VectorFloat(numInputDimensions,0) );
    
    if( !initialized ){
        errorLog << "init(UINT filterSize,UINT numDimensions) - Failed to resize dataBuffer!" << std::endl;
    }
    
    return initialized;
}

Float MovingAverageFilter::filter(const Float x){
    
    //If the filter has not been initialised then return 0, otherwise filter x and return y
    if( !initialized ){
        errorLog << "filter(const Float x) - The filter has not been initialized!" << std::endl;
        return 0;
    }
    
    VectorFloat y = filter(VectorFloat(1,x));
    
    if( y.size() == 0 ) return 0;
    return y[0];
}
    
VectorFloat MovingAverageFilter::filter(const VectorFloat &x){
    
    //If the filter has not been initialised then return 0, otherwise filter x and return y
    if( !initialized ){
        errorLog << "filter(const VectorFloat &x) - The filter has not been initialized!" << std::endl;
        return VectorFloat();
    }
    
    if( x.size() != numInputDimensions ){
        errorLog << "filter(const VectorFloat &x) - The size of the input vector (" << x.getSize() << ") does not match that of the number of dimensions of the filter (" << numInputDimensions << ")!" << std::endl;
        return VectorFloat();
    }
    
    if( ++inputSampleCounter > filterSize ) inputSampleCounter = filterSize;
    
    //Add the new value to the buffer
    dataBuffer.push_back( x );
    
    for(unsigned int j=0; j<numInputDimensions; j++){
        processedData[j] = 0;
        for(unsigned int i=0; i<inputSampleCounter; i++) {
            processedData[j] += dataBuffer[i][j];
        }
        processedData[j] /= Float(inputSampleCounter);
    }
    
    return processedData;
}

GRT_END_NAMESPACE

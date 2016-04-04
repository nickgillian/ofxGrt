/**
 @file
 @author  Nicholas Gillian <ngillian@media.mit.edu>
 @version 1.0
 
 @section LICENSE
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
 
 @section DESCRIPTION
 The DoubleMovingAverageFilter implements a low pass Float moving average filter.
 
 */

#include "DoubleMovingAverageFilter.h"

GRT_BEGIN_NAMESPACE
    
//Register the DoubleMovingAverageFilter module with the PreProcessing base class
RegisterPreProcessingModule< DoubleMovingAverageFilter > DoubleMovingAverageFilter::registerModule("DoubleMovingAverageFilter");

DoubleMovingAverageFilter::DoubleMovingAverageFilter(UINT filterSize,UINT numDimensions){
    classType = "DoubleMovingAverageFilter";
    preProcessingType = classType;
    debugLog.setProceedingText("[DEBUG DoubleMovingAverageFilter]");
    errorLog.setProceedingText("[ERROR DoubleMovingAverageFilter]");
    warningLog.setProceedingText("[WARNING DoubleMovingAverageFilter]");
    init(filterSize,numDimensions);
}
    
DoubleMovingAverageFilter::DoubleMovingAverageFilter(const DoubleMovingAverageFilter &rhs){
    
    this->filterSize = rhs.filterSize;
    this->filter1 = rhs.filter1;
    this->filter2 = rhs.filter2;
    classType = "DoubleMovingAverageFilter";
    preProcessingType = classType;
    debugLog.setProceedingText("[DEBUG DoubleMovingAverageFilter]");
    errorLog.setProceedingText("[ERROR DoubleMovingAverageFilter]");
    warningLog.setProceedingText("[WARNING DoubleMovingAverageFilter]");
    
    //Copy the base variables
    copyBaseVariables( (PreProcessing*)&rhs );
}
    
DoubleMovingAverageFilter::~DoubleMovingAverageFilter(){

}
    
DoubleMovingAverageFilter& DoubleMovingAverageFilter::operator=(const DoubleMovingAverageFilter &rhs){
    if(this!=&rhs){
        this->filterSize = rhs.filterSize;
        this->filter1 = rhs.filter1;
        this->filter2 = rhs.filter2;
        
        //Copy the base variables
        copyBaseVariables( (PreProcessing*)&rhs );
    }
    return *this;
}
    
bool DoubleMovingAverageFilter::deepCopyFrom(const PreProcessing *preProcessing){
    
    if( preProcessing == NULL ) return false;
    
    if( this->getPreProcessingType() == preProcessing->getPreProcessingType() ){
        
        DoubleMovingAverageFilter *ptr = (DoubleMovingAverageFilter*)preProcessing;
        
        //Clone the classLabelTimeoutFilter values 
        this->filterSize = ptr->filterSize;
        this->filter1 = ptr->filter1;
        this->filter2 = ptr->filter2;
        
        //Clone the preprocessing base variables
        return copyBaseVariables( preProcessing );
    }
    
    errorLog << "clone(const PreProcessing *preProcessing) -  PreProcessing Types Do Not Match!" << std::endl;
    
    return false;
}

    
bool DoubleMovingAverageFilter::process(const VectorFloat &inputVector){
    
    if( !initialized ){
        errorLog << "process(const VectorFloat &inputVector) - The filter has not been initialized!" << std::endl;
        return false;
    }

    if( inputVector.size() != numInputDimensions ){
        errorLog << "process(const VectorFloat &inputVector) - The size of the inputVector (" << inputVector.getSize() << ") does not match that of the filter (" << numInputDimensions << ")!" << std::endl;
        return false;
    }
    
    processedData = filter( inputVector );
    
    if( processedData.size() == numOutputDimensions ) return true;
    return false;
}

bool DoubleMovingAverageFilter::reset(){
    if( initialized ) return init(filterSize,numInputDimensions);
    return false;
}
    
bool DoubleMovingAverageFilter::saveModelToFile( std::string filename ) const{
    
    if( !initialized ){
        errorLog << "saveModelToFile(string filename) - The DoubleMovingAverageFilter has not been initialized" << std::endl;
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
    
bool DoubleMovingAverageFilter::saveModelToFile(std::fstream &file) const{
    
    if( !file.is_open() ){
        errorLog << "saveModelToFile(std::fstream &file) - The file is not open!" << std::endl;
        return false;
    }
    
    file << "GRT_DOUBLE_MOVING_AVERAGE_FILTER_FILE_V1.0" << std::endl;
    
    file << "NumInputDimensions: " << numInputDimensions << std::endl;
    file << "NumOutputDimensions: " << numOutputDimensions << std::endl;
    file << "FilterSize: " << filterSize << std::endl;
    
    return true;
}

bool DoubleMovingAverageFilter::loadModelFromFile( std::string filename ){
    
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

bool DoubleMovingAverageFilter::loadModelFromFile( std::fstream &file ){
    
    if( !file.is_open() ){
        errorLog << "loadModelFromFile(std::fstream &file) - The file is not open!" << std::endl;
        return false;
    }
    
    std::string word;
    
    //Load the header
    file >> word;
    
    if( word != "GRT_DOUBLE_MOVING_AVERAGE_FILTER_FILE_V1.0" ){
        errorLog << "loadModelFromFile(std::fstream &file) - Invalid file format!" << std::endl;
        return false;     
    }
    
    //Load the number of input dimensions
    file >> word;
    if( word != "NumInputDimensions:" ){
        errorLog << "loadModelFromFile(std::fstream &file) - Failed to read NumInputDimensions header!" << std::endl;
        return false;     
    }
    file >> numInputDimensions;
    
    //Load the number of output dimensions
    file >> word;
    if( word != "NumOutputDimensions:" ){
        errorLog << "loadModelFromFile(std::fstream &file) - Failed to read NumOutputDimensions header!" << std::endl;
        return false;     
    }
    file >> numOutputDimensions;
    
    //Load the filter size
    file >> word;
    if( word != "FilterSize:" ){
        errorLog << "loadModelFromFile(std::fstream &file) - Failed to read FilterSize header!" << std::endl;
        return false;     
    }
    file >> filterSize;
    
    //Init the filter module to ensure everything is initialized correctly
    return init(filterSize,numInputDimensions);  
}

bool DoubleMovingAverageFilter::init(UINT filterSize,UINT numDimensions){
    
    //Cleanup the old memory
    initialized = false;
    
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
    numInputDimensions = numDimensions;
    numOutputDimensions = numDimensions;
    processedData.clear();
    processedData.resize(numDimensions,0 );
    
    if( !filter1.init(filterSize, numDimensions) ){
        errorLog << "init(UINT filterSize,UINT numDimensions) - Failed to initialize filter 1!" << std::endl;
        return false;
    }
    
    if( !filter2.init(filterSize, numDimensions) ){
        errorLog << "init(UINT filterSize,UINT numDimensions) - Failed to initialize filter 1!" << std::endl;
        return false;
    }
    
    //Flag that the filter has been initialized
    initialized = true;
    
    return true;
}

Float DoubleMovingAverageFilter::filter(const Float x){
    
    //If the filter has not been initialised then return 0, otherwise filter x and return y
    if( !initialized ){
        errorLog << "filter(const Float x) - The filter has not been initialized!" << std::endl;
        return 0;
    }
    
    VectorFloat y = filter(VectorFloat(1,x));
    
    if( y.getSize() == 0 ) return 0;
    return y[0];
}
    
VectorFloat DoubleMovingAverageFilter::filter(const VectorFloat &x){
    
    //If the filter has not been initialised then return 0, otherwise filter x and return y
    if( !initialized ){
        errorLog << "filter(const VectorFloat &x) - The filter has not been initialized!" << std::endl;
        return VectorFloat();
    }
    
    if( x.getSize() != numInputDimensions ){
        errorLog << "filter(const VectorFloat &x) - The size of the input vector (" << x.getSize() << ") does not match that of the number of dimensions of the filter (" << numInputDimensions << ")!" << std::endl;
        return VectorFloat();
    }
    
    //Perform the first filter
    VectorFloat y = filter1.filter( x );
    
    if( y.size() == 0 ) return y;
    
    //Perform the second filter
    VectorFloat yy = filter2.filter( y );
    
    if( yy.size() == 0 ) return y;
    
    //Account for the filter lag
    const UINT N = y.getSize();
    for(UINT i=0; i<N; i++){
        yy[i] = y[i] + (y[i] - yy[i]); 
        processedData[i] = yy[i];
    }
    
    return yy;
}

GRT_END_NAMESPACE

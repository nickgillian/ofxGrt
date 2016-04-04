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

#include "HighPassFilter.h"

GRT_BEGIN_NAMESPACE
    
//Register the HighPassFilter module with the PreProcessing base class
RegisterPreProcessingModule< HighPassFilter > HighPassFilter::registerModule("HighPassFilter");
    
HighPassFilter::HighPassFilter(Float filterFactor,Float gain,UINT numDimensions,Float cutoffFrequency,Float delta){
    
    classType = "HighPassFilter";
    preProcessingType = classType;
    debugLog.setProceedingText("[DEBUG HighPassFilter]");
    errorLog.setProceedingText("[ERROR HighPassFilter]");
    warningLog.setProceedingText("[WARNING HighPassFilter]");
    init(filterFactor,gain,numDimensions);
    
    if( cutoffFrequency != -1 && delta != -1 ){
        setCutoffFrequency(cutoffFrequency, delta);
    }
}
    
HighPassFilter::HighPassFilter(const HighPassFilter &rhs){
    
    preProcessingType = classType;
    preProcessingType = "HighPassFilter";
    debugLog.setProceedingText("[DEBUG HighPassFilter]");
    errorLog.setProceedingText("[ERROR HighPassFilter]");
    warningLog.setProceedingText("[WARNING HighPassFilter]");
    
    this->filterFactor = rhs.filterFactor;
    this->gain = rhs.gain;
	this->xx = rhs.xx;
    this->yy = rhs.yy;
    copyBaseVariables( (PreProcessing*)&rhs );
}

HighPassFilter::~HighPassFilter(){

}
    
HighPassFilter& HighPassFilter::operator=(const HighPassFilter &rhs){
    if(this!=&rhs){
        this->filterFactor = rhs.filterFactor;
        this->gain = rhs.gain;
        this->xx = rhs.xx;
        this->yy = rhs.yy;
        copyBaseVariables( (PreProcessing*)&rhs );
    }
    return *this;
}
    
bool HighPassFilter::deepCopyFrom(const PreProcessing *preProcessing){
    
    if( preProcessing == NULL ) return false;
    
    if( this->getPreProcessingType() == preProcessing->getPreProcessingType() ){
        
        HighPassFilter *ptr = (HighPassFilter*)preProcessing;
        
        //Clone the HighPassFilter values 
        this->filterFactor = ptr->filterFactor;
        this->gain = ptr->gain;
        this->xx = ptr->xx;
        this->yy = ptr->yy;
        
        //Clone the base class variables
        return copyBaseVariables( preProcessing );
    }
    
    errorLog << "clone(const PreProcessing *preProcessing) -  PreProcessing Types Do Not Match!" << std::endl;
    
    return false;
}
    
bool HighPassFilter::process(const VectorFloat &inputVector){
    
    if( !initialized ){
        errorLog << "process(const VectorFloat &inputVector) - Not initialized!" << std::endl;
        return false;
    }
    
    if( inputVector.size() != numInputDimensions ){
        errorLog << "process(const VectorFloat &inputVector) - The size of the inputVector (" << inputVector.size() << ") does not match that of the filter (" << numInputDimensions << ")!" << std::endl;
        return false;
    }
    
    processedData = filter( inputVector );
    
    if( processedData.size() == numOutputDimensions ) return true;
    return false;

}

bool HighPassFilter::reset(){
    if( initialized ) return init(filterFactor,gain,numInputDimensions);
    return false;
}
    
bool HighPassFilter::saveModelToFile( std::string filename ) const{
    
    if( !initialized ){
        errorLog << "saveModelToFile(string filename) - The HighPassFilter has not been initialized" << std::endl;
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
    
bool HighPassFilter::saveModelToFile( std::fstream &file ) const{
    
    if( !file.is_open() ){
        errorLog << "saveModelToFile(fstream &file) - The file is not open!" << std::endl;
        return false;
    }
    
    file << "GRT_HIGH_PASS_FILTER_FILE_V1.0" << std::endl;
    
    file << "NumInputDimensions: " << numInputDimensions << std::endl;
    file << "NumOutputDimensions: " << numOutputDimensions << std::endl;
    file << "FilterFactor: " << filterFactor << std::endl;
    file << "Gain: " << gain << std::endl;
    
    return true;
}
    
bool HighPassFilter::loadModelFromFile( std::string filename ){
    
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
    
bool HighPassFilter::loadModelFromFile( std::fstream &file ){
    
    if( !file.is_open() ){
        errorLog << "loadModelFromFile(fstream &file) - The file is not open!" << std::endl;
        return false;
    }
    
    std::string word;
    
    //Load the header
    file >> word;
    
    if( word != "GRT_HIGH_PASS_FILTER_FILE_V1.0" ){
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
    if( word != "FilterFactor:" ){
        errorLog << "loadModelFromFile(fstream &file) - Failed to read FilterFactor header!" << std::endl;
        return false;     
    }
    file >> filterFactor;
    
    //Load the number of output dimensions
    file >> word;
    if( word != "Gain:" ){
        errorLog << "loadModelFromFile(fstream &file) - Failed to read Gain header!" << std::endl;
        return false;     
    }
    file >> gain;
    
    //Init the filter module to ensure everything is initialized correctly
    return init(filterFactor,gain,numInputDimensions);  
}
    
bool HighPassFilter::init(Float filterFactor,Float gain,UINT numDimensions){
    
    initialized = false;
    
    if( numDimensions == 0 ){
        errorLog << "init(Float filterFactor,Float gain,UINT numDimensions) - NumDimensions must be greater than 0!" << std::endl;
        return false;
    }
    
    if( filterFactor <= 0 ){
        errorLog << "init(Float filterFactor,Float gain,UINT numDimensions) - FilterFactor must be greater than 0!" << std::endl;
        return false;
    }
    
    if( gain <= 0 ){
        errorLog << "init(Float filterFactor,Float gain,UINT numDimensions) - Gain must be greater than 0!" << std::endl;
        return false;
    }
    
    this->filterFactor = filterFactor;
    this->gain = gain;
    this->numInputDimensions = numDimensions;
    this->numOutputDimensions = numDimensions;
    xx.clear();
    xx.resize(numDimensions,0);
    yy.clear();
    yy.resize(numDimensions,0);
    processedData.clear();
    processedData.resize(numDimensions,0);
    initialized = true;
    
    return true;
}

Float HighPassFilter::filter(const Float x){
    
    //If the filter has not been initialised then return 0, otherwise filter x and return y
    if( !initialized ){
        errorLog << "filter(const Float x) - The filter has not been initialized!" << std::endl;
        return 0;
    }
    
    VectorFloat y = filter(VectorFloat(1,x));
    
    if( y.size() == 0 ) return 0;
    return y[0];

}
    
VectorFloat HighPassFilter::filter(const VectorFloat &x){
    
    if( !initialized ){
        errorLog << "filter(const VectorFloat &x) - Not Initialized!" << std::endl;
        return VectorFloat();
    }
    
    if( x.size() != numInputDimensions ){
        errorLog << "filter(const VectorFloat &x) - The Number Of Input Dimensions (" << numInputDimensions << ") does not match the size of the input vector (" << x.size() << ")!" << std::endl;
        return VectorFloat();
    }
    
    for(UINT n=0; n<numInputDimensions; n++){
        //Compute the new output
        yy[n] = filterFactor * (yy[n] + x[n] - xx[n]) * gain;
        
        //Store the current input
        xx[n] = x[n];
        
        //Store the current output in processed data so it can be accessed by the base class
        processedData[n] = processedData[n];
    }
    return processedData;
}
    
bool HighPassFilter::setGain(Float gain){
    if( gain > 0 ){
        this->gain = gain;
        return true;
    }
    errorLog << "setGain(Float gain) - Gain value must be greater than 0!" << std::endl;
    return false;
}

bool HighPassFilter::setFilterFactor(Float filterFactor){
    if( filterFactor > 0 ){
        this->filterFactor = filterFactor;
        return true;
    }
    errorLog << "setFilterFactor(Float filterFactor) - FilterFactor value must be greater than 0!" << std::endl;
    return false;
}
    
bool HighPassFilter::setCutoffFrequency(Float cutoffFrequency,Float delta){
    if( cutoffFrequency > 0 && delta > 0 ){
        Float RC = (1.0/TWO_PI) / cutoffFrequency;
        filterFactor = RC / (RC+delta);
        return true;
    }
    return false;
}

GRT_END_NAMESPACE

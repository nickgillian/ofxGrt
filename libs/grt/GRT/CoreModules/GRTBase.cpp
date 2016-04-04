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

#include "GRTBase.h"

GRT_BEGIN_NAMESPACE
    
GRTBase::GRTBase(void):classType(""),debugLog("[DEBUG]"),errorLog("[ERROR]"),trainingLog("[TRAINING]"),testingLog("[TESTING]"),warningLog("[WARNING]"){

}
    
GRTBase::~GRTBase(void){
}
    
bool GRTBase::copyGRTBaseVariables(const GRTBase *base){
    
    if( base == NULL ){
        errorLog << "copyBaseVariables(const GRTBase *base) - base pointer is NULL!" << std::endl;
        return false;
    }

    this->classType = base->classType;
    this->debugLog = base->debugLog;
    this->errorLog = base->errorLog;
    this->infoLog = base->infoLog;
    this->trainingLog = base->trainingLog;
    this->testingLog = base->testingLog;
    this->warningLog = base->warningLog;
    
    return true;
}
    
std::string GRTBase::getLastWarningMessage() const {
    return warningLog.getLastMessage();
}

std::string GRTBase::getLastErrorMessage() const {
    return errorLog.getLastMessage();
}
    
std::string GRTBase::getLastInfoMessage() const {
    return infoLog.getLastMessage();
}
    
std::string GRTBase::getGRTVersion(bool returnRevision){
    std::string version = GRT_VERSION;
    if( returnRevision ) version += " revision: " + getGRTRevison();
    return version;
}

std::string GRTBase::getGRTRevison(){
    return GRT_REVISION;
}
    
std::string GRTBase::getClassType() const{
    return classType;
}
    
GRTBase* GRTBase::getGRTBasePointer(){
    return this;
}

const GRTBase* GRTBase::getGRTBasePointer() const{
    return this;
}

bool GRTBase::setInfoLoggingEnabled(const bool loggingEnabled){
    infoLog.setEnableInstanceLogging( loggingEnabled );
    return true;
}
    
bool GRTBase::setWarningLoggingEnabled(const bool loggingEnabled){
    warningLog.setEnableInstanceLogging( loggingEnabled );
    return true;
}
    
bool GRTBase::setErrorLoggingEnabled(const bool loggingEnabled){
    errorLog.setEnableInstanceLogging( loggingEnabled );
    return true;
}

GRT_END_NAMESPACE



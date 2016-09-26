//
//  ofxGrtSettings.h
//
//  Created by luca lolli on 11/08/2016.
// Updated: nickgillian: 26/09/2016
//

#pragma once

#include "ofMain.h"

class ofxGrtSettings
{
    
    
public:
    struct variables
    {
        variables()
        {
            fontSmall.load(fontFile,8);
            fontNormal.load(fontFile,10);
            fontLarge.load(fontFile,14);
        }
        
        static const int info_margin = 15;
        
        static const std::string fontFile;
        
        static ofTrueTypeFont fontSmall;
        static ofTrueTypeFont fontNormal;
        static ofTrueTypeFont fontLarge;
        static const ofColor activeTextColor;
        static const ofColor gridColor;
        static const ofColor axisColor;
        static const ofColor backgroundColor;
        static const ofColor disabledTextColor;
        static const int titleTextSpacer = 5;
        
        static const int axisTicksSize = 5;
        
    };
    
    ofxGrtSettings()
    {
        mVariables = shared_ptr<variables>( new variables());
    }
    
    static ofxGrtSettings& GetInstance()
    {
        static ofxGrtSettings config;
        return config;
    }
    
    std::shared_ptr<variables> get()
    {
        return mVariables;
    }
    
private:
    
    static std::shared_ptr<variables> mVariables;
    
    ofxGrtSettings(const ofxGrtSettings&);
    ofxGrtSettings& operator=(const ofxGrtSettings&);
    
};


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
        
        static int info_margin;
        
        static std::string fontFile;
        
        static ofTrueTypeFont fontSmall;
        static ofTrueTypeFont fontNormal;
        static ofTrueTypeFont fontLarge;
        static ofColor activeTextColor;
        static ofColor gridColor;
        static ofColor axisColor;
        static ofColor backgroundColor;
        static ofColor disabledTextColor;
        static int titleTextSpacer;
        static int axisTicksSize;
        
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


//
//  settings.h
//
//  Created by luca lolli on 11/08/2016.
//
//



#ifndef settings_h
#define settings_h

#include "ofMain.h"

class settings
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
        static const std::string rangePlotterTitle;
        static const ofColor activeTextColor;
        static const ofColor gridColor;
        static const ofColor axisColor;
        static const ofColor backgroundColor;
        static const ofColor disabledTextColor;
        static const int titleTextSpacer = 5;
        
        static const int axisTicksSize = 5;
        
    };
    
    settings()
    {
        mVariables = shared_ptr<variables>( new variables());
    }
    
    static settings& GetInstance()
    {
        static settings config;
        return config;
    }
    
    std::shared_ptr<variables> get()
    {
        return mVariables;
    }
    
private:
    
    static std::shared_ptr<variables> mVariables;
    
    settings(const settings&);
    settings& operator=(const settings&);
    
};

#endif /* settings_h */

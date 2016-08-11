//
//  settings.h
//
//  Created by luca lolli on 11/08/2016.
//
//
#include "settings.h"

//We need to declare our static non-const variable after being definid in the header
std::shared_ptr<settings::variables> settings::mVariables;

ofTrueTypeFont settings::variables::fontSmall;
ofTrueTypeFont settings::variables::fontNormal;
ofTrueTypeFont settings::variables::fontLarge;
const std::string settings::variables::fontFile = "Roboto-Regular.ttf";
const std::string settings::variables::rangePlotterTitle = "Range Doppler";
const ofColor settings::variables::activeTextColor = {0,60,150,255};
const ofColor settings::variables::gridColor = {198,219,244,255};
const ofColor settings::variables::axisColor = {128,153,184,255};
const ofColor settings::variables::backgroundColor = {255,255,255,255};

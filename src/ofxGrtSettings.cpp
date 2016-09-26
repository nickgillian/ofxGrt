//
//  ofxGrtSettings.h
//
//  Created by luca lolli on 11/08/2016.
// Updated: nickgillian: 26/09/2016
//
#include "ofxGrtSettings.h"

//We need to declare our static non-const variable after being definid in the header
std::shared_ptr<ofxGrtSettings::variables> ofxGrtSettings::mVariables;

ofTrueTypeFont ofxGrtSettings::variables::fontSmall;
ofTrueTypeFont ofxGrtSettings::variables::fontNormal;
ofTrueTypeFont ofxGrtSettings::variables::fontLarge;
const std::string ofxGrtSettings::variables::fontFile = "Roboto-Regular.ttf";
const ofColor ofxGrtSettings::variables::activeTextColor = {0,60,150,255};
const ofColor ofxGrtSettings::variables::gridColor = {198,219,244,255};
const ofColor ofxGrtSettings::variables::axisColor = {128,153,184,255};
const ofColor ofxGrtSettings::variables::backgroundColor = {255,255,255,255};

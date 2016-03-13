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
#pragma once

#include <GRT/GRT.h>

#include "ofMain.h"

using namespace GRT;

class ofxGrtMatrixPlot {
public:
    ofxGrtMatrixPlot();
    bool resize( const unsigned int rows, const unsigned int cols );
    void update( const Matrix<double> &data );
    void update( const Matrix<float> &data );
    void update( const MatrixFloat &data, float minValue, float maxValue );
    void update( float *data, const unsigned int rows, const unsigned int cols );
    bool draw(float x, float y) const;
    bool draw(float x, float y, float w, float h) const;
    bool draw(float x, float y, float w, float h,ofShader &shader) const;
    bool setFont( const ofTrueTypeFont &font ){ this->font = &font; return this->font->isLoaded(); }
    bool setTitle( const std::string &plotTitle ){ this->plotTitle = plotTitle; return true; }

    unsigned int getRows() const;
    unsigned int getCols() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
protected:
    unsigned int rows;
    unsigned int cols;

    std::string plotTitle;
    ofColor textColor;
    vector<float> pixelData;
    ofFloatPixels pixels;
    ofTexture texture;
    const ofTrueTypeFont *font;
};



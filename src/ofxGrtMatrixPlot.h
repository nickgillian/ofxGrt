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
#include "ofxGrtSettings.h"

using namespace GRT;

/**
 @brief This class can be used for drawing matrix data. The matrix data should be normalized so it is in the range of [0. 1.].
*/
class ofxGrtMatrixPlot {
public:
    /**
    Default constructor, sets the size, name, and font of the matrix to be drawn. These can all be changed later using the various setter functions.
    @param rows: the number of rows in the matrix
    @param cols: the number of columns in the matrix
    @param title: a string that is drawn above the matrix to identify it
    @param font: the font that should be used to render text for the matrix
    @param textColor: the color used to draw any text
    */
    ofxGrtMatrixPlot( const std::string &title = "", const ofTrueTypeFont *font = NULL, const ofColor &textColor = ofColor(255,255,255) );

    ofxGrtMatrixPlot( const std::string &plotTitle, const std::string &xAxisInfo, const std::string &yAxisInfo);
    /**
    Resizes the internal buffers to prepare the instance for drawing
    @param rows: the number of rows in the matrix
    @param cols: the number of columns in the matrix
    @return returns true if the matrix was resized correctly, false otherwise
    */
    bool resize( const unsigned int rows, const unsigned int cols );

    /**
    Updates the internal data to prepare it for drawing, the data in the input matrix should be scaled in the range [0. 1.].
    If the size of the input matrix does not match that of the internal buffers then the internal buffers will be automatically rescaled to match.
    @param data: the input data that should be drawn, it should already be scaled in the range [0. 1.]
    @return returns true if the instance was updated successfully, false otherwise
    */
    bool update( const Matrix<double> &data );

    /**
    Updates the internal data to prepare it for drawing, the data in the input matrix should be scaled in the range [0. 1.].
    If the size of the input matrix does not match that of the internal buffers then the internal buffers will be automatically rescaled to match.
    @param data: the input data that should be drawn, it should already be scaled in the range [0. 1.]
    @return returns true if the instance was updated successfully, false otherwise
    */
    bool update( const Matrix<float> &data );

    /**
    Updates the internal data to prepare it for drawing, if the min and max values are not 0.0 and 1.0 respectively then the data will be scaled to the range of [0. 1.0].
    If the size of the input matrix does not match that of the internal buffers then the internal buffers will be automatically rescaled to match.
    @param data: the input data that should be drawn, it should already be scaled in the range [0. 1.]
    @param minValue: the minimum value in the input matrix
    @param maxValue: the maximum value in the input matrix
    @return returns true if the instance was updated successfully, false otherwise
    */
    bool update( const MatrixFloat &data, const float minValue, const float maxValue );

    /**
    Updates the internal data to prepare it for drawing, the data in the input matrix should be scaled in the range [0. 1.].
    If the size of the rows and cols does not match that of the internal buffers then the internal buffers will be automatically rescaled to match.
    @param data: a pointer to the data that should be drawn, it should already be scaled in the range [0. 1.]
    @param rows: the number of rows in the input data
    @param cols: the number of columns in the input data
    @return returns true if the instance was updated successfully, false otherwise
    */
    bool update( float *data, const unsigned int rows, const unsigned int cols );

    /**
    Draws the current data in the internal buffer at the location [x,y] on the screen. The data will be draw with a width and height matching the size of the matrix.
    @param x: the x location to draw the matrix
    @param y: the y location to draw the matrix
    @return returns true if the instance was drawn successfully, false otherwise
    */
    bool draw(const float x, const float y) const;

    /**
    Draws the current data in the internal buffer at the location [x,y] on the screen using the set width and height.
    @param x: the x location to draw the matrix
    @param y: the y location to draw the matrix
    @param w: the width of the matrix to draw
    @param h: the height of the matrix to draw
    @return returns true if the instance was drawn successfully, false otherwise
    */
    bool draw(const float x, const float y, const float w, const float h) const;

    /**
    Draws the current data in the internal buffer at the location [x,y] on the screen using the set width and height using the input shader.
    @param x: the x location to draw the matrix
    @param y: the y location to draw the matrix
    @param w: the width of the matrix to draw
    @param h: the height of the matrix to draw
    @param shader: the shader to use to draw the data
    @return returns true if the instance was drawn successfully, false otherwise
    */
    bool draw(const float x, const float y, const float w, const float h,const ofShader &shader) const;

    /**
    Sets the font used to draw information about the matrix, the matrix instance will use a pointer to this font for drawing so it must exist for the lifetime of the matrix instance.
    @param font: a pointer to the font that will be used for drawing information on this instance
    @param textColor: sets the color used when drawing any text
    @return returns true if the parameter was updated successfully, false otherwise
    */
    bool setFont( const ofTrueTypeFont *font, const ofColor &textColor = ofColor(255,255,255) );

    /**
    Sets the font used to draw information about the matrix, the matrix instance will use a pointer to this font for drawing so it must exist for the lifetime of the matrix instance.
    @param font: a pointer to the font that will be used for drawing information on this instance
    @param textColor: sets the color used when drawing any text
    @return returns true if the parameter was updated successfully, false otherwise
    */
    GRT_DEPRECATED_MSG( "use setFont(const ofTrueTypeFont *font, const ofColor &textColor = ofColor(255,255,255)) instead.", bool setFont( const ofTrueTypeFont &font, const ofColor &textColor = ofColor(255,255,255) ) );

    /**
    Sets the title of this matrix, which will be draw in the top center of the matrix
    @param title: the title text that should be drawn, an empty string indicates no text should be drawn
    @return returns true if the parameter was updated successfully, false otherwise
    */
    bool setTitle( const std::string &plotTitle );

    /**
    @return returns the number of rows in the matrix
    */
    unsigned int getRows() const;

    /**
    @return returns the number of columns in the matrix
    */
    unsigned int getCols() const;

    /**
    @return returns the width of the matrix (this is the number of columns)
    */
    unsigned int getWidth() const;

    /**
    @return returns the height of the matrix (this is the number of rows)
    */
    unsigned int getHeight() const;
protected:
    unsigned int rows;
    unsigned int cols;

    std::string plotTitle;
    std::string xAxisInfo;
    std::string yAxisInfo;
    ofColor textColor;
    vector<float> pixelData;
    ofFloatPixels pixels;
    ofTexture texture;
    const ofTrueTypeFont *font;
    
    std::shared_ptr<ofxGrtSettings::variables> config;
};



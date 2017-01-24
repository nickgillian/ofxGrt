
#include "ofxGrtMatrixPlot.h"

using namespace GRT;

ofxGrtMatrixPlot::ofxGrtMatrixPlot( const std::string &title, const ofTrueTypeFont *font, const ofColor &textColor ){
    plotTitle = "";
    this->font = NULL;
    this->rows = 0;
    this->cols = 0;
    this->textColor = textColor;
    config = ofxGrtSettings::GetInstance().get();
    if( font == NULL ) this->font = &config->fontNormal;
    if( title != "" ) setTitle( title );
    if( font ) setFont( font, textColor );
}

ofxGrtMatrixPlot::ofxGrtMatrixPlot(const std::string &plotTitle, const std::string &xAxisInfo, const std::string &yAxisInfo){
    this->plotTitle = plotTitle;
    this->xAxisInfo = xAxisInfo;
    this->yAxisInfo = yAxisInfo;
    config = ofxGrtSettings::GetInstance().get();
    textColor = config->activeTextColor;
    font = &config->fontNormal;
    rows = cols = 0;
}

bool ofxGrtMatrixPlot::resize( const unsigned int rows, const unsigned int cols ){

    const size_t size = rows*cols;
    this->rows = rows;
    this->cols = cols;
    pixelData.resize( size );

    for(unsigned int i=0; i<size; i++){
        pixelData[ i++ ] = 0.0;
    }

    const unsigned int width = cols;
    const unsigned int height = rows;
    pixels.setFromExternalPixels(&pixelData[0],width,height,OF_PIXELS_GRAY);

    if(!texture.isAllocated()){
        texture.allocate( pixels, false );
        texture.setRGToRGBASwizzles(true);
    }
    texture.loadData( pixels );
    texture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );

    return true;
}

bool ofxGrtMatrixPlot::update( const Matrix<double> &data ){

    const unsigned int rows = data.getNumRows();
    const unsigned int cols = data.getNumCols();
    const size_t size = rows*cols;

    if( this->rows != rows || this->cols != cols ){
        this->rows = rows;
        this->cols = cols;
        pixelData.resize( size );
    }

    unsigned int index = 0;
    for(unsigned int i=0; i<rows; i++){
        for(unsigned int j=0; j<cols; j++){
            pixelData[ index++ ] = data[i][j];
        }
    }
    float *pixelPointer = &pixelData[0];

    return update( pixelPointer, rows, cols );
}

bool ofxGrtMatrixPlot::update( const Matrix<float> &data ){

    const unsigned int rows = data.getNumRows();
    const unsigned int cols = data.getNumCols();
    const size_t size = rows*cols;

    if( this->rows != rows || this->cols != cols ){
        this->rows = rows;
        this->cols = cols;
        pixelData.resize( size );
    }

    unsigned int index = 0;
    for(unsigned int i=0; i<rows; i++){
        for(unsigned int j=0; j<cols; j++){
            pixelData[ index++ ] = data[i][j];
        }
    }
    float *pixelPointer = &pixelData[0];

    return update( pixelPointer, rows, cols );
}

bool ofxGrtMatrixPlot::update( const MatrixFloat &data, const float minValue, const float maxValue ){

    const unsigned int rows = data.getNumRows();
    const unsigned int cols = data.getNumCols();
    const size_t size = rows*cols;

    if( this->rows != rows || this->cols != cols ){
        this->rows = rows;
        this->cols = cols;
        pixelData.resize( size );
    }

    unsigned int index = 0;
    for(unsigned int i=0; i<rows; i++){
        for(unsigned int j=0; j<cols; j++){
            pixelData[ index++ ] = Util::scale(data[i][j],minValue,maxValue,0.0,1.0);
        }
    }
    float *pixelPointer = &pixelData[0];

    return update( pixelPointer, rows, cols );
}

bool ofxGrtMatrixPlot::update( float *data, const unsigned int rows, const unsigned int cols ){

    const unsigned int width = cols;
    const unsigned int height = rows;
    pixels.setFromExternalPixels(data,width,height,OF_PIXELS_GRAY);

    if(!texture.isAllocated()){
        texture.allocate( pixels, false );
        texture.setRGToRGBASwizzles(true);
    }
    texture.loadData( pixels );
    texture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );

    return true;
}

bool ofxGrtMatrixPlot::draw(const float x, const float y) const{
    if( pixels.size() == 0 ) return false;
    return draw(x, y, texture.getWidth(), texture.getHeight());
}

bool ofxGrtMatrixPlot::draw(const float x, const float y, const float w, const float h) const{

    if( pixels.size() == 0 ) return false;

    //Draw the texture
    texture.draw(x,y,w,h);

    //Draw the text
    if( plotTitle != "" ){

        ofSetColor(textColor);
        float textX = x + w*0.5;
        float textY = y-config->titleTextSpacer;//tempY + 5 + (font->getLineHeight()*0.5);
        ofRectangle bounds;

        if( font ){
            if( !font->isLoaded() ) return false;

            textY = y + (font->getLineHeight()*0.5);
            bounds = font->getStringBoundingBox( plotTitle, 0, 0 );
            font->drawString( plotTitle, textX - bounds.width*0.5 , textY + bounds.height );

            ofPushMatrix();
            {
                ofRotateZ(-90.0f);
                font->drawString(yAxisInfo, -(textY+h)+config->info_margin-config->titleTextSpacer, textX-config->titleTextSpacer);
            }
            ofPopMatrix();

            font->drawString( xAxisInfo, textX, textY+h+font->getLineHeight()-config->info_margin+config->titleTextSpacer );

        }else{
            ofDrawBitmapString( plotTitle, textX, textY );
        }
    }

    return true;
}

bool ofxGrtMatrixPlot::draw(const float x, const float y, const float w, const float h,const ofShader &shader) const{

    if( pixels.size() == 0 ) return false;

    //Set the shader and draw the texture
    shader.begin();
    texture.draw(x,y,w,h);
    shader.end();

    //Draw the text
    if( plotTitle != "" ){

        ofSetColor(textColor);
        float textX = x + w*0.5;
        float textY = y-config->titleTextSpacer;//tempY + 5 + (font->getLineHeight()*0.5);
        ofRectangle bounds;

        if( font ){
            if( !font->isLoaded() ) return false;

            textY = y + (font->getLineHeight()*0.5);
            bounds = font->getStringBoundingBox( plotTitle, 0, 0 );
            font->drawString( plotTitle, textX - bounds.width*0.5 , textY + bounds.height );

            ofPushMatrix();
            {
                ofRotateZ(-90.0f);
                font->drawString(yAxisInfo, -(textY+h)+config->info_margin-config->titleTextSpacer, textX-config->titleTextSpacer);
            }
            ofPopMatrix();

            font->drawString( xAxisInfo, textX, textY+h+font->getLineHeight()-config->info_margin+config->titleTextSpacer );

        }else{
            ofDrawBitmapString( plotTitle, textX, textY );
        }
    }

    return true;
}

unsigned int ofxGrtMatrixPlot::getRows() const{
    return this->rows;
}

unsigned int ofxGrtMatrixPlot::getCols() const{
    return this->cols;
}

unsigned int ofxGrtMatrixPlot::getWidth() const{
    return this->cols;
}

unsigned int ofxGrtMatrixPlot::getHeight() const{
    return this->rows;
}

bool ofxGrtMatrixPlot::setFont( const ofTrueTypeFont *font, const ofColor &textColor ){
    this->font = font;
    this->textColor = textColor;
    if( this->font != NULL ) return this->font->isLoaded();
    return true;
}

bool ofxGrtMatrixPlot::setFont( const ofTrueTypeFont &font, const ofColor &textColor ){
    return setFont( &font, textColor );
}

bool ofxGrtMatrixPlot::setTitle( const std::string &plotTitle ){
    this->plotTitle = plotTitle;
    return true;
}



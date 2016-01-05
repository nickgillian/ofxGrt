
#include "ofxGrtMatrixPlot.h"

GRT_BEGIN_NAMESPACE

ofxGrtMatrixPlot::ofxGrtMatrixPlot(){
    plotTitle = "";
    font = NULL;
    rows = cols = 0;
    textColor[0] = 255;
    textColor[1] = 255;
    textColor[2] = 255;
}

void ofxGrtMatrixPlot::update( const Matrix<double> &data ){
 
    const unsigned int rows = data.getNumRows(); 
    const unsigned int cols = data.getNumCols();
    const size_t size = rows*cols;

    if( this->rows != rows || this->cols != cols ){
        this->rows = rows;
        this->cols = cols;
        pixelData.resize( size );
    }
    
    unsigned int index = 0;
    for(unsigned int j=0; j<cols; j++){
        for(unsigned int i=0; i<rows; i++){
            pixelData[ index++ ] = data[i][j];
        }
    }
    float *pixelPointer = &pixelData[0];

    update( pixelPointer, rows, cols );
}

void ofxGrtMatrixPlot::update( const Matrix<float> &data ){
 
    const unsigned int rows = data.getNumRows(); 
    const unsigned int cols = data.getNumCols();
    const size_t size = rows*cols;

    if( this->rows != rows || this->cols != cols ){
        this->rows = rows;
        this->cols = cols;
        pixelData.resize( size );
    }
    
    unsigned int index = 0;
    for(unsigned int j=0; j<cols; j++){
        for(unsigned int i=0; i<rows; i++){
            pixelData[ index++ ] = data[i][j];
        }
    }
    float *pixelPointer = &pixelData[0];

    update( pixelPointer, rows, cols );
}

void ofxGrtMatrixPlot::update( const MatrixFloat &data ){
 
    const unsigned int rows = data.getNumRows(); 
    const unsigned int cols = data.getNumCols();
    const size_t size = rows*cols;

    if( this->rows != rows || this->cols != cols ){
        this->rows = rows;
        this->cols = cols;
        pixelData.resize( size );
    }
    
    unsigned int index = 0;
    for(unsigned int j=0; j<cols; j++){
        for(unsigned int i=0; i<rows; i++){
            pixelData[ index++ ] = data[i][j];
        }
    }
    float *pixelPointer = &pixelData[0];

    update( pixelPointer, rows, cols );
}

void ofxGrtMatrixPlot::update( float *data, const unsigned int rows, const unsigned int cols ){
    
    pixels.setFromExternalPixels(data,rows,cols,OF_PIXELS_GRAY);

    if(!texture.isAllocated()){
        texture.allocate( pixels, false );
        texture.setRGToRGBASwizzles(true);
    }
    texture.loadData( pixels );
    texture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );

}

bool ofxGrtMatrixPlot::draw(float x, float y) const{
    if( pixels.size() == 0 ) return false;
    return draw(x, y, texture.getWidth(), texture.getHeight());
}

bool ofxGrtMatrixPlot::draw(float x, float y, float w, float h) const{

    if( pixels.size() == 0 ) return false;
	auto & tex = texture;
	auto ratio = w/h;
	auto texRatio = tex.getWidth()/tex.getHeight();
	if(ratio > texRatio){
		auto drawW = h*texRatio;
		auto drawX = x+(w-drawW)/2;
		tex.draw(drawX,y,drawW,h);
	}else{
		auto drawH = w/texRatio;
		auto drawY = y+(h-drawH)/2;
		tex.draw(x,drawY,w,drawH);
	}

    //Only draw the text if the font has been loaded
    if( font && plotTitle != "" ){

        if( !font->isLoaded() ) return false;
        
        ofRectangle bounds = font->getStringBoundingBox(plotTitle, 0, 0);
        int textX = 10;
        int textY = bounds.height + 5;
        int textSpacer = bounds.height + 5;

        ofSetColor(textColor[0],textColor[1],textColor[2]);
        font->drawString( plotTitle, textX, textY );
        textY += textSpacer;
    }

    return true;
}

bool ofxGrtMatrixPlot::draw(float x, float y, float w, float h,ofShader &shader) const{

    if( pixels.size() == 0 ) return false;
    auto & tex = texture;
    auto ratio = w/h;
    auto texRatio = tex.getWidth()/tex.getHeight();
    shader.begin();
    if(ratio > texRatio){
        auto drawW = h*texRatio;
        auto drawX = x+(w-drawW)/2;
        tex.draw(drawX,y,drawW,h);
    }else{
        auto drawH = w/texRatio;
        auto drawY = y+(h-drawH)/2;
        tex.draw(x,drawY,w,drawH);
    }
    shader.end();

    //Only draw the text if the font has been loaded
    if( font && plotTitle != "" ){

        if( !font->isLoaded() ) return false;
        
        ofRectangle bounds = font->getStringBoundingBox(plotTitle, 0, 0);
        int textX = 10;
        int textY = bounds.height + 5;
        int textSpacer = bounds.height + 5;

        ofSetColor(textColor[0],textColor[1],textColor[2]);
        font->drawString( plotTitle, textX, textY );
        textY += textSpacer;
    }

    return true;
}

unsigned int ofxGrtMatrixPlot::getRows() const{
    return this->rows;
}

unsigned int ofxGrtMatrixPlot::getCols() const{
    return this->cols;
}

GRT_END_NAMESPACE


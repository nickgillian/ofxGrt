/*
 See the README file for more info.
 */

#include "ofApp.h"
#define TEXTURE_RESOLUTION 1024

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    largeFont.load("verdana.ttf", 12, true, true);
    largeFont.setLineHeight(14.0f);
    smallFont.load("verdana.ttf", 10, true, true);
    smallFont.setLineHeight(12.0f);
    
    //Initialize the training and info variables
    infoText = "";
    targetVector.resize(3);
    targetVector[0] = 1;
    targetVector[1] = 0;
    targetVector[2] = 0;
    record = false;
    drawInfo = true;
    
    //The input to the training data will be the [x y] from the mouse, so we set the number of dimensions to 2
    trainingData.setInputAndTargetDimensions( 2, 3 );
    
    //set the default classifier
    setRegressifier( LINEAR_REGRESSION );
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //Grab the current mouse x and y position
    GRT::VectorFloat inputVector(2);
    inputVector[0] = mouseX / double(ofGetWidth());
    inputVector[1] = mouseY / double(ofGetHeight());
    
    //If we are recording training data, then add the current sample to the training data set
    if( record ){
        record = false;
        trainingData.addSample( inputVector, targetVector );
    }
    
    //If the pipeline has been trained, then run the prediction
    if( pipeline.getTrained() ){
        pipeline.predict( inputVector );
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(225, 225, 225);

    //If the model has been trained, then draw the texture
    if( pipeline.getTrained() ){
        ofSetColor(255,255,255);
        ofFill();
        ofEnableAlphaBlending();
        texture.draw( 0, 0, ofGetWidth(), ofGetHeight() );
        ofDisableAlphaBlending();
    }
    
    //Draw the training data
    for(unsigned int i=0; i<trainingData.getNumSamples(); i++){
        float x = trainingData[i].getInputVector()[0] * ofGetWidth();
        float y = trainingData[i].getInputVector()[1] * ofGetHeight();
        float r = trainingData[i].getTargetVector()[0] * 255.0;
        float g = trainingData[i].getTargetVector()[1] * 255.0;
        float b = trainingData[i].getTargetVector()[2] * 255.0;
        ofSetColor( r, g, b );
        ofFill();
        ofDrawEllipse(x,y,5,5);
        ofSetColor(0,0,0);
        ofNoFill();
        ofDrawEllipse(x,y,5+1,5+1);;
    }

    //Draw the info text
    if( drawInfo ){
        float textX = 10;
        float textY = 25;
        float textSpacer = smallFont.getLineHeight() * 1.5;

        ofFill();
        ofSetColor(100,100,100);
        ofDrawRectangle( 5, 5, 250, 225 );
        ofSetColor( 255, 255, 255 );

        largeFont.drawString( "GRT Regressifier Example", textX, textY ); textY += textSpacer*2;

        smallFont.drawString( "[i]: Toogle Info", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[r]: Record Sample", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[t]: Train Model", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[1:5]: Set Target", textX, textY ); textY += textSpacer;
        smallFont.drawString( "[tab]: Select Regressifier", textX, textY ); textY += textSpacer;

        textY += textSpacer;
        smallFont.drawString( "Taget Value: " + ofToString( targetVector[0] ) + " " + ofToString( targetVector[1] ) + " " + ofToString( targetVector[2] ), textX, textY ); textY += textSpacer;
        smallFont.drawString( "Classifier: " + regressifierTypeToString( regressifierType ), textX, textY ); textY += textSpacer;
        smallFont.drawString( infoText, textX, textY ); textY += textSpacer;
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    infoText = "";
    bool buildTexture = false;
    
    switch ( key) {
        case 'r':
            record = true;
            break;
        case '1':
            targetVector[0] = 1;
            targetVector[1] = 0;
            targetVector[2] = 0;
            break;
        case '2':
            targetVector[0] = 0;
            targetVector[1] = 1;
            targetVector[2] = 0;
            break;
        case '3':
            targetVector[0] = 0;
            targetVector[1] = 0;
            targetVector[2] = 1;
            break;
        case '4':
            targetVector[0] = 0.5;
            targetVector[1] = 0.5;
            targetVector[2] = 0;
            break;
        case '5':
            targetVector[0] = 0.5;
            targetVector[1] = 0;
            targetVector[2] = 0.5;
            break;
        case 't':
            if( pipeline.train( trainingData ) ){
                infoText = "Pipeline Trained";
                buildTexture = true;
            }else infoText = "WARNING: Failed to train pipeline";
            break;
        case 's':
            if( trainingData.save( ofToDataPath("TrainingData.grt") ) ){
                infoText = "Training data saved to file";
            }else infoText = "WARNING: Failed to save training data to file";
            break;
        case 'l':
            if( trainingData.load( ofToDataPath("TrainingData.grt") ) ){
                infoText = "Training data saved to file";
            }else infoText = "WARNING: Failed to load training data from file";
            break;
        case 'c':
            trainingData.clear();
            infoText = "Training data cleared";
            break;
        case 'i':
            drawInfo = !drawInfo;
        break;
        case OF_KEY_TAB:
            setRegressifier( ++this->regressifierType % NUM_REGRESSIFIERS );
        break;
        default:
            break;
    }


    if( buildTexture ){
        const unsigned int rows = TEXTURE_RESOLUTION;
        const unsigned int cols = TEXTURE_RESOLUTION;
        const unsigned int size = rows*cols*4;
        vector<float> pixelData( size );
        ofFloatPixels pixels;

        unsigned int index = 0;
        unsigned int classLabel = 0;
        VectorFloat inputVector(2);
        VectorFloat regressionData;
        float r,g,b,a;
        for(unsigned int j=0; j<cols; j++){
            for(unsigned int i=0; i<rows; i++){
                inputVector[0] = i/double(rows);
                inputVector[1] = j/double(cols);
                if( pipeline.predict( inputVector ) ){
                    regressionData = pipeline.getRegressionData();
                    r = GRT::Util::limit(regressionData[0],0.0,1.0);
                    g = GRT::Util::limit(regressionData[1],0.0,1.0);
                    b = GRT::Util::limit(regressionData[2],0.0,1.0);
                    a = 1.0;
                    pixelData[ index++ ] = r;
                    pixelData[ index++ ] = g;
                    pixelData[ index++ ] = b;
                    pixelData[ index++ ] = a;
                }
            }
        }
        pixels.setFromExternalPixels(&pixelData[0],rows,cols,OF_PIXELS_RGBA);
        if(!texture.isAllocated()){
            texture.allocate( pixels, false );
            texture.setRGToRGBASwizzles(true);
        }
        texture.loadData( pixels );
        texture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
    }

}


bool ofApp::setRegressifier( const int type ){

    LinearRegression linearRegression;
    LogisticRegression logisticRegression;
    MLP mlp;

    this->regressifierType = type;

    pipeline.clear();

    switch( regressifierType ){
        case LINEAR_REGRESSION:
                pipeline << MultidimensionalRegression(linearRegression,true);
            break;
        case LOGISTIC_REGRESSION:
                pipeline << MultidimensionalRegression(logisticRegression,true);
            break;
        case NEURAL_NET:
            {
                unsigned int numInputNeurons = trainingData.getNumInputDimensions();
                unsigned int numHiddenNeurons = 10;
                unsigned int numOutputNeurons = 1; //1 as we are using multidimensional regression
                
                //Initialize the MLP
                mlp.init(numInputNeurons, numHiddenNeurons, numOutputNeurons, Neuron::LINEAR, Neuron::SIGMOID, Neuron::SIGMOID );
                
                //Set the training settings
                mlp.setMaxNumEpochs( 1000 ); //This sets the maximum number of epochs (1 epoch is 1 complete iteration of the training data) that are allowed
                mlp.setMinChange( 1.0e-10 ); //This sets the minimum change allowed in training error between any two epochs
                mlp.setLearningRate( 0.001 ); //This sets the rate at which the learning algorithm updates the weights of the neural network
                mlp.setNumRandomTrainingIterations( 5 ); //This sets the number of times the MLP will be trained, each training iteration starts with new random values
                mlp.setUseValidationSet( true ); //This sets aside a small portiion of the training data to be used as a validation set to mitigate overfitting
                mlp.setValidationSetSize( 20 ); //Use 20% of the training data for validation during the training phase
                mlp.setRandomiseTrainingOrder( true ); //Randomize the order of the training data so that the training algorithm does not bias the training
                
                //The MLP generally works much better if the training and prediction data is first scaled to a common range (i.e. [0.0 1.0])
                mlp.enableScaling( true );

                pipeline << MultidimensionalRegression(mlp,true);
            }
            break;
        default:
            return false;
        break;
    }

    return true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
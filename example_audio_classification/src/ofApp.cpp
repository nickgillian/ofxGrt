/*
 
 */

#include "ofApp.h"

#define AUDIO_BUFFER_SIZE 256
#define AUDIO_SAMPLE_RATE 44100
#define FFT_WINDOW_SIZE 512
#define FFT_MAG_SIZE FFT_WINDOW_SIZE/2
#define FFT_HOP_SIZE AUDIO_BUFFER_SIZE
#define SPECTROGRAM_SIZE 128
#define SPECTROGRAM_PLOT_SIZE SPECTROGRAM_SIZE*5

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);

    heatmap.load("heatmap");
    
    //Setup the FFT
    fft.init( FFT_WINDOW_SIZE, FastFourierTransform::HAMMING_WINDOW, true, false, false );
    fftInputData.resize( FFT_WINDOW_SIZE );
    spectrogram.resize( SPECTROGRAM_SIZE, VectorFloat(FFT_MAG_SIZE) );

    //Setup the plots
    magnitudePlot.setup( FFT_MAG_SIZE, 1 );
    //spectrogramPlot.setup( SPECTROGRAM_PLOT_SIZE, FFT_MAG_SIZE );
    spectrogramPlotBuffer.resize( SPECTROGRAM_PLOT_SIZE, VectorFloat(FFT_MAG_SIZE) );

    trainingClassLabel = 1;
    sampleCounter = 0;
    record = false;
    processAudio = true;
    featureVector.resize( SPECTROGRAM_SIZE * FFT_MAG_SIZE ); //The feature vector is the data from the spectrogram
    trainingData.setNumDimensions( featureVector.getSize() ); 
    trainingData.reserve( 100 * 1000 );

    //Setup the classifier
    RandomForests forest;
    forest.setForestSize( 10 );
    forest.setNumRandomSplits( (UINT)floor( featureVector.getSize()/10.0 ) );
    forest.setMaxDepth( 10 );
    forest.setMinNumSamplesPerNode( 10 );

    //Add the lassifier to the pipeline
    pipeline << forest;

    //Setup the audio card
    ofSoundStreamSetup(2, 1, this, AUDIO_SAMPLE_RATE, AUDIO_BUFFER_SIZE, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
    //All the updates are performed in the audio callback

    //Update the spectrogram plot, plot the data transposed as it looks nicer
    MatrixFloat matrix(FFT_MAG_SIZE,SPECTROGRAM_PLOT_SIZE);
    for(UINT i=0; i<SPECTROGRAM_PLOT_SIZE; i++){
        for(UINT j=0; j<FFT_MAG_SIZE; j++){
            matrix[j][i] = spectrogramPlotBuffer[i][FFT_MAG_SIZE-1-j];
        }
    }
    spectrogramPlot.update( matrix );
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0, 0, 0);

    string text;
    const int MARGIN = 20;
    const int graphSpacer = 15;
    int textX = MARGIN;
    int textY = MARGIN;

    float margin = 10;
    float x = margin;
    float y = textY += 35;
    float w = ofGetWidth() - margin*2;
    float h = 250;

    magnitudePlot.draw( x, y, w, h );

    y += h + 25;
    spectrogramPlot.draw( x, y, w, h, heatmap );

    //If the pipeline has been trained, then draw the plots
    if( pipeline.getTrained() ){

        //Draw the prediction info
        ofSetColor(255, 255, 255);
        text = "------------------- PredictionInfo -------------------";
        ofDrawBitmapString(text, textX,textY);

        textY += 15;
        text = "Pipeline Trained";
        ofDrawBitmapString(text, textX,textY);

        textY += 15;
        text = "Predicted Class Label: " + ofToString( pipeline.getPredictedClassLabel() );
        ofDrawBitmapString(text, textX,textY);

        

        y += h + 15;
        classLikelihoodsPlot.draw( x, y, w, h );
    }
    else{ //Draw the training info

        //Draw the training info
        ofSetColor(255, 255, 255);
        text = "------------------- TrainingInfo -------------------";
        ofDrawBitmapString(text, textX,textY);
        
        if( record ) ofSetColor(255, 0, 0);
        else ofSetColor(255, 255, 255);
        textY += 15;
        text = record ? "RECORDING" : "Not Recording";
        ofDrawBitmapString(text, textX,textY);
        
        ofSetColor(255, 255, 255);
        textY += 15;
        text = "TrainingClassLabel: " + ofToString(trainingClassLabel);
        ofDrawBitmapString(text, textX,textY);
        
        textY += 15;
        text = "NumTrainingSamples: " + ofToString(trainingData.getNumSamples());
        ofDrawBitmapString(text, textX,textY);

        textY += 15;
        text = "Info: " + infoText;
        ofDrawBitmapString(text, textX,textY);

    }

}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){

    if( !processAudio ) return;

    for (int i=0; i<bufferSize; i++) {
        fftInputData[sampleCounter++] = input[i];
    }

    if( sampleCounter >= FFT_HOP_SIZE ){
        sampleCounter = 0;
        //Compute the fft
        if( !fft.computeFFT( fftInputData ) ) return;

        //Update the FFT plot
        VectorFloat rawMagData = fft.getMagnitudeData();
        magnitudePlot.setData( rawMagData );

        //Update the spectorgram
        VectorFloat scaledMagData( FFT_MAG_SIZE );
        Float minValue = -64;
        Float maxValue = 32;
        for(UINT i=0; i<FFT_MAG_SIZE; i++){
            scaledMagData[i] = GRT::Util::scale( 20.0 * log10( rawMagData[i] + 1.0e-8 ), minValue, maxValue, 0.0, 1.0, true );
        }
        spectrogram.push_back( scaledMagData );
        spectrogramPlotBuffer.push_back( scaledMagData );

        UINT featureIndex = 0;
        for(UINT i=0; i<SPECTROGRAM_SIZE; i++){
            for(UINT j=0; j<FFT_MAG_SIZE; j++){
                featureVector[ featureIndex++ ] = spectrogram[i][j];
            }
        }

        if( record ){
            trainingData.addSample( trainingClassLabel, featureVector );
        }
        else if( pipeline.getTrained() ){

            //Run the prediction using the spectrogram features
            pipeline.predict( featureVector );

            //Update the likelihood plot
            classLikelihoodsPlot.update( pipeline.getClassLikelihoods() );
        }
    }
}

void ofApp::exit(){
    processAudio = false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch ( key) {
        case 'r':
            record = !record;
            break;
        case '1':
            trainingClassLabel = 1;
            break;
        case '2':
            trainingClassLabel = 2;
            break;
        case '3':
            trainingClassLabel = 3;
            break;
        case 't':
            if( pipeline.train( trainingData ) ){
                infoText = "Pipeline Trained";

                //Update the plots
                classLikelihoodsPlot.setup( 60 * 5, pipeline.getNumClasses() );
                classLikelihoodsPlot.setRanges(0,1);
            }else infoText = "WARNING: Failed to train pipeline";
            break;
        case 's':
            if( trainingData.save("TrainingData.grt") ){
                infoText = "Training data saved to file";
            }else infoText = "WARNING: Failed to save training data to file";
            break;
        case 'l':
            if( trainingData.load("TrainingData.grt") ){
                infoText = "Training data saved to file";
            }else infoText = "WARNING: Failed to load training data from file";
            break;
        case 'c':
            trainingData.clear();
            infoText = "Training data cleared";
            break;
        default:
            break;
    }

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
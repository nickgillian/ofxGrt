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

/*
 Logistic Regression Example
 This examples demonstrates how to initialize, train, and use the LinearRegression class for regression.

 Linear Regression is a simple regression algorithm that can map an N-dimensional signal to a 1-dimensional signal.

 In this example we create an instance of an LinearRegression algorithm and then use the algorithm to train a model using some pre-recorded training data.
 The trained model is then used to perform regression on the test data.

 This example shows you how to:
 - Create an initialize the LinearRegression algorithm for regression
 - Create a new instance of a GestureRecognitionPipeline and add the regression instance to the pipeline
 - Load some RegressionData from a file
 - Train a LinearRegression model using the training dataset
 - Test the LinearRegression model using the test dataset
 - Save the output of the LinearRegression algorithm to a file
*/

//You might need to set the specific path of the GRT header relative to your project
#include <GRT/GRT.h>
using namespace GRT;
using namespace std;

int main (int argc, const char * argv[])
{
    //Parse the data filename from the argument list
    if( argc != 3 ){
        cout << "Error: failed to parse data filename from command line. You should run this example with two arguments for the training and test datasets filenames\n";
        return EXIT_FAILURE;
    }
    const string trainingDataFilename = argv[1];
    const string testDataFilename = argv[2];

    //Turn on the training log so we can print the training status of the LinearRegression to the screen
    TrainingLog::enableLogging( true ); 

    //Load the training data
    RegressionData trainingData;
    RegressionData testData;

    if( !trainingData.load( trainingDataFilename ) ){
        cout << "ERROR: Failed to load training data: " << trainingDataFilename << endl;
        return EXIT_FAILURE;
    }
    
    if( !testData.load( testDataFilename ) ){
        cout << "ERROR: Failed to load test data: " << testDataFilename << endl;
        return EXIT_FAILURE;
    }

    //Make sure the dimensionality of the training and test data matches
    if( trainingData.getNumInputDimensions() != testData.getNumInputDimensions() ){
        cout << "ERROR: The number of input dimensions in the training data (" << trainingData.getNumInputDimensions() << ")";
        cout << " does not match the number of input dimensions in the test data (" << testData.getNumInputDimensions() << ")\n";
        return EXIT_FAILURE;
    }

    if( testData.getNumTargetDimensions() != testData.getNumTargetDimensions() ){
        cout << "ERROR: The number of target dimensions in the training data (" << testData.getNumTargetDimensions() << ")";
        cout << " does not match the number of target dimensions in the test data (" << testData.getNumTargetDimensions() << ")\n";
        return EXIT_FAILURE;
    }

    cout << "Training and Test datasets loaded\n";

    //Print the stats of the datasets
    cout << "Training data stats:\n";
    trainingData.printStats();

    cout << "Test data stats:\n";
    testData.printStats();

    //Create a new gesture recognition pipeline
    GestureRecognitionPipeline pipeline;

    //Add a LinearRegression instance to the pipeline
    pipeline.setRegressifier( LinearRegression() );

    //Train the LinearRegression model
    cout << "Training LogisticRegression model...\n";
    if( !pipeline.train( trainingData ) ){
        cout << "ERROR: Failed to train LinearRegression model!\n";
        return EXIT_FAILURE;
    }

    cout << "Model trained.\n";

    //Test the model
    cout << "Testing LinearRegression model...\n";
    if( !pipeline.test( testData ) ){
        cout << "ERROR: Failed to test LinearRegression model!\n";
        return EXIT_FAILURE;
    }

    cout << "Test complete. Test RMS error: " << pipeline.getTestRMSError() << endl;

    //Run back over the test data again and output the results to a file 
    fstream file;
    file.open("LinearRegressionResultsData.csv", fstream::out);

    VectorFloat inputVector, targetVector, outputVector;

    for(UINT i=0; i<testData.getNumSamples(); i++){
        inputVector = testData[i].getInputVector();
        targetVector = testData[i].getTargetVector();

        //Map the input vector using the trained regression model
        if( !pipeline.predict( inputVector ) ){
            cout << "ERROR: Failed to map test sample " << i << endl;
            return EXIT_FAILURE;
        }
        
        //Get the mapped regression data
        outputVector = pipeline.getRegressionData();
        
        //Write the mapped value and also the target value to the file
        for(UINT j=0; j<outputVector.size(); j++){
            file << outputVector[j] << "\t";
        }
        for(UINT j=0; j<targetVector.size(); j++){
            file << targetVector[j] << "\t";
        }
        file << endl;
    }

    //Close the file
    file.close();

    return EXIT_SUCCESS;
}

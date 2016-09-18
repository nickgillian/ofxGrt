##Regression Example

This example demonstrates the main GRT regression algorithms in one application.  

The example lets you experiment with three different GRT regression algorithms, namely:

1. Linear Regression
2. Logistic Regression
3. Neural Network (Multilayer Perceptron)

The GRT regression algorithms enable you to take any N-dimensional input signal and map it to a K-dimensional output signal.

This example sets up a basic framework that allows you to map a two dimensional input signal (the [x y] coordinates of the mouse on the screen), to a three dimensional output signal (which is used to control the red, green and blue pixel colors on the screen).

The example lets you add training data to different regions of the screen, then use this training data to build a model which is then used to color all the pixels on the screen, as shown in the image below.

![alt text](https://github.com/nickgillian/ofxGrt/tree/master/example_regression/images/regression_step_10.png "Regression Example")

All regression algorithms have both strengths and weaknesses.  This simply example should demonstrate some important aspects of each algorithm, such as:

* does the algorithm support multi-dimensional inputs (all GRT regression algorithms do!)
* does the algorithm support multi-dimensional outputs (all GRT regression algorithms do, either inheriently like the MLP neural network, or via using the GRT MultiDimensionalRegression meta algorithm)
* does the algorithm support non-linear mappings (Linear and Logistic regression do not, however the MLP does)
* is the algorithm stable (i.e., if you train it multiple times, do you get the same result each time, the MLP starts from random values each time, so its model can change significantly over multiple runs)

See the instructions below on how to build, run, and use the example.

##Building and running the example
On OS X and Linux, you can build this example by running the following command in terminal:

````
cd THIS_DIRECTORY
make -j4
````

To run the example, run the following in terminal:

````
make run
````

##Using the example
To record a training example, simply move your mouse to a specific location in the app window and press the **r** key.  This will add one training example for the current class.  You can change the class label by pressing keys **1**, **2**, or **3**.  This will set the class color to red, green, or blue respectively.

When you have recorded a few training examples for each class, press the **t** key to train a model for the currently selected classifier.

After the model has been trained, it will automatically color each pixel on the screen with the color of the most likely class.

To change the classifier type, use the **tab** key.
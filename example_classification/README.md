##Classifier Example

This example demonstrates the main GRT classifiers in one application.  The example lets you add training data for up to 3 classes (defined by the [x y] coordinates of the mouse on the screen), train a classification model, and then classify all the pixels in the application window using the trained model. 

This demonstrates the various decision boundaries learned by each classification algorithm, which should give a basic insite into the strengths and weaknesses of each classification algorithm.

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
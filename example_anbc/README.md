##ANBC Example

This example demonstrates how to use the GRT Naive Bayes classifier (ANBC).

The Adaptive Naive Bayes Classifier (ANBC) is a naive but powerful classifier that works very well on both basic and more complex recognition problems.

See this [wiki page](http://www.nickgillian.com/wiki/pmwiki.php/GRT/ANBC) for more details on the algorithm.

##Example
This example lets you add training data for up to 3 classes (defined by the [x y] coordinates of the mouse on the screen), train an ANBC model, and then classify all the pixels in the application window using the ANBC model.  

This shows you the basics of how to record training data for the ANBC algorithm, how to train a model, and how to run prediction.

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

When you have recorded a few training examples for each class, press the **t** key to train the ANBC model.

After the model has been trained, it will automatically color each pixel on the screen with the color of the most likely class.
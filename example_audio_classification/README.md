##Audio Classification Example

This example demonstrates how to perform some basic audio classification using an FFT and a RandomForests classifier.

##Building the example
You can build this example on OSX by opening terminal in this directory and running:

````
make -j4
make run
````

This will compile the example using the pre-built GRT static library packaged with the ofxGrt addon.

If you want to build this example on Linux or want to compile the example against your own version of the GRT (installed in /usr/local/...) then edit the 
main **addon_config.mk** file to change the location of the GRT header and library paths.

##Using the example

###Training
You can record some training data for this example by:

1. Press a number key (e.g., 1) to set the class label
2. Press the **r** key to start the recording
3. Make some noise to represent the current class label (e.g., the first class could just be generic background noise)
4. Press another number key (e.g., 2) to change the class label and immediately start making noise for the new class (e.g., start humming) 
5. Repeat this for each audio class you want to add
6. Press the **r** key to stop the recording
7. Press the **t** key to train a model (this could take several seconds depending on the number of training samples)

###Prediction
After training a model, the application will immediately start real-time prediction.  You should now see two graphs on the screen, the top graph shows the FFT magnitude data, with the bottom class shows the class likelihoods for each class in the model.

If you make the sound for each class, you should hopefully see the class likelihoods change in the lower graph.
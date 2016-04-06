##Gyrosc Orientation Example

This example interfaces with [gyrosc](http://www.bitshapesoftware.com/instruments/gyrosc), an application that sends sensor data from your iPhone, iPod Touch, or iPad's motion sensors over your local wireless network to this openFrameworks application via the [OSC](http://opensoundcontrol.org/introduction-osc) protocol.

This example is setup for orientation classification (i.e., detecting which orientation the phone is currently held in), using the **gravity** data from the gyrosc data stream and a GRT Naive Bayes classifier.

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
Download and install the [gyrosc](http://www.bitshapesoftware.com/instruments/gyrosc) app on your iOS device.  Set the IP address in the gyrosc app to that of your computer and set the receiving port number to 5000 (both your host machine and iOS device need to be on the same wireless network).

When you start this example, you should see the graphs on the screen change as you move your iOS device (if not, then check the network settings, IP and port values all match).

To record a training example, simply orientate your iOS device in a specific orientation (e.g., screen facing up), then press the **r** key to start recording some training examples for that class (the class should default to 1 when you start the example).  Move your phone around for a few seconds to add some variation to the orientation, then press the **r** key again to stop the recording.

Press the **2** number key to switch the class label to 2, move your iOS device to a new orientation (e.g., screen facing down), and press the **r** key again to record some training data for class 2.  Move your phone around for a few seconds to add some variation to the orientation, then press the **r** key again to stop the recording.

Repeat this again for class **3**, holding the phone in a new orientation.

When you have recorded training data for each class (a few hundred training samples per class should be enough), press the **t** key to train a Naive Bayes classification model.

After the model has been trained, realtime classification should immediately start and a new graph should appear that shows the real-time probabilities for detecting each class (i.e, orientation).  If you trained 3 classes, then the graph colors should represent:
- red: class 1
- green: class 2
- blue: class 3

As you move your phone around the various orientations, you should see the class probabilities change (with the most likely class being the value closest to 1.0).
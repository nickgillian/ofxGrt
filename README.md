#ofxGrt

This repo contains a GRT ([Gesture Recognition Toolkit](https://github.com/nickgillian/grt)) extension for openFrameworks.  

The GRT is a cross-platform, open-source, C++ machine learning library designed for real-time gesture recognition.

This extension is designed to make it as easy as possible to use the GRT in openFrameworks.

Note that this extensions contains a local, pre compiled version of the GRT, so you do not need to download and install the main GRT library to use the ofxGRT addon.

To use ofxGrt:

```
- download openFrameworks (you will need an OF version that supports C++11, you can find the latest version on the openFrameworks github page)

- clone this repo to the openFrameworks addons folder
```
    cd openFrameworks/addons
    git clone https://github.com/nickgillian/ofxGrt
```

- to build and run any of the ofxGRT examples, cd to an example folder and run:
```
    cd ofxGRT/example_timeseries_plot
    make
    make run
```

You can find additional README files in each of the examples that describe more details about each specific example.

##License
The ofxGRT is available under a MIT license.

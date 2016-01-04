#ofxGrt

This repo contains a GRT extension for openFrameworks.

To use ofxGrt:

- download and install the Gesture Recognition Toolkit: https://github.com/nickgillian/grt
```
    git clone https://github.com/nickgillian/grt
    cd grt/build
    mkdir build
    cd build
    cmake ..
    make
    sudo make install
```
- download openFrameworks (you will need an OF version that supports C++11, you can find the latest version on the openFrameworks github page)

- clone this repo to the openFrameworks addons folder

    cd openFrameworks/addons
    git clone https://github.com/nickgillian/ofxGrt

- cd to one of the examples folders and run **make**

    cd ofxGRT/example_timeseries_plot
    make
    make run


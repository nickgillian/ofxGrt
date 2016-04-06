#pragma once

#include "ofMain.h"
#include "ofxGrt.h"
#include "ofxOsc.h"

class Gyrosc{
public:
	Gyrosc(){

		newData = false;
		acc.resize( 3, 0 );
		grav.resize( 3, 0 );

	}

	~Gyrosc(){

	}

	bool setup( int listenerPort ){

		oscReceiver.setup( listenerPort );
		newData = false;

		return true;
	}

	bool update(){

		newData = false;
		ofxOscMessage msg;
		while( oscReceiver.hasWaitingMessages() ){
			if( oscReceiver.getNextMessage( msg ) ){
				if( msg.getAddress() == "/gyrosc/accel" ){
					newData = true;
					acc[0] = msg.getArgAsFloat( 0 );
					acc[1] = msg.getArgAsFloat( 1 );
					acc[2] = msg.getArgAsFloat( 2 );
				}
				else if( msg.getAddress() == "/gyrosc/grav" ){
					newData = true;
					grav[0] = msg.getArgAsFloat( 0 );
					grav[1] = msg.getArgAsFloat( 1 );
					grav[2] = msg.getArgAsFloat( 2 );
				}
			}
		}

		return true;
	}

	bool draw(float x,float y,float w,float h){


		return true;
	}

	bool getNewDataReady() const {
		return newData;
	}

	GRT::VectorFloat getAcc() const {
		return acc;
	}

	GRT::VectorFloat getGrav() const {
		return grav;
	}

protected:
	bool newData;
	GRT::VectorFloat acc;
	GRT::VectorFloat grav;
	ofxOscReceiver oscReceiver;
};
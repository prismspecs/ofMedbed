#pragma once
#include "ofxJSON.h"
#include "ofMain.h"

class ofApp : public ofBaseApp
{

public:
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void sendSerialData();

	ofSerial serial;
	#define SERIAL_PORT "/dev/cu.usbmodem1101"
	#define BAUD_RATE 38400
	ofVideoPlayer vid;
	ofSoundPlayer sound1;
	ofxJSONElement json;
	float durationInSeconds;
};
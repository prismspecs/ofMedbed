#pragma once
#include "ofxJSON.h"
#include "ofMain.h"
// #include "ofxSerial.h"

class ofApp : public ofBaseApp
{

public:
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;

	void keyPressed(int key) override;
	void sendSerial(string arduinoData);

	ofTrueTypeFont myFont;

	ofSerial serial;
	// #define SERIAL_PORT "/dev/cu.usbmodem1101"
	// #define SERIAL_PORT "/dev/ttyUSB0"
	#define SERIAL_PORT "/dev/ttyACM0" // UNO
	#define BAUD_RATE 115200
	

	ofVideoPlayer vid;
	// ofSoundPlayer sound1;
	ofxJSONElement json;
	float duration_seconds;

	bool test_mode = true;
};

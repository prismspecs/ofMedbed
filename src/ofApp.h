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
	void readSerial();
	void videoEnded();
	unsigned char mapDataToByte(string data);

	ofTrueTypeFont myFont;

	ofSerial serial;
    // #define SERIAL_PORT "/dev/cu.usbmodem1101"
	// #define SERIAL_PORT "/dev/ttyUSB0"
	#define SERIAL_PORT "/dev/ttyACM0" // UNO
	#define BAUD_RATE 57600
	#define VIDEO_FILENAME "video-vp9opus.webm"
	#define TIMESTAMPS_FILE "timestamps.json"
	const bool BYTE_MODE = true;
	
	ofVideoPlayer vid;
	// ofSoundPlayer sound1;
	ofxJSONElement json;
	float duration_seconds;

	bool test_mode = false;
};

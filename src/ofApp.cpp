#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // set up serial
    serial.setup(SERIAL_PORT, BAUD_RATE);

    vid.load("video.mp4");
    vid.setLoopState(OF_LOOP_NORMAL);
    durationInSeconds = vid.getDuration();
    ofLogNotice("setup") << "Video duration: " << durationInSeconds << " seconds.";
    vid.play();

    if (json.open("timestamps.json"))
    {
        ofLogNotice("setup") << "Successfully loaded JSON file.";
    }
    else
    {
        ofLogError("setup") << "Failed to load JSON file.";
    }

    // add a property to every member of json object "isFinished"
    for (int i = 0; i < json.size(); i++)
    {
        json[i]["completed"] = false;
    }

    ofLogNotice("setup") << json.getRawString(true);
}

//--------------------------------------------------------------
void ofApp::update()
{

    vid.update();

    float currentPositionInSeconds = vid.getPosition() * durationInSeconds;

    // log video time
    // ofLogNotice("update") << "Video time: " << currentPositionInSeconds << " seconds.";

    // check if video time has gone past a timestamp
    for (int i = 0; i < json.size(); i++)
    {
        if (currentPositionInSeconds > json[i]["time"].asInt() && !json[i]["completed"].asBool()) {
            // ofLogNotice("update") << "Video time has gone past " << json[i]["time"].asFloat() << " seconds.";
            json[i]["completed"] = true;

            
            // create a string out of timestamp selectedOption and integerValue
            string arduinoData = json[i]["selectedOption"].asString() + "," + json[i]["integerValue"].asString() + "\n";

            // log this
            ofLogNotice("update") << "Sending to Arduino: " << arduinoData;

            // send to arduino
            serial.writeBytes((unsigned char*)arduinoData.c_str(), arduinoData.size());

            
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    vid.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::exit()
{
    serial.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    // toggle fullsreen
    if (key == 'f')
    {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::sendSerialData() {
    // Data to send (replace with your data)
    unsigned char dataToSend = 42;  // Change this to the data you want to send

    // Send the data as a single byte
    serial.writeByte(dataToSend);
}

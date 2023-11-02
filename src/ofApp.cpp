#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // try to setup serial connection
    if (serial.setup(SERIAL_PORT, BAUD_RATE))
    {
        ofLogNotice("setup") << "Successfully connected to Arduino.";
    }
    else
    {
        ofLogError("setup") << "Unable to connect to Arduino.";
    }

    vid.load("video.mp4");
    vid.setLoopState(OF_LOOP_NORMAL);
    duration_seconds = vid.getDuration();
    ofLogNotice("setup") << "Video duration: " << duration_seconds << " seconds.";
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

    myFont.load("UbuntuMono.ttf", 32);
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (!test_mode)
    {
        vid.update();

        float currentPositionInSeconds = vid.getPosition() * duration_seconds;

        // log video time
        // ofLogNotice("update") << "Video time: " << currentPositionInSeconds << " seconds.";

        // check if video time has gone past a timestamp
        for (int i = 0; i < json.size(); i++)
        {
            if (currentPositionInSeconds > json[i]["time"].asInt() && !json[i]["completed"].asBool())
            {
                // ofLogNotice("update") << "Video time has gone past " << json[i]["time"].asFloat() << " seconds.";
                json[i]["completed"] = true;

                // create a string out of timestamp selectedOption and integerValue
                string arduinoData = json[i]["selectedOption"].asString() + "," + json[i]["integerValue"].asString() + "\n";

                // log this
                ofLogNotice("update") << "Sending to Arduino: " << arduinoData;

                // send to arduino
                serial.writeBytes((unsigned char *)arduinoData.c_str(), arduinoData.size());
            }
        }
    }
    else
    {
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if (!test_mode)
    {
        vid.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    else
    {
        // background black
        ofBackground(0);

        // white fill
        ofSetColor(255, 255, 255);

        // draw text to screen "test mode" with large font
        int spacing = 40;
        myFont.drawString("TEST MODE", spacing, spacing * 2);
        myFont.drawString("Q: LEDs Red", spacing, spacing * 4);
        myFont.drawString("W: LEDs Blue", spacing, spacing * 5);
    }
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
    else if (key == 't')
    {
        test_mode = !test_mode;
        ofLogNotice("keyPressed") << "Test mode: " << (test_mode ? "activated" : "deactivated");
    }
    else if (key == 'q')
    {
        ofLogNotice("quick command") << "LEDs Red";
        sendSerial("LEDs,1");
    }
}
//--------------------------------------------------------------
void ofApp::sendSerial(string arduinoData)
{
    // Your string to be sent over serial
    string myString = arduinoData;

    // Convert the string to a C-style character array (char*)
    const char *cString = myString.c_str();

    // Write the string to the serial port
    serial.writeBytes((unsigned char *)cString, myString.length());
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::sendSerialData()
{
    // Data to send (replace with your data)
    unsigned char dataToSend = 42; // Change this to the data you want to send

    // Send the data as a single byte
    serial.writeByte(dataToSend);
}

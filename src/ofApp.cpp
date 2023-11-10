#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // try to setup serial connection
    serial.listDevices();
    if (serial.setup(SERIAL_PORT, BAUD_RATE))
    {
        ofLogNotice("setup") << "Successfully connected to Arduino.";
    }
    else
    {
        ofLogError("setup") << "Unable to connect to Arduino.";
    }

    vid.load("final.webm");
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
        // LED commands
        myFont.drawString("Q: LEDs Off", spacing, spacing * 4);
        myFont.drawString("W: LEDs Blue", spacing, spacing * 5);
        myFont.drawString("E: LEDs Red", spacing, spacing * 6);
        // scanner commands
        myFont.drawString("R: Scanner Stop", spacing, spacing * 8);
        myFont.drawString("T: Scanner Forward", spacing, spacing * 9);
        myFont.drawString("Y/Z: Scanner Reverse", spacing, spacing * 10);
        // tilt commands
        myFont.drawString("U: Tilt Stop", spacing, spacing * 12);
        myFont.drawString("I: Tilt Back", spacing, spacing * 13);
        myFont.drawString("O: Tilt Forward", spacing, spacing * 14);
        // door commands
        myFont.drawString("A: Doors Stop", spacing, spacing * 16);
        myFont.drawString("S: Doors Close", spacing, spacing * 17);
        myFont.drawString("D: Doors Open", spacing, spacing * 18);
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
    else if (key == 'm')
    {
        test_mode = !test_mode;
        ofLogNotice("keyPressed") << "Test mode: " << (test_mode ? "activated" : "deactivated");
    }
    // LEDs
    if (test_mode && key == 'q')
    {
        ofLogNotice("quick command") << "LEDs Off";
        sendSerial("LEDs,0\n");
    }
    if (test_mode && key == 'w')
    {
        ofLogNotice("quick command") << "LEDs Blue";
        sendSerial("LEDs,1\n");
    }
    if (test_mode && key == 'e')
    {
        ofLogNotice("quick command") << "LEDs Red";
        sendSerial("LEDs,2\n");
    }
    // scan motor
    if (test_mode && key == 'r')
    {
        ofLogNotice("quick command") << "Scanner Stop";
        sendSerial("DEBUG,0");
    }
    if (test_mode && key == 't')
    {
        ofLogNotice("quick command") << "Scanner Forward";
        sendSerial("DEBUG,9");
    }
    if (test_mode && key == 'y')
    {
        ofLogNotice("quick command") << "Scanner Reverse";
        sendSerial("DEBUG,11");
    }
    // tilt motor
    if (test_mode && key == 'u')
    {
        ofLogNotice("quick command") << "Tilt Stop";
        sendSerial("Tilt,0");
    }
    if (test_mode && key == 'i')
    {
        ofLogNotice("quick command") << "Tilt Back";
        sendSerial("Tilt,1");
    }
    if (test_mode && key == 'o')
    {
        ofLogNotice("quick command") << "Tilt Forward";
        sendSerial("Tilt,2");
    }
    if (test_mode && key == 'a')
    {
        ofLogNotice("quick command") << "Doors Stop";
        sendSerial("Doors,0");
    }
    if (test_mode && key == 's')
    {
        ofLogNotice("quick command") << "Doors Close";
        sendSerial("Doors,1");
    }
    if (test_mode && key == 'd')
    {
        ofLogNotice("quick command") << "Doors Open";
        sendSerial("Doors,2\n");
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
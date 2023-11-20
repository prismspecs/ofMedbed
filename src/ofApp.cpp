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

    vid.load(VIDEO_FILENAME);
    vid.setLoopState(OF_LOOP_NORMAL);
    duration_seconds = vid.getDuration();
    ofLogNotice("setup") << "Video duration: " << duration_seconds << " seconds.";
    vid.play();

    if (json.open(TIMESTAMPS_FILE))
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

                // combine info into a string
                string arduinoData = json[i]["selectedOption"].asString() + "," + json[i]["integerValue"].asString();

                ofLogNotice("json") << "command: " << arduinoData;

                // send to arduino
                // bool bytesWereWritten = serial.writeBytes((unsigned char *)arduinoData.c_str(), arduinoData.size());
                sendSerial(arduinoData);
            }
        }
    }
    else
    {
    }

    readSerial();

    if (vid.getIsMovieDone())
    {
        videoEnded();
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
        myFont.drawString("E: LEDs Purple", spacing, spacing * 6);
        // scanner commands
        myFont.drawString("R: Scanner Stop", spacing, spacing * 8);
        myFont.drawString("T: Scanner -> Head", spacing, spacing * 9);
        myFont.drawString("Y/Z: Scanner -> Feet", spacing, spacing * 10);
        // tilt commands
        myFont.drawString("U: Tilt Stop", spacing, spacing * 12);
        myFont.drawString("I: Tilt Back", spacing, spacing * 13);
        myFont.drawString("O: Tilt Forward", spacing, spacing * 14);
        // door commands
        myFont.drawString("A: Doors Stop", spacing, spacing * 16);
        myFont.drawString("S: Doors Close", spacing, spacing * 17);
        myFont.drawString("D: Doors Open", spacing, spacing * 18);
        // home commands
        myFont.drawString("H: Home", ofGetWidth() / 2, spacing * 2);
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
        sendSerial("LEDs,0");
    }
    if (test_mode && key == 'w')
    {
        ofLogNotice("quick command") << "LEDs Blue";
        sendSerial("LEDs,1");
    }
    if (test_mode && key == 'e')
    {
        ofLogNotice("quick command") << "LEDs Red";
        sendSerial("LEDs,2");
    }
    // scan motor
    if (test_mode && key == 'r')
    {
        ofLogNotice("quick command") << "Scanner Stop";
        sendSerial("DEBUG,0");
    }
    if (test_mode && key == 't')
    {
        ofLogNotice("quick command") << "Scanner towards Head";
        sendSerial("DEBUG,9");
    }
    if (test_mode && key == 'y')
    {
        ofLogNotice("quick command") << "Scanner towards Feet";
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
        sendSerial("Tilt,2");
    }
    if (test_mode && key == 'o')
    {
        ofLogNotice("quick command") << "Tilt Forward";
        sendSerial("Tilt,1");
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
        sendSerial("Doors,2");
    }
    if (test_mode && key == 'h')
    {
        ofLogNotice("quick command") << "Home";
        sendSerial("Home,1");
    }

    // video jump ahead...
    if (key == 'j')
    {
        vid.setPosition(.98);
    }
}
//--------------------------------------------------------------
void ofApp::sendSerial(string arduinoData)
{
    ofLogNotice("update") << "Sending to Arduino: " << arduinoData;

    if (BYTE_MODE)
    {
        unsigned char myByte = mapDataToByte(arduinoData);
        // ofLogNotice("serial") << "wrote myByte:" << myByte;

        bool byteWasWritten = serial.writeByte(myByte);
        if (!byteWasWritten)
            ofLogNotice("serial") << "byte was not written to serial port";
    }
    else
    {
        arduinoData += "\n";

        ofLogNotice("serial") << arduinoData;

        // your string to be sent over serial
        string myString = arduinoData;

        // convert the string to a C-style character array (char*)
        const char *cString = myString.c_str();

        // write the string to the serial port
        serial.writeBytes((unsigned char *)cString, myString.length());
    }
}
//--------------------------------------------------------------
void ofApp::readSerial()
{
    if (serial.available() > 0)
    {
        string receivedString;
        while (serial.available() > 0)
        {

            char byte = serial.readByte();
            if (byte == '\n')
            {
                break;
            }
            else
            {
                receivedString += byte;
            }
        }
        // log this
        ofLogNotice("Received from Arduino") << receivedString;
    }
}
//--------------------------------------------------------------
void ofApp::videoEnded()
{

    ofLogNotice("video") << "looped";

    // restart the video to loop again
    vid.setPosition(0);
    vid.play();

    // reset json flags
    for (int i = 0; i < json.size(); i++)
    {

        json[i]["completed"] = false;
    }
}
//--------------------------------------------------------------
unsigned char ofApp::mapDataToByte(string data)
{
    /*

                0   Home,0
                10  Scanner,0
                11  Scanner,1
                12  Scanner,2
                13  Scanner,3
                14  Scanner,4
                20  LEDs,0
                21  LEDs,1
                22  LEDs,2
                23  LEDs,3
                24  LEDs,4
                30  Tilt,0
                31  Tilt,1
                32  Tilt,2
                40  Doors,0
                41  Doors,1
                42  Doors,2

            */

    string arduinoData = data;

    ofStringReplace(arduinoData, " ", ""); // Remove spaces
    ofTrim(arduinoData);
    ofStringReplace(arduinoData, "\r", ""); // Remove carriage return if present
    ofStringReplace(arduinoData, "\n", ""); // Remove newline if present

    unsigned char myByte;

    // Switch statement
    if (arduinoData == "Home,0")
    {
        myByte = 0;
    }
    else if (arduinoData == "Scanner,0")
    {
        myByte = 10;
    }
    else if (arduinoData == "Scanner,1")
    {
        myByte = 11;
    }
    else if (arduinoData == "Scanner,2")
    {
        myByte = 12;
    }
    else if (arduinoData == "Scanner,3")
    {
        myByte = 13;
    }
    else if (arduinoData == "Scanner,4")
    {
        myByte = 14;
    }
    else if (arduinoData == "LEDs,0")
    {
        myByte = 20;
    }
    else if (arduinoData == "LEDs,1")
    {
        myByte = 21;
    }
    else if (arduinoData == "LEDs,2")
    {
        myByte = 22;
    }
    else if (arduinoData == "LEDs,3")
    {
        myByte = 23;
    }
    else if (arduinoData == "LEDs,4")
    {
        myByte = 24;
    }
    else if (arduinoData == "Tilt,0")
    {
        myByte = 30;
    }
    else if (arduinoData == "Tilt,1")
    {
        myByte = 31;
    }
    else if (arduinoData == "Tilt,2")
    {
        myByte = 32;
    }
    else if (arduinoData == "Doors,0")
    {
        myByte = 40;
    }
    else if (arduinoData == "Doors,1")
    {
        myByte = 41;
    }
    else if (arduinoData == "Doors,2")
    {
        myByte = 42;
    }
    else
    {
        ofLogNotice("serial") << "got arduinoData without map:" << arduinoData;
    }

    return myByte;
}
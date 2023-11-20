#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1920, 1080);
	// settings.setPosition
	settings.setPosition(glm::vec2(1920,0)); 
	settings.windowMode = OF_FULLSCREEN; // switch to OF_GAME_MODE for production

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}

#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include "ofAppGLFWWindow.h"

int main( ){
    ofAppGLFWWindow window;
    window.setDoubleBuffering( true );
    window.setNumSamples( 0 );
    ofSetupOpenGL( &window, 1920, 1080, OF_WINDOW );
	ofRunApp( ofApp::init() );
}

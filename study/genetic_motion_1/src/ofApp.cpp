#include "ofApp.h"

void ofApp::setup(){
	
	for( int i=0; i<num_agent; i++ ) {
		float rad = (float)ofGetWidth() / 22;
		ofPoint loc( (i%20)*ofGetWidth()/20+rad/2, (i/20)*ofGetHeight()/20+rad/2 );
		la[i].setup( loc, rad );
	}
	
	ofSetFrameRate( 60 );
}

void ofApp::update(){
}

void ofApp::draw(){
	
    ofEnableAlphaBlending();
    
    
	ofBackground( 0 );
	
    ofSetColor( 255 );
	if( ofGetMousePressed(0) ){
		for( int i=0; i<num_agent; i++ ) {
            la[i].dna.setBoundsMode( i%3 );
            la[i].dna.mutate( ofMap(mouseX, 0, ofGetWidth(), 0, 0.1) );
		}
	}

	if( ofGetKeyPressed() ){
		for( int i=0;i<num_agent-1; i++ ){
            la[i].dna.setBoundsMode( i%3 );
            la[i].dna.setMateMode( floor(ofRandom(0,4)) );
			la[i].dna.mate( la[i+1].dna, ofMap(mouseX, 0, ofGetWidth(), 0, 0.1)  );
		}
	} 
	
	for( int i=0; i<num_agent; i++ ){
		la[i].display();
	}
	string txt = "notmutating"; 
	if( ofGetMousePressed(0)) txt = "mutating";
	if( ofGetKeyPressed()) txt = "mating";
	txt += "\nmutationindex: "+ofToString(ofMap(mouseX, 0, ofGetWidth(), 0, 0.1) );
	ofSetColor(0);
	ofDrawBitmapString( txt, 5, 20 );
}

void ofApp::keyPressed( int key ){
	
}


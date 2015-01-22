#pragma once

#include "ofMain.h"
#include "ofxAlembic.h"
#include "ad_image_saver.h"
#include "ofxGpuNoise.h"
#include "ofxExportImageSequence.h"
#include "ofxSimpleSpline.h"
#include "ofxSvg.h"

class Vine{

public:
	Vine():	radius(0), angle(0), past_top(ofVec3f(0,0,0)), age(0){}
	float radius;
	float angle;

	int age;
	int num_cv;
	ofVec3f past_top;
	vector<ofVec3f> speeds;
	vector<ofVec3f> accels;
};

class ofApp : public ofBaseApp{
    
public:
	void setup();
	void load_svg();
	void update();
	void update_particle();
	void update_spline();
	void add_particle();
	
	void draw();
	void keyPressed( int key );
	void draw_info();
	
	bool bStart;
	
	float frame;
    float rot;

    ofVboMesh rf_points;
    ofVboMesh r_lines;
	
	ofImage img;
	vector<ofxSimpleSpline> splines;
	vector<Vine> vines;
	
	ofxExportImageSequence exporter;

	static ofColor orange;
	
	ofxSVG svg;
	ofPoint win;
	
	ofVec3f start;
	ofVec3f end;

	float max_dist;
	float gAngle;
};

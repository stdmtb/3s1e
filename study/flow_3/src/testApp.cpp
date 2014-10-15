#include "testApp.h"

void testApp::setup(){
    ofSetWindowPosition( 0, 0 );
    bInit = false;
    bStart = true;
    frame = 0;
    ofSetVerticalSync( true );
	ofSetFrameRate( 60 );
	ofBackground( 0 );
	
	string path = "HY_Emitter01.abc";
	abc.open(path);
    abc.dumpNames();
    
    cam.setDistance( 5 );
    cam.setNearClip( 0.00001 );
    cam.setFarClip( 10000 );

    points.setUsage( GL_DYNAMIC_DRAW );
    points.setMode( OF_PRIMITIVE_POINTS );

    lines.setUsage( GL_DYNAMIC_DRAW );
    lines.setMode( OF_PRIMITIVE_LINES );

}

void testApp::update(){
    
    if( !bStart )
        return;
    
    frame++;
    float time =  frame * 0.0333333;
    abc.setTime( time );

    vector<ofVec3f> pos;
    abc.get( 0, pos );

    if( !bInit && pos.size()>0 ) {
        for( int i=0; i<pos.size(); i++ ){
            initial_pos.push_back( pos[i] );
        }
        bInit = true;
    }

    points.clear();
    for( int i=0; i<pos.size(); i++ ){
        ofVec3f p = pos[i]; // - initial_pos[i];
        
        points.addVertex( p );
        points.addColor( ofFloatColor( ofNoise(i, frame*0.001)-0.2, 0.5) );
    }

    if( 0 ){
        lines.clear();
        int num_p = points.getNumVertices();
        int line_num = 100;
        if( pos.size()!=0 && num_p>2 ){
            for( int i=0; i<line_num; i++ ){
                
                int index1 = floor( ofRandom( 0, num_p ));
                int index2 = floor( ofRandom( 0, num_p ));
                
                ofVec3f p1 = points.getVertex( index1 );
                ofVec3f p2 = points.getVertex( index2 );
                
                float dist = p1.distance( p2 );
                
                if( 0.01<dist && dist<0.05 ){
                    
                    lines.addVertex( p1 );
                    lines.addVertex( p2 );
                    
                    ofFloatColor c1, c2;
                    c1.white; c2.white;
                    c1.setHsb( ofRandom(1.0), 0.3, 0.6, 0.6 );
                    c2.setHsb( ofRandom(1.0), 0.3, 0.6, 0.6 );
                    lines.addColor( c1 );
                    lines.addColor( c2 );
                }
            }
        }
    }
}

void testApp::draw(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    ofBackground( 255 );
    
	cam.begin();
	glPointSize(1);
    
    ofPushMatrix();
    ofSetColor( 20 );
    glPointSize( 2 );
    points.draw( OF_MESH_POINTS );

    glLineWidth( 1 );
    lines.drawWireframe();
    ofPopMatrix();
    
    cam.end();
    
    ofSetColor( 0 );
    stringstream ss;
    ss << "frame : " << ofGetFrameNum() << "\n";
    ss << "point size: " << points.getNumVertices() << "\n";
    ss << "time : " <<  abc.getTime() << "\n";
    ofDrawBitmapString( ss.str(), 20, 20 );
}

void testApp::keyPressed( int key ){
    switch( key ) {

        case ' ':
            bStart = !bStart;
            break;
            
        case 'f':
            ofToggleFullscreen();
//            ofSetWindowShape( 1920*2, 1080*2 );
            break;
            
            
        case 'o':
            //bOrtho = !bOrtho;
            break;
            
            
        case 'I':
            //bDraw_info = !bDraw_info;
            break;
            
        case 'S':
            //saver.start( ofGetTimestampString(), "", 3000 );
            break;
            
        default:
            break;
    }
    
}

void testApp::exit(){
    abc.close();
}

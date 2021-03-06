#include "testApp.h"

void testApp::setup() {
	ofBackground( 0 );

    bStart = false;
    
	camera.setPosition(ofVec3f(0, 0, -1700.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
	
	world.setup();
    world.setGravity( ofVec3f(0,0,0) );
    world.setCamera(&camera);
    
    points.setUsage( GL_DYNAMIC_DRAW );
    lines.setUsage( GL_DYNAMIC_DRAW );
    lines.setMode( OF_PRIMITIVE_LINES );
    
    int w = 5000;
    int h = 2500;
    exporter.setup( w, h, 25, GL_RGB, 0);
    exporter.setFilePattern( ofGetTimestampString() + "/F_%05i.png");
    exporter.setFrameRange( 0, 3000 );
    ofSetWindowShape( w/2, h/2 );
    ofSetWindowPosition(0, 0);
    
    int n = 1500;
    for( int d=0; d<2; d++ ){
        for( int i=0; i<n; i++ ){
            
            int index = i + n*d;
            
            float mass = ofNoise(index)*3.0 + 0.5;
            float size = 1 + ofNoise( mass * 0.1) * 2.0;
            
            ofVec3f pos(i - n/2, 0, 0 );
            pos *= 2.0;
            pos.y = ofRandomuf() * ofNoise(pos.x*0.01, index*0.001 ) * ((d+1)*150) ;
            pos.y += 10.0;
            pos.x += ofSignedNoise(pos.x*0.1, i*0.011, index*0.03) * 100;
//            pos.rotate(ofRandomf()*360, ofVec3f(1,0,0) );
            
            // Sphere
            shapes.push_back( new ofxBulletSphere() );
            ((ofxBulletSphere*)shapes[index])->create( world.world, pos, mass, size );
            shapes[index]->add(1,0);
            points.addVertex( pos );
            {
                ofFloatColor col(1, 0, 0, ofNoise(pos.x*0.1, index*0.01)*0.7+0.4);
                col.setBrightness( ofNoise(pos.x*0.2, pos.y*0.02, index*0.024)*0.4 + 0.3);
                col.setSaturation( ofNoise(pos.x*0.1, pos.y*0.01, index*0.012)*0.4 + 0.2);
                col.setHue( ofNoise(pos.x*0.4, pos.y*0.01, index*0.006)*0.45 + 0.4);
                points.addColor( col );
            }
            
            // Constraint
            lines.addVertex( pos ); // i*2
            pos.y = 0;
            lines.addVertex( pos ); // i*2+1
            {
                ofFloatColor col(1, 0, 0, ofNoise(pos.x*0.1, index*0.01)*0.4+0.3);
                col.setBrightness( ofNoise(pos.x*0.2, pos.y*0.02, index*0.024)*0.4 + 0.3);
                col.setSaturation( ofNoise(pos.x*0.1, pos.y*0.01, index*0.012)*0.9 + 0.2);
                col.setHue( ofNoise(pos.x*0.4, pos.y*0.01, index*0.006)*0.4 + 0.32);
                lines.addColor( col );
                col.setBrightness(col.getBrightness()*0.7 );
                lines.addColor( col );
            }
            
            if( 0 ){
                p2ps.push_back( new ofxBulletP2PConstraint() );
                p2ps[index]->create( world.world, *shapes[index], pos );
                p2ps[index]->add();
            }else{
                joints.push_back( new ofxBulletJoint() );
                joints[index]->create( world.world, shapes[index], pos );
                joints[index]->add();
            }

            
            // Attractor
            if( d == 0 && index%2== 0){
                pos.y -= ofNoise(i*0.1)*150 + ofNoise(i*0.013)*300 + 200;
                pos.x *= 1.2;
                pos.rotate( ofSignedNoise(pos.x*0.1, index*0.01)*180.0,  ofVec3f(1,0,0) );
                attrs.push_back(Attr(pos, 40000, ofNoise(index*i*0.001)*2.0 + 1) );
            }
            
        }
    }
}

void testApp::update() {
    if(!bStart ) return;
    
	world.update();

    vector<ofVec3f> &vecp = points.getVertices();
    vector<ofVec3f> &linep = lines.getVertices();
    
    // move
    for(int a=0; a<attrs.size(); a++ ){
        attrs[a].pos.rotate(attrs[a].speed, ofVec3f(1,0,0) );
    
        // atract
        for(int i=0; i<shapes.size(); i++ ){
            ofVec3f p = shapes[i]->getPosition();

            // pull
            ofVec3f dir = (attrs[a].pos - p);
            float dist2 = dir.lengthSquared();
            
            if( dist2 < 30 || 1000000 < dist2) continue;
            ofVec3f impl = dir.normalized() * (attrs[a].power / (dist2+0.0000000001) );
            shapes[i]->getRigidBody()->applyCentralImpulse( btVector3(impl.x, impl.y, impl.z) );
            // p.z = 0;
            vecp[i] = p;
            linep[i*2] = p;
        }
    }
    
//    for (int i=0; i<shapes.size(); i++) {
//        // Move Pivot
//        ofVec3f &pivot = linep[i*2 +1];
//        pivot.y -= 1;
//        joints[i]->updatePivotPos(pivot, 1);
//    }
}

void testApp::draw() {

    exporter.begin( camera );
    ofEnableAlphaBlending();
    
    
    ofBackground( 255 );
    
    glPointSize(1);
    glLineWidth(1);
    ofFill();
	if(bDrawDebug) world.drawDebug();
		
    points.drawVertices();
    lines.draw();
    
    if( bDrawDebug ){
        for( int i=0; i<attrs.size(); i++){
            attrs[i].draw();
        }
    }
	
    exporter.end();
    
    ofBackground(0);
    ofSetColor(255);
    exporter.draw(0, 0);
    
	ofSetColor( 0 );
	stringstream ss;
	ss << "fps            : " << ofGetFrameRate() << endl;
    ss << "export frame   : " << exporter.getFrameNum() << endl;
    ss << "num shapes     : " << (shapes.size()) << endl;
	ofDrawBitmapString(ss.str().c_str(), 40, 40);
}

void testApp::keyPressed(int key) {
	switch (key) {

        case ' ':
            bStart = !bStart;
            break;
            
        case 'S':
            bStart = true;
            exporter.startExport();
            ofSetWindowShape(exporter.getFbo().getWidth()/4, exporter.getFbo().getHeight()/4);
            ofSetWindowPosition(0, 0);
            break;
            
        case 'd':
			bDrawDebug = !bDrawDebug;
			break;
		default:
			break;
	}
}

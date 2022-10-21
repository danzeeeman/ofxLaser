#pragma once

#include "ofMain.h"
#include "ofxLaserManager.h"

class Particle {
public:
	Particle() {

	};
	~Particle() {

	}
	void setup(ofVec2f start, float force, float width, float height, float acc, float circle) {
		dir = (start - ofVec2f(width / 2, height / 2)).normalize();
		pos = start;
		vel = force;
		accel = acc;
		laser_width = width;
		laser_height = height;
		dead = false;
		c = ofColor(255, 255, 255);
		circle_width = circle;
	};
	ofVec2f update() {
		if (!dead) {
			circle_width *= 0.999;
			if (circle_width < 5) {
				circle_width = 5.0;
			}
			pos += dir * vel;
			dist = (pos - ofVec2f(laser_width / 2, laser_height / 2)).length();
			if(dist >= laser_width) {
				dead = true;
			}
			vel *= accel;
		}
		return pos;
	};

	float getDist() {
		return dist/laser_width;
	}

	bool isDead() {
		return dead;
	}
	ofColor getColor() {
		return c;
	}
	float getCircleWidth() {
		return circle_width;
	}
private:
	float circle_width;
	float dist;
	bool dead;
	float laser_width;
	float laser_height;
	float accel;
	float vel;
	ofVec2f dir;
	ofVec2f pos;
	int index;
	ofColor c;
};


class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(ofKeyEventArgs& e);
	void mouseDragged(ofMouseEventArgs& e);
	void mousePressed(ofMouseEventArgs& e);
	void mouseReleased(ofMouseEventArgs& e);
	
	void showLaserEffect(int effectnum);
	
	ofParameter<int> currentLaserEffect;
    ofParameter<float>timeSpeed;
    ofParameter<int> renderProfileIndex;
    ofParameter<string> renderProfileLabel;
    
    int numLaserEffects;
    float elapsedTime;
    
	ofxLaser::Manager laserManager;
	
	bool drawingShape = false;
	int laserWidth;
	int laserHeight;
	float startTime;
	float last_add;
	float cut_off;
    
	std::vector<ofPolyline> polyLines;
	vector<ofxLaser::Graphic> laserGraphics;
	
    ofParameter<ofColor> colour;

	vector<Particle> particles;
	vector<Particle> circles;
};




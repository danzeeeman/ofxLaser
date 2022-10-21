#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup() {

	// get the filenames of all the svgs in the data/svgs folder
	string path = "svgs/";
	ofDirectory dir(path);
	dir.allowExt("svg");
	dir.listDir();
	dir.sort();

	// and load them all
	const vector<ofFile>& files = dir.getFiles();

	dir.close();

	//svgs.resize(files.size());

	for (int i = 0; i < files.size(); i++) {
		const ofFile& file = files.at(i);

		laserGraphics.emplace_back();

		// addSvgFromFile(string filename, bool optimise, bool subtractFills)
		laserGraphics.back().addSvgFromFile(file.getAbsolutePath(), false, true);

		// this centres all the graphics
		laserGraphics.back().autoCentre();

		ofLog(OF_LOG_NOTICE, file.getAbsolutePath());
	}

	laserWidth = 800;
	laserHeight = 800;
	laserManager.setCanvasSize(laserWidth, laserHeight);

	numLaserEffects = 9;

	// if you don't want to manage your own GUI for your
	// app you can add extra params to the laser GUI
	laserManager.addCustomParameter(renderProfileLabel);
	laserManager.addCustomParameter(renderProfileIndex.set("Render Profile", 0, 0, 2));

	laserManager.addCustomParameter(currentLaserEffect.set("Current effect", 0, 0, numLaserEffects - 1));
	laserManager.addCustomParameter(timeSpeed.set("Animation speed", 1, 0, 2));

	laserManager.addCustomParameter(colour.set("Colour", ofColor(0, 255, 0), ofColor(0), ofColor(255)));

	ofParameter<string> description;
	description.setName("description");
	description.set("INSTRUCTIONS : \nLeft and Right Arrows to change current effect \nMouse to draw polylines \nC to clear");
	laserManager.addCustomParameter(description);

	currentLaserEffect = 0;

	//if (particles.size() < 50) {
		// LASER PARTICLES ANIMATING
	int random = 2 + floor(ofRandom(3)) + floor(ofRandom(33));
	for (int i = 0; i < random; i++) {
		Particle p;
		ofVec2f start;
		float r = ofNoise((elapsedTime * 0.0123 * i) * 0.001, 0.001, 0.0001, 0.00001) * 25;
		start.x = sin(ofMap(i, 0, random, 0, TWO_PI) + elapsedTime + i) * r + laserWidth / 2;
		start.y = cos(ofMap(i, 0, random, 0, TWO_PI) + elapsedTime + i) * r + laserHeight / 2;
		p.setup(start, ofNoise((elapsedTime + i) * 0.01, 0.001, 0.0001, 0.00001) * 1.15, laserWidth, laserHeight, 1, 10);
		particles.push_back(p);
	}
	//}
cut_off = ofRandom(4);
}

//--------------------------------------------------------------
void ofApp::update() {

	float deltaTime = ofClamp(ofGetLastFrameTime(), 0, 0.2);
	elapsedTime += (deltaTime * timeSpeed);

	// prepares laser manager to receive new points
	laserManager.update();

}


void ofApp::draw() {

	ofBackground(15, 15, 20);

	showLaserEffect(currentLaserEffect);

	// sends points to the DAC
	laserManager.send();

	laserManager.drawUI();


}


void ofApp::showLaserEffect(int effectnum) {

	string renderProfile;
	switch (renderProfileIndex) {
	case 0:
		renderProfile = OFXLASER_PROFILE_DEFAULT;
		break;
	case 1:
		renderProfile = OFXLASER_PROFILE_DETAIL;
		break;
	case 2:
		renderProfile = OFXLASER_PROFILE_FAST;
		break;
	}
	renderProfileLabel = "Render Profile : OFXLASER_PROFILE_" + renderProfile;


	float left = laserWidth * 0.1;
	float top = laserHeight * 0.1;
	float right = laserWidth * 0.9;
	float bottom = laserHeight * 0.9;
	float width = laserWidth * 0.8;
	float height = laserHeight * 0.8;



	float speed = 1;
	vector<int> index;
	int i = 0;
	for (auto& p : circles) {
		ofColor c;
		ofVec2f pt = p.update();
		ofPoint point = ofPoint(pt.x, pt.y);
		c = p.getColor();
		if (p.isDead()) {
			index.push_back(i);
		}
		else {
			laserManager.drawCircle(point, p.getCircleWidth(), c, renderProfile);
		}

		i++;
	}
	bool add_more = false;
	for (int i = index.size() - 1; i >= 0; i--) {
		circles.erase(circles.begin() + index[i]);
	}

	index.clear();
	i = 0;
	for (auto& p : particles) {
		ofColor c;
		ofVec2f pt = p.update();
		ofPoint point = ofPoint(pt.x, pt.y);
		c = p.getColor();
		if (p.isDead()) {
			index.push_back(i);
		}
		else {
			laserManager.drawDot(point, c, 1, renderProfile);
		}

		i++;
	}

	for (int i = index.size() - 1; i >= 0; i--) {
		particles.erase(particles.begin() + index[i]);
	}
	bool add = false;
	if ((elapsedTime - last_add) > cut_off) {
		cut_off = 0.125/2 + ofRandom(1.5);
		last_add = elapsedTime;
		add = true;
	}
	if (circles.size() < 15 && add) {
		
		float rate = ofNoise((elapsedTime) * 0.001, 0.001, 0.0001, 0.00001);
		//for (int j = 0; j < rates.size(); j++) {
			//int random = floor(ofRandom(12));
			//float r = ofNoise((elapsedTime) * 0.01, 0.001, 0.0001, 0.00001) * 20;
			//for (int i = 0; i < random; i++) {
			//	Particle p;
			//	ofVec2f start;
			//	float r = ofNoise((elapsedTime * 0.0123) * 0.001, 0.001, 0.0001, 0.00001) * 25;
			//	start.x = sin(ofMap(i, 0, random, 0, TWO_PI)) * r + laserWidth / 2;
			//	start.y = cos(ofMap(i, 0, random, 0, TWO_PI)) * r + laserHeight / 2;
			//	float accel = ofMap(ofNoise((elapsedTime * 0.3345) * 0.01, 0.001, 0.0001, 0.00001), 0, 1, 1, 1.01);
			//	p.setup(start, ofNoise((elapsedTime*0.542) * 0.01, 0.001, 0.0001, 0.00001) * 1.15, laserWidth, laserHeight, accel, laserWidth/30);
			//	circles.push_back(p);
			//}
		//}
	}
	if (particles.size() < 299 && add) {
		int random = floor(ofRandom(23));
		for (int i = 0; i < random; i++) {
			Particle p;
			ofVec2f start;
			float r = ofNoise((elapsedTime) * 0.001, 0.001, 0.0001, 0.00001) * 15 ;
			start.x = sin(ofMap(i, 0, random, 0, TWO_PI) + elapsedTime + i) * r + laserWidth / 2;
			start.y = cos(ofMap(i, 0, random, 0, TWO_PI) + elapsedTime + i) * r + laserHeight / 2;
			float accel = ofMap(ofNoise((elapsedTime) * 0.01, 0.001, 0.0001, 0.00001), 0, 1, 1, 1.01);
			p.setup(start, ofNoise((elapsedTime+i) * 0.01, 0.001, 0.0001, 0.00001) * 1.15, laserWidth, laserHeight, accel, laserWidth /30);
			particles.push_back(p);
		}
	}

	if (elapsedTime - startTime < startTime + 10) {
		laserManager.beginDraw();

		ofPushMatrix();

		ofTranslate(laserWidth / 2, laserHeight / 2);
		ofScale(0.35, 0.35);
		laserManager.drawLaserGraphic(laserGraphics[0], 1, renderProfile);
		ofPopMatrix();

		laserManager.endDraw();
	}

	if (elapsedTime - startTime > startTime + 25) {
		startTime = elapsedTime;
	}

}


//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs& e) {
	if (e.key == OF_KEY_TAB) {
		laserManager.selectNextLaser();
	}

}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs& e) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs& e) {

}

void ofApp::mouseReleased(ofMouseEventArgs& e) {

}

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	this->number_of_belts = 2;
	for (int i = 0; i < this->number_of_belts; i++) {

		this->noise_seed_list.push_back(ofRandom(1000));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	for (int i = 0; i < this->number_of_belts; i++) {

		auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		for(int k = 0; k < 700; k++){

			int deg = ofRandom(360);

			auto angle_x = ofMap(ofNoise(noise_seed.x, cos(deg * DEG_TO_RAD) * 0.2, this->noise_seed_list[i]), 0, 1, -PI, PI);
			auto angle_y = ofMap(ofNoise(noise_seed.y, sin(deg * DEG_TO_RAD) * 0.2, this->noise_seed_list[i]), 0, 1, -PI, PI);
			auto angle_z = ofMap(ofNoise(noise_seed.z, this->noise_seed_list[i]), 0, 1, -PI, PI);

			auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

			auto location = glm::vec3(280 * cos(deg * DEG_TO_RAD), 280 * sin(deg * DEG_TO_RAD), ofRandom(-50, 50));
			location = glm::vec4(location, 0) * rotation_z * rotation_y * rotation_x;

			this->face.addVertex(location);
			this->frame.addVertex(location);

			this->face.addColor(ofColor(39, 32));
			this->frame.addColor(ofColor(39));
		}
	}

	for (int i = 0; i < this->face.getNumVertices(); i++) {

		vector<int> near_index_list;
		auto location = this->face.getVertex(i);
		for (int k = 0; k < this->face.getNumVertices(); k++) {

			auto other = this->face.getVertex(k);
			auto distance = glm::distance(location, other);

			if (distance < 30) {

				near_index_list.push_back(k);
			}
		}

		if (near_index_list.size() > 3) {

			for (int k = 0; k < near_index_list.size() - 2; k++) {

				this->face.addIndex(i);
				this->face.addIndex(near_index_list[k]);
				this->face.addIndex(near_index_list[k + 1]);

				this->frame.addIndex(i); this->frame.addIndex(near_index_list[k]);
				this->frame.addIndex(i); this->frame.addIndex(near_index_list[k + 1]);
			}
		}
	}

	auto frame = 60;
	auto param = ofGetFrameNum() % (frame * this->number_of_belts);
	auto index = param / frame;

	for (int i = 0; i < this->number_of_belts; i++) {
	
		if (i == index) {
			
			auto f = ofGetFrameNum() % frame;
			if (f > frame * 0.5) {

				this->noise_seed_list[index] += ofMap(f, frame * 0.5, frame, 0.03, 0.0005);
			}
			else {

				this->noise_seed_list[index] += ofMap(f, 0, frame * 0.5, 0.0005, 0.03);
			}
		}
		else {

			this->noise_seed_list[i] += 0.0005;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.5);
	ofRotateX(ofGetFrameNum() * 0.75);

	this->face.draw();
	this->frame.drawWireframe();

	for (auto& vertex : this->face.getVertices()) {

		ofDrawSphere(vertex, 3);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {


	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
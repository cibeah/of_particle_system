#pragma once
#if (_MSC_VER >= 1915)
#define no_init_all deprecated
#endif

#include "ofMain.h"
#include "ParticlesSystem.h"
#include "Particle.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ParticlesSystem ps;
		ofColor color1;
		ofColor color2;

		int timer;
		int numel;
};

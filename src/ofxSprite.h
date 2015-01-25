//
//  ofxSprite.h
//  AnimationBreaker
//
//  Created by Gal Sasson on 1/19/15.
//
//

#ifndef __AnimationBreaker__ofxSprite__
#define __AnimationBreaker__ofxSprite__

#include <stdio.h>
#include "ofMain.h"

struct SpriteTexCoords
{
	ofVec2f val[4];
};

class ofxSprite
{
public:

	ofxSprite();

	enum LoopType {
		LOOP_NONE,
		LOOP_NORMAL,
		LOOP_PINGPONG
	};

	enum Direction {
		FORWARD,
		BACKWARD
	};

	void setup(ofTexture* tex, float spriteWidth, float spriteHeight, int numFrames);

	void play();
	void stop();

	void setFrame(int index);
	int getFrame() { return currentFrame; }

	void setStartFrame(int frame);
	int getStartFrame() { return startFrame; }
	void setEndFrame(int frame);
	int getEndFrame() { return endFrame; }

	void setLoopType(LoopType type) { loopType = type; }
	LoopType getLoopType() { return loopType; }
	void setDirection(Direction dir) { direction = dir; }
	Direction getDirection() { return direction; }

	void setFrameHold(int frames) { frameHold = frames; }
	int getFrameHold() { return frameHold; }

	void update();
	void draw(float x=0, float y=0);

private:
	bool bInitialized;
	
	ofTexture* sheetTex;
	ofVec2f spriteSize;
	LoopType loopType;
	Direction direction;
	bool bAnimating;

	int nFrames;
	int previousFrame;
	int currentFrame;
	int frameHold;

	int startFrame;
	int endFrame;

	int frameHoldCounter;

	ofVbo vbo;
	void setupSpriteCoords();
	map<int, SpriteTexCoords> spriteCoords;

	void initVbo();
	void advanceFrame();
};
#endif /* defined(__AnimationBreaker__ofxSprite__) */

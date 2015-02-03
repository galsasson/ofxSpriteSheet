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


class ofxSprite : public ofNode
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

	void setAnchor(float x, float y);

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

	void setTintColot(const ofColor& c) { tintColor = c; }
	ofColor getTintColor() { return tintColor; }

	void update();
	void draw(float x=0, float y=0);

	// use this together with bind/unbind to draw many sprites with the same texture
	void drawNoBind(float x=0, float y=0);

	// use this to draw without push/pop matrix

	// use this together with drawNoBind to draw many sprites with the same texture
	void bindTex();
	void unbindTex();

private:
	bool bInitialized;
	
	ofTexture* sheetTex;
	ofVec2f spriteSize;
	LoopType loopType;
	Direction direction;
	ofColor tintColor;
	bool bAnimating;

	ofVec2f anchor;

	int nFrames;
	int previousFrame;
	int currentFrame;
	int frameHold;

	int startFrame;
	int endFrame;

	int frameHoldCounter;

	void setupSpriteVbo();
	ofVbo vbo;

	void initVbo();
	void advanceFrame();
};
#endif /* defined(__AnimationBreaker__ofxSprite__) */

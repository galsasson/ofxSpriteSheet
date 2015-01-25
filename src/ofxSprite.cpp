//
//  ofxSprite.cpp
//  AnimationBreaker
//
//  Created by Gal Sasson on 1/19/15.
//
//

#include "ofxSprite.h"

ofxSprite::ofxSprite()
{
	bInitialized = false;
}

void ofxSprite::setup(ofTexture* tex, float spriteWidth, float spriteHeight, int numFrames)
{
	sheetTex = tex;
	ofEnableArbTex();
	spriteSize = ofVec2f(spriteWidth, spriteHeight);
	loopType = LOOP_NORMAL;
	direction = FORWARD;
	bAnimating = false;

	frameHold = 2;
	nFrames = numFrames;

	previousFrame = currentFrame = 0;
	startFrame = 0;
	endFrame = nFrames-1;

	setupSpriteCoords();

	bInitialized = true;
}

void ofxSprite::play()
{
	bAnimating = true;

	setFrame(startFrame);
}

void ofxSprite::stop()
{
	bAnimating = false;
}

void ofxSprite::setFrame(int index)
{
	previousFrame = currentFrame;
	currentFrame = index;

	frameHoldCounter = frameHold;
}

void ofxSprite::setStartFrame(int frame)
{
	if (frame < 0 ||
		frame >= endFrame) {
		ofLogError("ofxSprite::setStartFrame", "invalid start frame %d", frame);
		return;
	}

	startFrame = frame;
}

void ofxSprite::setEndFrame(int frame)
{
	if (frame <= startFrame ||
		frame >= nFrames) {
		ofLogError("ofxSprite::setEndFrame", "invalid end frame %d", frame);
		return;
	}

	endFrame = frame;
}


void ofxSprite::update()
{
	if (!bAnimating) {
		return;
	}

	frameHoldCounter--;
	if (frameHoldCounter==0) {
		advanceFrame();
	}
}

void ofxSprite::draw(float x, float y)
{
	if (!bInitialized) {
		return;
	}
	
	ofPushMatrix();
	ofTranslate(x, y);

	SpriteTexCoords& coords = spriteCoords[currentFrame];

	ofSetColor(255);

	sheetTex->bind();

	glBegin(GL_QUADS);
	glTexCoord2f(coords.val[0].x, coords.val[0].y);
	glVertex2f(0, 0);
	glTexCoord2f(coords.val[1].x, coords.val[1].y);
	glVertex2f(spriteSize.x, 0);
	glTexCoord2f(coords.val[2].x, coords.val[2].y);
	glVertex2f(spriteSize.x, spriteSize.y);
	glTexCoord2f(coords.val[3].x, coords.val[3].y);
	glVertex2f(0, spriteSize.y);
	glEnd();

	sheetTex->unbind();

	ofPopMatrix();
}

void ofxSprite::setupSpriteCoords()
{
	ofVec2f origin(0, 0);

	for (int i=0; i<nFrames; i++) {
		SpriteTexCoords coords;
		coords.val[0] = origin;
		coords.val[1] = origin + ofVec2f(spriteSize.x, 0);
		coords.val[2] = origin + ofVec2f(spriteSize.x, spriteSize.y);
		coords.val[3] = origin + ofVec2f(0, spriteSize.y);
		spriteCoords[i] = coords;

		// advance origin
		origin.x += spriteSize.x;
		if (origin.x + spriteSize.x > sheetTex->getWidth()) {
			origin.y += spriteSize.y;
			origin.x = 0;
		}
	}
}

void ofxSprite::advanceFrame()
{
	if (direction == FORWARD) {

		if (currentFrame == endFrame) {

			if (loopType == LOOP_NONE) {

				stop();
			}
			else if (loopType == LOOP_NORMAL) {

				setFrame(startFrame);
			}
			else if (loopType == LOOP_PINGPONG) {

				direction = BACKWARD;
				setFrame(currentFrame-1);
			}
		}
		else {
			setFrame(currentFrame+1);
		}
	}
	else {

		if (currentFrame == startFrame) {

			if (loopType == LOOP_NONE) {

				stop();
			}
			else if (loopType == LOOP_NORMAL) {

				setFrame(endFrame);
			}
			else if (loopType == LOOP_PINGPONG) {

				direction = FORWARD;
				setFrame(currentFrame+1);
			}
		}
		else {
			setFrame(currentFrame-1);
		}
	}
}




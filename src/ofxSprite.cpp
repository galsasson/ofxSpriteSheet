//
//  ofxSprite.cpp
//  AnimationBreaker
//
//  Created by Gal Sasson on 1/19/15.
//
//

#include "ofxSprite.h"

#define USE_TRIANGLE_STRIP

ofxSprite::ofxSprite()
{
	bInitialized = false;
}

void ofxSprite::setup(ofTexture* tex, float spriteWidth, float spriteHeight, int numFrames)
{
	sheetTex = tex;
	spriteSize = ofVec2f(spriteWidth, spriteHeight);
	loopType = LOOP_NORMAL;
	direction = FORWARD;
	tintColor = ofColor(255);
	bAnimating = false;

	frameHold = 2;
	nFrames = numFrames;

	previousFrame = currentFrame = 0;
	startFrame = 0;
	endFrame = nFrames-1;

	setupSpriteVbo();

	bInitialized = true;
}

void ofxSprite::setAnchor(float x, float y)
{
	anchor = ofVec2f(x, y);
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
	
	sheetTex->bind();

	drawNoBind();

	sheetTex->unbind();
}

void ofxSprite::drawNoBind(float x, float y)
{
	if (!bInitialized) {
		return;
	}

	ofPushMatrix();
	ofTranslate(x, y);

	ofMultMatrix(getGlobalTransformMatrix());

	ofTranslate(-anchor);

	ofSetColor(tintColor);

#ifdef USE_TRIANGLE_STRIP
	vbo.draw(GL_TRIANGLE_STRIP, currentFrame*4, 4);
#else
	vbo.draw(GL_TRIANGLES, currentFrame*6, 6);
#endif

	ofPopMatrix();
}

void ofxSprite::bindTex()
{
	if (sheetTex) {
		sheetTex->bind();
	}
}

void ofxSprite::unbindTex()
{
	if (sheetTex) {
		sheetTex->unbind();
	}
}

void ofxSprite::setupSpriteVbo()
{
	vector<ofVec3f> verts;
	vector<ofVec2f> texCoords;

	float visibleSize = 1;

	ofVec2f tlVec(0, 0);
	ofVec2f trVec(spriteSize.x*0.95f, 0);
	ofVec2f blVec(0, spriteSize.y*0.95f);
	ofVec2f brVec(spriteSize.x*0.95f, spriteSize.y*0.95f);

	ofVec2f origin(0, 0);
	for (int i=0; i<nFrames; i++)
	{
#ifdef USE_TRIANGLE_STRIP
		// first triangle
		verts.push_back(ofVec2f(0, 0));
		texCoords.push_back(origin);

		verts.push_back(ofVec2f(visibleSize, 0));
		texCoords.push_back(origin + trVec);

		verts.push_back(ofVec2f(0, visibleSize));
		texCoords.push_back(origin + blVec);

		verts.push_back(ofVec2f(visibleSize, visibleSize));
		texCoords.push_back(origin + brVec);


#else								// GL_TRIANGLES
		// first triangle
		verts.push_back(ofVec2f(0, 0));
		texCoords.push_back(origin);

		verts.push_back(ofVec2f(visibleSize, 0));
		texCoords.push_back(origin + trVec);

		verts.push_back(ofVec2f(visibleSize, visibleSize));
		texCoords.push_back(origin + brVec);



		// second triangle
		verts.push_back(ofVec2f(visibleSize, visibleSize));
		texCoords.push_back(origin + brVec);

		verts.push_back(ofVec2f(0, visibleSize));
		texCoords.push_back(origin + blVec);

		verts.push_back(ofVec2f(0, 0));
		texCoords.push_back(origin);
#endif


		// advance origin
		origin.x += spriteSize.x;
#ifdef TARGET_OPENGLES
		if (origin.x + spriteSize.x > 1) { 		// normalized texture coords
#else
		if (origin.x + spriteSize.x > sheetTex->getWidth()) {
#endif
			origin.y += spriteSize.y;
			origin.x = 0;
		}
	}

	vbo.setVertexData(&verts[0], verts.size(), GL_STATIC_DRAW);
	vbo.setTexCoordData(&texCoords[0], texCoords.size(), GL_STATIC_DRAW);
}


void ofxSprite::advanceFrame()
{
	if (direction == FORWARD) {

		if (currentFrame == endFrame) {

			switch (loopType) {

				case LOOP_NONE:
					stop();
					break;

				case LOOP_NORMAL:
					setFrame(startFrame);
					break;

				case LOOP_PINGPONG:
					direction = BACKWARD;
					setFrame(currentFrame-1);
					break;
			}
		}
		else {
			setFrame(currentFrame+1);
		}
	}
	else {

		if (currentFrame == startFrame) {

			switch (loopType) {
				case LOOP_NONE:
					stop();
					break;

				case LOOP_NORMAL:
					setFrame(endFrame);
					break;

				case LOOP_PINGPONG:
					direction = FORWARD;
					setFrame(currentFrame+1);
					break;
			}
		}
		else {
			setFrame(currentFrame-1);
		}
	}
}




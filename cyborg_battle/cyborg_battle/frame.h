#ifndef FRAME
#define FRAME

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "drawing_functions.h"
#include "globals.h"
#include "groupBuilder.h"

using namespace std;

class Frame {
public:
	int frameNumber; // or frame index
	SDL_Rect clip; // it's the region on the spritesheet where this frame is 
	float duration; //How long does the frame run for
	SDL_Point offset; // pivot point to help align frames in an animation

	list<Group*> frameData; // holds groups of additionnal data for the frame

	void Draw(SDL_Texture* spriteSheet, float x, float y);

	// TODO loadframe
};

#endif

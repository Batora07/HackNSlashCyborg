#ifndef KEYBOARDINPUT
#define KEYBOARDINPUT

#include "hero.h"

class KeyboardInput {
public:
	// to which hero I look after
	Hero* hero;
	// different keys on your keyboard
	SDL_Scancode UP, DOWN, LEFT, RIGHT;
	SDL_Scancode SLASH, DASH;

	KeyboardInput();
	void update(SDL_Event* e);
};

#endif
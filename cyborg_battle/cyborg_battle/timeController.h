#ifndef TIMECONTROLLER
#define TIMECONTROLLER

#include <iostream>
#include "SDL.h"

class TimeController {
public:
	// reference values
	static const int PLAY_STATE, PAUSE_STATE;

	int timeState;
	Uint32 lastUpdate;  // last time we checked how many ticks we were up to
	float dT; // delta time = amount of time since the last frame was rendered onto screen
	
	TimeController();
	void updateTime(); // Update lastUpdate and dT
	void pause();
	void resume();
	void reset();

	static TimeController timeController;
};

#endif

#ifndef CAMERACONTROLLER
#define CAMERACONTROLLER

#include "globals.h"
#include "entity.h"

class CameraController {
public:
	Entity* target = NULL;
	float lerp = 2.6f;  // how fast it moves around the target

	void update();
};

#endif
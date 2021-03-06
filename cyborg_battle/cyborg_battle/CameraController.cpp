#include "CameraController.h"

void CameraController::update() {
	if (target != NULL) {
		// great, we're following someone
		float targetX = target->x - Globals::camera.w / 2;
		float targetY = target->y - Globals::camera.h / 2;

		// MOVE TO TARGET POSITION WITH LERP
		Globals::camera.x += ((targetX) - Globals::camera.x) * lerp * TimeController::timeController.dT;
		Globals::camera.y += ((targetY) - Globals::camera.y) * lerp * TimeController::timeController.dT;
	}
	//  else don't follow
}
#ifndef WALL
#define WALL

#include "entity.h"

class Wall : public Entity {
public:
	Wall(AnimationSet *animSet);
	void update();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	// don't do anything, Walls don't move or get pushed in this gaame
	void updateCollision();
};

#endif
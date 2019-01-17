#ifndef BULLET
#define BULLET

#include <cstdlib>
#include "livingEntity.h" // we're using this to make damages with the bullet
#include "soundManager.h"

class Bullet : public LivingEntity {
public :
	static const string BULLET_ANIM;

	Bullet(AnimationSet* animSet, int x, int y);
	void update();
	void die() {};  // need this becaus we inherit from living entity class
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateAnimation();
	void updateDamages() { ; } // need this becaus we inherit from living entity class
	void hitLanded(LivingEntity* entity);
	virtual void crashOntoSolid();
};

#endif
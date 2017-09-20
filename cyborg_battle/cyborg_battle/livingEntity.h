#ifndef LIVINGENTITY
#define LIVINGENTITY

#include "entity.h"

//abstract class also
class LivingEntity : public Entity {
public:
	int hp, hpMax;
	int damage = 0;
	// similar to collisionbox but describes damaging area
	SDL_Rect hitBox;
	// if > 0, then cant be hurt 
	float invincibleTimer = 0;

	virtual void updateHitBox();
	// how we get damages by other things (up to each class to define)
	virtual void updateDamages() = 0;
	// abstract, up to other classes to decide how they die
	virtual void die() = 0;
	virtual void updateInvincibleTimer();

	// overriding entities draw
	void draw();
}

#endif
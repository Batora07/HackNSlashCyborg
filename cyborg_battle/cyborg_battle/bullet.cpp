#include "bullet.h"

const string BULLET_ANIM = "bullet";

Bullet::Bullet(AnimationSet* animSet, int x, int y) {
	this->animSet = animSet;

	solid = false;
	collidedWithSolids = true;
	dieOnSolids = true;
	moving = true;
	type = "enemy";

	this->x = x;
	this->y = y;
	moveSpeed = 40;
	moveSpeedMax = 40;
	hp = hpMax = 1;
	damage = 5;
	collisionBoxW = 5;
	collisionBoxH = 5;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
	collisionBoxYOffset = 0;  // centered around the (0,0) mark

	direction = DIR_DOWN;

	changeAnimation(0, true);

	updateCollisionBox();

	invincibleTimer = 0;
}

void Bullet::update() {
	updateCollisionBox();

	move(angle);
	updateMovement();

	updateCollisions();

	updateHitBox();
	updateDamages();

	updateAnimation();
}

void Bullet::changeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;
	currentAnim = animSet->getAnimation(BULLET_ANIM);

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}

void Bullet::updateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL) 
		return;

	//if got frames, update frame timer and animation
	frameTimer += TimeController::timeController.dT;

	if (frameTimer >= currentFrame->duration) {
		//if it's the end of the animation, restart it
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()) 
			currentFrame = currentAnim->getFrame(0);
		else 
			currentFrame = currentAnim->getNextFrame(currentFrame);

		frameTimer = 0;
	}
}

void Bullet::hitLanded(LivingEntity* entity) {
	// we crashed into an entity and damaged them, time to destroy bullet
	active = false;
	// TODO : create hit effect
}

void Bullet::crashOntoSolid() {
	//for bullets, when they hit walls, destroy them
	active = false;
	//TODO : create a hit effect
}
#include "roundKing.h"

const string ROUND_KING_ANIM_IDLE = "idle";
const string ROUND_KING_ANIM_CHARGE = "charge";
const string ROUND_KING_ANIM_SHOOT = "shoot";
const string ROUND_KING_ANIM_SLAM = "slam";
const string ROUND_KING_ANIM_JUMP_TELEGRAPH = "jumpTelegraph";
const string ROUND_KING_ANIM_JUMP = "jump";
const string ROUND_KING_ANIM_DIE = "die";

const int ROUND_KING_STATE_IDLE = 1;
const int ROUND_KING_STATE_CHARGE = 2;
const int ROUND_KING_STATE_SHOOT = 3;
const int ROUND_KING_STATE_SLAM = 4;
const int ROUND_KING_STATE_JUMP_TELEGRAPH = 5;
const int ROUND_KING_STATE_JUMP = 6;
const int ROUND_KING_STATE_DEAD = 7;

const int ROUND_KING_PHASE_NORMAL = 1;
const int ROUND_KING_PHASE_DAMAGED = 2;
const int ROUND_KING_PHASE_FRANTIC = 3;

RoundKing::RoundKing(AnimationSet* animSet, AnimationSet* bulletAnimSet) {
	this->animSet = animSet;
	this->bulletAnimSet = bulletAnimSet;

	type = "enemy";

	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 500;
	damage = 0;
	collisionBoxW = 50;
	collisionBoxH = 30;

	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;

	collisionBoxYOffset = -14;

	changeAnimation(ROUND_KING_STATE_IDLE, true);

	updateCollisionBox();
}

void RoundKing::update() {
	//check if dead
	if (hp < 1 && state != ROUND_KING_STATE_DEAD) {
		changeAnimation(ROUND_KING_STATE_DEAD, true);
		moving = false;
		die();
	}

	// ai thinks what to do
	think();

	//update our shooting stuff
	updateShoot();

	updateCollisionBox();
	updateMovement();
	updateCollisions(); 

	updateHitBox();
	updateDamages();

	updateAnimation();

	updateInvincibleTimer();
}

void RoundKing::updateShoot() {
	if (state == ROUND_KING_STATE_SHOOT) {
		shootTimer -= TimeController::timeController.dT;
		bulletTimer -= TimeController::timeController.dT;

		//if shooting time is over, stop shooting
		if (shootTimer <= 0) {
			changeAnimation(ROUND_KING_STATE_IDLE, true);
		}
		else if(bulletTimer <=0) {   //otherwise if still shootig and it's time to take a shot
			bulletTimer = 0.5;

			Bullet* bullet = new Bullet(bulletAnimSet, x, y);
			SoundManager::soundManager.playSound("shoot");
			bullet->angle = angle;
			Entity::entities.push_back(bullet);
		}
	}
}

void RoundKing::think() {
	// finds closest hero
	findNearestTarget();

	//if there is a hero we can target, do the thinking
	if (target != NULL) {
		//only tick down think timer if in IDLE state
		if (state == ROUND_KING_STATE_IDLE) 
			thinkTimer -= TimeController::timeController.dT;
		
		//keep setting angle to point towards target
		angle = Entity::angleBetweenTwoEntities(this, target);

		// CHECK WHICH PHASE WE ARE IN
		if (hp > 250) 
			aiState = ROUND_KING_PHASE_NORMAL;
		else if (hp > 100) 
			aiState = ROUND_KING_PHASE_DAMAGED;
		else 
			aiState = ROUND_KING_PHASE_FRANTIC;
		
		// if thinkTimer is up, work out what to do next
		if (thinkTimer <= 0 && state == ROUND_KING_STATE_IDLE) {
			// reset animations, we're about to do something new
			frameTimer = 0;

			//in normal phase do :
			if (aiState == ROUND_KING_PHASE_NORMAL) {
				//reset timer
				thinkTimer = 2;

				// randomly select either slam or charge
				int action = getRandomNumber(4);
				if (action % 2 == 0) 
					slam();
				else 
					charge();
			}
			//in damaged phase do :
			else if (aiState == ROUND_KING_PHASE_DAMAGED) {
				//reset timer
				thinkTimer = 1.5f;

				// randomly select either slam, charge or jump
				int action = getRandomNumber(6);
				if (action < 2)
					slam();
				else if (action < 4)
					charge();
				else
					jumpTelegraph();
			}
			else {
				//assuming we are in frantic phase
				//reset timer
				thinkTimer = 1;

				// randomly select either charge or jump
				int action = getRandomNumber(4);
				if (action % 2 == 0)
					jumpTelegraph();
				else
					charge();
			}
		}
	}
	else {
		// targeting no one
		moving = 0;
		changeAnimation(ROUND_KING_STATE_IDLE, (state != ROUND_KING_STATE_IDLE));
	}
}

void RoundKing::charge() {

}

void RoundKing::shoot() {

}

void RoundKing::slam() {

}

void RoundKing::jumpTelegraph() {

}

void RoundKing::jump() {

}

void RoundKing::die() {

}

void RoundKing::findNearestTarget() {
}

void RoundKing::changeAnimation(int newState, bool resetFRameToBeginning) {

}

void RoundKing::updateAnimation() {

}

void RoundKing::updateDamages() {

}
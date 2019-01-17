#include "roundKing.h"

const string RoundKing::ROUND_KING_ANIM_IDLE = "idle";
const string RoundKing::ROUND_KING_ANIM_CHARGE = "charge";
const string RoundKing::ROUND_KING_ANIM_SHOOT = "shoot";
const string RoundKing::ROUND_KING_ANIM_SLAM = "slam";
const string RoundKing::ROUND_KING_ANIM_JUMP_TELEGRAPH = "jumpTelegraph";
const string RoundKing::ROUND_KING_ANIM_JUMP = "jump";
const string RoundKing::ROUND_KING_ANIM_DIE = "die";

const int RoundKing::ROUND_KING_STATE_IDLE = 1;
const int RoundKing::ROUND_KING_STATE_CHARGE = 2;
const int RoundKing::ROUND_KING_STATE_SHOOT = 3;
const int RoundKing::ROUND_KING_STATE_SLAM = 4;
const int RoundKing::ROUND_KING_STATE_JUMP_TELEGRAPH = 5;
const int RoundKing::ROUND_KING_STATE_JUMP = 6;
const int RoundKing::ROUND_KING_STATE_DEAD = 7;

const int RoundKing::ROUND_KING_PHASE_NORMAL = 1;
const int RoundKing::ROUND_KING_PHASE_DAMAGED = 2;
const int RoundKing::ROUND_KING_PHASE_FRANTIC = 3;

int RoundKing::bossKilled = 0;

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
	moving = false;
	SoundManager::soundManager.playSound("laugh");
	changeAnimation(ROUND_KING_STATE_CHARGE, true);
}

void RoundKing::shoot() {
	moving = false;

	shootTimer = 5; // 5 seconds of shooting at the player
	bulletTimer = 0; // take first shot immediately during shooting Time

	//if damaged or frantic, have a shorter shooting time
	if (aiState != ROUND_KING_PHASE_NORMAL) 
		shootTimer = 3;
	
	changeAnimation(ROUND_KING_STATE_SHOOT, true);
}

void RoundKing::slam() {
	moving = false;
	changeAnimation(ROUND_KING_STATE_SLAM, true);
}

void RoundKing::jumpTelegraph() {
	moving = false;
	frameTimer = 0;
	changeAnimation(ROUND_KING_STATE_JUMP_TELEGRAPH, true);
}

void RoundKing::jump() {
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	changeAnimation(ROUND_KING_STATE_JUMP, true);
}

void RoundKing::die() {
	moving = false;
	changeAnimation(ROUND_KING_STATE_DEAD, true);
	SoundManager::soundManager.playSound("enemyDie");

	// add to our score count
	bossKilled++;
}

void RoundKing::findNearestTarget() {
	// TODO cleaner solution than just copy/pasted code
	// suggestion : make another class to help with targeting entities and make glob, grob, and roundking inherit from it

	target = NULL;
	// find closest target
	for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
		if ((*entity)->type == "hero" && (*entity)->active) {
			// if found first hero in list then set them as the target fo now
			if (target == NULL) {
				target = (LivingEntity*)(*entity); // if can't cast to LivingEntity, throw casting exception
			}
			else {
				// otherwise, is this other hero closer than the previous target
				if (Entity::distanceBetweenTwoEntities(this, (*entity)) < Entity::distanceBetweenTwoEntities(this, target)) {
					target = (LivingEntity*)(*entity);
				}
			}
		}
	}
}

void RoundKing::changeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;

	if (state == ROUND_KING_STATE_IDLE) 
		currentAnim = animSet->getAnimation(ROUND_KING_ANIM_IDLE);
	if (state == ROUND_KING_STATE_CHARGE)
		currentAnim = animSet->getAnimation(ROUND_KING_ANIM_CHARGE);
	if (state == ROUND_KING_STATE_SHOOT)
		currentAnim = animSet->getAnimation(ROUND_KING_ANIM_SHOOT);
	if (state == ROUND_KING_STATE_SLAM)
		currentAnim = animSet->getAnimation(ROUND_KING_ANIM_SLAM);
	if (state == ROUND_KING_STATE_JUMP_TELEGRAPH)
		currentAnim = animSet->getAnimation(ROUND_KING_ANIM_JUMP_TELEGRAPH);
	if (state == ROUND_KING_STATE_JUMP)
		currentAnim = animSet->getAnimation(ROUND_KING_ANIM_JUMP);
	if (state == ROUND_KING_STATE_DEAD)
		currentAnim = animSet->getAnimation(ROUND_KING_ANIM_DIE);
	
	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}

void RoundKing::updateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL)
		return;

	frameTimer += TimeController::timeController.dT;
	//if frameTimer greater than frame duration;
	if (frameTimer >= currentFrame->duration) {
		// if at the end of animation
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()) {
			if (state == ROUND_KING_STATE_CHARGE)
				shoot();
			else if (state == ROUND_KING_STATE_JUMP_TELEGRAPH)
				jump();
			else if (state == ROUND_KING_STATE_SLAM || state == ROUND_KING_STATE_JUMP)
				changeAnimation(ROUND_KING_STATE_IDLE, true);
			else if (state == ROUND_KING_STATE_DEAD) {
				frameTimer = 0;
				// if somehow live again
				if (hp > 0) 
					changeAnimation(ROUND_KING_STATE_IDLE, true);
				else
					active = false; // get rid of him in the game loop, bye bye
			}
		}
		else {
			// not end of animation, so move to next frame normally
			currentFrame = currentAnim->getNextFrame(currentFrame);

			// IF THE NEW FRAME IS DEALING DAMAGE, we might want to make sounds
			GroupNumber* damages = (GroupNumber*)GroupBuilder::findGroupByName("damage", currentFrame->frameData);

			//if does damage
			if (damages != NULL && damages->numberOfDataInGroup() > 0) {
				// there is at least one damage value in this group of frames
				
				//and if it's in slam state
				if (state == ROUND_KING_STATE_SLAM)
					SoundManager::soundManager.playSound("smash");
				else if (state == ROUND_KING_STATE_JUMP) // if in jump state
					SoundManager::soundManager.playSound("crash");
			}
		}
		frameTimer = 0;
	}
}

void RoundKing::updateDamages() {
	// is roundking hitable
	if (active && hp > 0 && invincibleTimer <= 0) {
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			if ((*entity)->active && (*entity)->type == "hero") {
				//cast entity pointer to livingEntity pointer
				LivingEntity* enemy = (LivingEntity*)(*entity);

				//if enemy is hitting me
				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox)) {
					enemy->hitLanded(this);
					// roundking get hurt by player
					hp -= enemy->damage;

					if (hp > 0) {
						SoundManager::soundManager.playSound("enemyHit");
						invincibleTimer = 0.1f;
					}
				}
			}
		}
	}
}
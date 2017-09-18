#include "entity.h"

const int Entity::DIR_UP = 0, Entity::DIR_DOWN = 1, Entity::DIR_LEFT = 2, Entity::DIR_RIGHT = 3, Entity::DIR_NONE = -1;

// Virtual functions
void Entity::update() { ; } // override me to do something useful

void Entity::draw() { 
	// override me if you want something else or more specific
	if (currentFrame != NULL && active) {
		currentFrame->Draw(animSet->spriteSheet, x, y);
	}
	// draw collision box
	if (solid && Globals::debugging) {
		// sets the current drawing color, doesn't affect textures
		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);
	}
} 

void Entity::move(float angle) {
	moving = true;
	moveSpeed = moveSpeedMax;
	this->angle = angle;

	int newDirection = angleToDirection(angle);
	// if direction changed, update current animation
	if (direction != newDirection) {
		direction = newDirection;
		changeAnimation(state, false);
	}
}

void Entity::updateMovement() {
	updateCollisionBox();
	// store the collision box before we move
	lastCollisionBox = collisionBox;

	// reset total moves
	totalXMove = 0;
	totalYMove = 0;

	if (moveSpeed > 0) {
		// works out move distance using speed, dt (time since last loop) and multiplies by lerp
		float moveDist = moveSpeed * (TimeController::timeController.dT) * moveLerp;
		if (moveDist > 0) {
			// xmove = distance * cos (angle in radians) 
			float xMove = moveDist * (cos(angle*Globals::PI / 100));
			// ymove = distance * sin (angle in radians) 
			float yMove = moveDist * (sin(angle*Globals::PI / 100));

			x += xMove;
			y += yMove;

			totalXMove = xMove;
			totalYMove = yMove;

			if (!moving) {
				moveSpeed -= moveDist;
			}
		}
	}
	// sliding around
	if (slideAmount > 0) {
		float slideDist = slideAmount * TimeController::timeController.dT * moveLerp;
		if (slideDist > 0) {
			float xMove = slideDist * (cos(slideAngle * Globals::PI / 100));
			float yMove = slideDist * (sin(slideAngle* Globals::PI / 100));

			x += xMove;
			y += yMove;

			totalXMove += xMove;
			totalYMove += yMove;
			slideAmount -= slideDist;
		}
		else {
			slideAmount = 0;
		}
	}
	// now that we've moved, move the collisionBox up to where we are now
	updateCollisionBox();
	// to help with collision checking, union collisionBox with lastCollisionBox
	SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}

void updateCollisionBox();

void changeAnimation(int newState, bool resetFrameToBeginning) = 0; //abstract function -> makes the entire class abstract
void updateCollisions(); // how we bump into stuff in the world

								 // HELP Functions
static float distanceBetweenTwoRects(const SDL_Rect &r1, const SDL_Rect &r2);
static float distanceBetweenTwoEntities(const Entity *e1, const Entity *e2);
static float angleBetweenTwoEntities(const Entity *e1, const Entity *e2);
static bool checkCollision(const SDL_Rect cbox1, const SDL_Rect cbox2);
static int angleToDirection(float, float);
static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
static float angleBetweenTwoRects(const SDL_Rect &r1, const SDL_Rect &r2);

// global entities list I can refer to anytime
static list<Entity*> entities;
static bool EntityCompare(const Entity* const &a, const Entity* const &b); // helps compare 2 entities in a list to help sorting (sorts based on y value)
static void removeInactiveEntitiesFromList(list<Entity*> *entityList, bool deleteEntities);
static void removeAllFromList(list<Entity*> *entityList, bool deleteEntities);

#include "entity.h"

const int Entity::DIR_UP = 0, Entity::DIR_DOWN = 1, Entity::DIR_LEFT = 2, Entity::DIR_RIGHT = 3, Entity::DIR_NONE = -1;

// Virtual functions
void Entity::update() { ; } // override me to do something useful

void Entity::draw() { 
	// override me if you want something else or more specific
	if (currentFrame != NULL && active) {
		currentFrame->Draw(animSet->spriteSheet, x, y);
	}
	// draw collision box if debug activate
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

void Entity::updateCollisionBox() {
	collisionBox.x = x - collisionBox.w / 2;
	collisionBox.y = y + collisionBoxYOffset;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
}

void Entity::updateCollisions() {
	if (active && collidedWithSolids && (moveSpeed > 0 || slideAmount > 0)) {
		// list of potential collisions
		list<Entity*> collisions;

		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			// if we collide with this entity with our currently unioned collisionbox, add to list
			if ((*entity)->active
				&& (*entity)->type != this->type 
				&& (*entity)->solid 
				&& Entity::checkCollision(collisionBox, (*entity)->collisionBox)) {
				//add it to the list
				collisions.push_back(*entity);
			}
		}
		// if we have a list of potential entities we may hit, then lets check them properly to do collision resolution
		if (collisions.size() > 0) {
			updateCollisionBox();

			// multisample check for collisions from where we started to where we are planning to go to

			// first we are going to find the sample distance we should travel between checks
			float boxTravelSize = 0;
			if (collisionBox.w < collisionBox.h)
				boxTravelSize = collisionBox.w / 4;
			else
				boxTravelSize = collisionBox.h / 4;

			// use sample box to check for collisions from start point to end point, moving at boxTravelSize each sample
			SDL_Rect sampleBox = lastCollisionBox;
			float movementAngle = Entity::angleBetweenTwoRects(lastCollisionBox, collisionBox);

			bool foundCollision = false;
			while (!foundCollision) {
				// check sample box for collisions where it is now
				SDL_Rect intersection;
				for (auto entity = collisions.begin(); entity != collisions.end(); entity++) {
					if (SDL_IntersectRect(&sampleBox, &(*entity)->collisionBox, &intersection)) {
						// there is a collision
						foundCollision = true;
						moveSpeed = 0;
						moving = false;
						slideAngle = angleBetweenTwoEntities((*entity), this);
						// currently intersecting a entity, now we need to do collision resolution
						if (intersection.w < intersection.h) {
							if (lastCollisionBox.x + lastCollisionBox.w / 2 < (*entity)->collisionBox.w / 2)
								sampleBox.x -= intersection.w; // started on left so move left out of collision
							else
								sampleBox.x += intersection.w; // otherwise, started on right
						}
						else {
							if (lastCollisionBox.y + lastCollisionBox.h / 2 < (*entity)->collisionBox.y + (*entity)->collisionBox.h / 2)
								sampleBox.y -= intersection.h; // started above so move out of collision
							else
								sampleBox.y += intersection.h; // otherwise started below
						}
					}
				}

				// if collisions found or sample box is at same place as collision box, exit loop
				if (foundCollision || (sampleBox.x == collisionBox.x && sampleBox.y == collisionBox.y))
					break;

				// move sample box up to check the next spot
				if (distanceBetweenTwoRects(sampleBox, collisionBox) > boxTravelSize) {
					float xMove = boxTravelSize * (cos(movementAngle*Globals::PI / 100));
					float yMove = boxTravelSize * (sin(movementAngle*Globals::PI / 100));

					sampleBox.x += xMove;
					sampleBox.y += yMove;
				}
				else {
					sampleBox = collisionBox;
				}
			}

			if (foundCollision) {
				// move our entity to where the sample box ended up
				slideAmount = slideAmount / 2;
				x = sampleBox.x + sampleBox.w / 2;
				y = sampleBox.y - collisionBoxYOffset;
			}
			updateCollisionBox();
		}
	}
}

void changeAnimation(int newState, bool resetFrameToBeginning) = 0; //abstract function -> makes the entire class abstract


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

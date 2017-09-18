#ifndef ENTITY
#define ENTITY

#include "globals.h"
#include "timeController.h"
#include "animationSet.h"

// Abstract class
class Entity {
public:
	// reference constants
	static const int DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE;

	// keeps track of the state from the entity
	int state;

	float x, y;
	int direction;
	bool solid = true; // is this thing solid, can things pass through it ?
	bool collidedWithSolids = true; // sometimes can pass through other solid
	bool active = true; // entity turn on or off
	string type = "entity"; // what type of entity is it : e.g : hero, wall, enemy, etc
	bool moving; // is the entity moving ?
	float angle; // angle to move entity in (360 degree angle)
	float moveSpeed;
	float moveSpeedMax;
	float slideAngle; // direction pushed in
	float slideAmount; // amount of push in the slideAngle
	float moveLerp = 4; // acting kinds like a magic number to smooth things up
	float totalXMove, totalYMove; // keeps track of total s,y movement per movement turn, just in case we need to retract movement

	SDL_Rect collisionBox; // box describing the size of our entity and this is used to bump into things
	SDL_Rect lastCollisionBox; // Where our collisionBox was last
	int collisionBoxW, collisionBoxH; // default Width and Height for the collisionBox
	float collisionBoxYOffset; // from my entities y value, where should I sit this collisionBox

	AnimationSet *animSet; // Set of all animations this entity can have
	Animation *currentAnim; // current animation the entity is using
	Frame *currentFrame; // the current frame in the above animation the entity is using
	float frameTimer; // helps animate frame to frame

	// Virtual functions
	virtual void update();
	virtual void draw();

	virtual void move(float angle);
	virtual void updateMovement();
	virtual void updateCollisionBox();

	virtual void changeAnimation(int newState, bool resetFrameToBeginning) = 0; //abstract function -> makes the entire class abstract
	virtual void updateCollisions(); // how we bump into stuff in the world

	// HELP Functions
	static float distanceBetweenTwoRects(const SDL_Rect &r1, const SDL_Rect &r2);
	static float distanceBetweenTwoEntities(const Entity *e1, const Entity *e2);
	static float angleBetweenTwoEntities(const Entity *e1, const Entity *e2);
	static bool checkCollision(const SDL_Rect cbox1, const SDL_Rect cbox2);
	static int angleToDirection(float);
	static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
	static float angleBetweenTwoRects(const SDL_Rect &r1, const SDL_Rect &r2);

	// global entities list I can refer to anytime
	static list<Entity*> entities;
	static bool EntityCompare(const Entity* const &a, const Entity* const &b); // helps compare 2 entities in a list to help sorting (sorts based on y value)
	static void removeInactiveEntitiesFromList(list<Entity*> *entityList, bool deleteEntities);
	static void removeAllFromList(list<Entity*> *entityList, bool deleteEntities);
};

#endif

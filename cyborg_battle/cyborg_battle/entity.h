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
	// is this thing solid, can things pass through it ?
	bool solid = true; 
	// sometimes can pass through other solid
	bool collidedWithSolids = true; 
	// entity turn on or off
	bool active = true; 
	// what type of entity is it : e.g : hero, wall, enemy, etc
	string type = "entity"; 
	// is the entity moving ?
	bool moving; 
	// angle to move entity in (360 degree angle)
	float angle; 
	float moveSpeed;
	float moveSpeedMax;
	// direction pushed in
	float slideAngle; 
	// amount of push in the slideAngle
	float slideAmount; 
	// acting kinds like a magic number to smooth things up
	float moveLerp = 4; 
	// keeps track of total x,y movement per movement turn, just in case we need to retract movement
	float totalXMove, totalYMove; 

	// box describing the size of our entity and this is used to bump into things
	SDL_Rect collisionBox; 
	// Where our collisionBox was last
	SDL_Rect lastCollisionBox;
	// default Width and Height for the collisionBox
	int collisionBoxW, collisionBoxH; 
	// from my entities y value, where should I sit this collisionBox
	float collisionBoxYOffset; 

	// Set of all animations this entity can have
	AnimationSet *animSet; 
	// current animation the entity is using
	Animation *currentAnim;
	// the current frame in the above animation the entity is using
	Frame *currentFrame;
	// helps animate frame to frame
	float frameTimer; 

	// Virtual functions
	virtual void update();
	virtual void draw();

	virtual void move(float angle);
	virtual void updateMovement();
	virtual void updateCollisionBox();

	virtual void changeAnimation(int newState, bool resetFrameToBeginning) = 0; //abstract function -> makes the entire class abstract
	// how we bump into stuff in the world																			// how we bump into stuff in the world
	virtual void updateCollisions(); 

	// HELP Functions
	static float distanceBetweenTwoRects(const SDL_Rect &r1, const SDL_Rect &r2);
	static float distanceBetweenTwoEntities(const Entity *e1, const Entity *e2);
	static float angleBetweenTwoEntities(const Entity *e1, const Entity *e2);
	static bool checkCollision(const SDL_Rect cbox1, const SDL_Rect cbox2);
	// converts a 360degree angle into one of our constant value (0,1,2,3,-1)
	static int angleToDirection(float);
	static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
	static float angleBetweenTwoRects(const SDL_Rect &r1, const SDL_Rect &r2);

	// global entities list I can refer to anytime
	static list<Entity*> entities;
	// helps compare 2 entities in a list to help sorting (sorts based on y value)
	static bool EntityCompare(const Entity* const &a, const Entity* const &b); 
	static void removeInactiveEntitiesFromList(list<Entity*> *entityList, bool deleteEntities);
	static void removeAllFromList(list<Entity*> *entityList, bool deleteEntities);
};

#endif

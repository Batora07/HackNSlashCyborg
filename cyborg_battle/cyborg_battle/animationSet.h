#ifndef ANIMATIONSET
#define ANIMATIONSET

#include "animation.h"

using namespace std;

class AnimationSet {
public:
	string imageName;
	SDL_Texture* spriteSheet; // holds the spritesheet image for all of our animations
	SDL_Texture* whiteSpriteSheet = NULL; // Use this spritesheet to show damage
	/* Actually, it works like this : we'll get the spriteSheet then acces the pixels in it and swap colors 
	so the characters appears "hurt"*/
	list<Animation> animations;

	~AnimationSet(); 

	Animation* getAnimation(string name);

	void loadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColourKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);

};

#endif // !ANIMATIONSET

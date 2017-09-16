#include "animationSet.h"


AnimationSet::~AnimationSet() {
	cleanup(spriteSheet);
	if (whiteSpriteSheet != NULL) {
		cleanup(whiteSpriteSheet);
	}
}


Animation* AnimationSet::getAnimation(string name) {
	for (list<Animation>::iterator animIt = animations.begin();
		animIt != animations.end(); animIt++) {
		Animation* anim = &(*animIt);

		// if we find match on name return that animation from the list
		if (name == anim->name) {
			return anim;
		}
	}

	return NULL;
}

#include "frame.h"

void Frame::Draw(SDL_Texture* spriteSheet, float x, float y) {
	SDL_Rect dest; // destination of where we want to draw this frame
	dest.x = x - offset.x;
	dest.y = y - offset.y;
	dest.w = clip.w;
	dest.h = clip.h;

	renderTexture(spriteSheet, Globals::renderer, dest, &clip);
}
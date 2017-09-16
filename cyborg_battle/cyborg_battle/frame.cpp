#include "frame.h"

void Frame::Draw(SDL_Texture* spriteSheet, float x, float y) {
	SDL_Rect dest; // destination of where we want to draw this frame
	dest.x = x - offset.x;
	dest.y = y - offset.y;
	dest.w = clip.w;
	dest.h = clip.h;

	renderTexture(spriteSheet, Globals::renderer, dest, &clip);
}

void Frame::loadFrame(ifstream &file, list<DataGroupType> &groupTypes) {
	// ok so we dragged these groupTypes all the way here so we could construct groups on our frame to hold the data in
	GroupBuilder::buildGroups(groupTypes, frameData);

	string buffer;
	//frame - basically a separator between data
	getline(file, buffer);
	//clip
	getline(file, buffer);
	stringstream ss; // good for building strings and reading from strings
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> clip.x >> clip.y >> clip.w >> clip.h;
	//offset / pivot
	getline(file, buffer);
	ss.clear();
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> offset.x >> offset.y;
	//duration
	getline(file, buffer);
	ss.clear();
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> duration;
	//index
	getline(file, buffer);
	ss.clear();
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> frameNumber;

	GroupBuilder::loadGroups(file, frameData);
}

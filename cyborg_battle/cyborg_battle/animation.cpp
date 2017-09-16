#include "animation.h"

Animation::Animation(string name) {
	this->name = name;
}

int Animation::getNextFrameNumber(int frameNumber) {
	if (frameNumber + 1 < frames.size()) {
		return frameNumber + 1;
	}
	else {
		return 0;
	}
}
Frame* Animation::getNextFrame(Frame* frame) {
	return getFrame(getNextFrameNumber(frame->frameNumber));
}
int Animation::getEndFrameNumber(int frameNumber){
	return (frameNumber + 1) % frames.size;
}
Frame* Animation::getFrame(int frameNumber) {
	if (frames.size() == 0) {
		return NULL;
	}

	list<Frame>::iterator i = frames.begin(); // point iterator to first frame in the frames list

	int counter = 0;

	for (counter = 0; counter < frameNumber && counter < frames.size() - 1; counter++) {
		i++; // make iterator point to the next frame in list
	}

	Frame* frame = &(*i); // make frame point to the frame inside the list our iterator is pointing at 

	return frame;
}

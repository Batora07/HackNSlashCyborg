#ifndef ANIMATION
#define ANIMATION

#include "frame.h"

using namespace std;

class Animation {
public:
	string name; //name of the animation
	list<Frame> frames; //list of our frames

	Animation(string name = "");

	int getNextFrameNumber(int frameNumber); //return the next frame number in the list
	Frame* getNextFrame(Frame* frame); // whatever frame we're up to, it checks its frame number and returns the one after that
	int getEndFrameNumber(); //returns the last frame number
	Frame* getFrame(int frameNumber); // get frame using frame number

	void loadAnimation(ifstream &file, list<DataGroupType> &groupTypes);
};

#endif

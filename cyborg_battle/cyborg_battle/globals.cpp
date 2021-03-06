#include "globals.h"

const float Globals::PI = 3.14159;

//set to true to see hitboxes
bool Globals::debugging = false;


int Globals::ScreenWidth = 640, Globals::ScreenHeight = 352, Globals::ScreenScale = 1;
SDL_Renderer *Globals::renderer = NULL;

string Globals::clipOffDataHeader(string data) {
	int pos = data.find(":", 0); // returns where we find the : in the string otherwise returns -1
	if (pos != -1) {
		data = data.substr(pos+1, data.length() - pos + 2);
	}
	return data;
}

SDL_Rect Globals::camera;
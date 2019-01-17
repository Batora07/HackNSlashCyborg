#include "hpBar.h"

void HPBar::draw() {
	if (entity != NULL) {
		// set rgb color
		SDL_SetRenderDrawColor(Globals::renderer, 240, 51, 159, SDL_ALPHA_OPAQUE);
		
		//Draw outside of the HP bar
		SDL_Rect barContainer = { x, y, barWidth, barHeight };
		SDL_RenderDrawRect(Globals::renderer, &barContainer);

		// percentage of entities health remaining
		float hpPercent = entity->hp / (entity->hpMax*1.0f);  //*1.0f upgrade math integer into float division so we don't lose decimals

		SDL_Rect hpRect = { x + 2, y + 2, (barWidth - 4) * hpPercent, barHeight - 4 };
	
		//Render the rect
		SDL_RenderFillRect(Globals::renderer, &hpRect);
	}
}
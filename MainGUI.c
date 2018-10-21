/*
 * gui.c
 *
 */

#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include "MainGUI.h"
#include "SPGUIManager.h"

int main_gui(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	SPGuiManager* manager = spManagerCreate();
	if (manager == NULL) {
		SDL_Quit();
		return 0;
	}

	SDL_Event event;
	while (1) {
		SDL_WaitEvent(&event);
		SP_MANAGER_EVENET managerEvent =spHandleManagerEvent(manager, &event);
		if (managerEvent == SP_MANAGER_QUIT) {
			printf("Exiting...\n");
			break;
		}
		if(managerEvent == SP_MANAGER_UPDATE){
			spManagerDraw(manager);
		}
	}
	spManagerDestroy(manager);
	SDL_Quit();
	return 0;
}


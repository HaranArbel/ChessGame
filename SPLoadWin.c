/*
 * LoadWin.c
 *
 */

#include "SPLoadWin.h"


SPLoadWin* spLoadWindowCreate(){
	SPLoadWin* res = NULL;
	res = (SPLoadWin*) calloc(sizeof(SPLoadWin), sizeof(char));
	if (res == NULL){
		return NULL;
	}
	// Create an application window with the following LOAD:
	res->loadWindow = SDL_CreateWindow("ChessProg - Load", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			400,                               // width, in pixels
			800,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->loadWindow == NULL) {
		spLoadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->loadRenderer = SDL_CreateRenderer(res->loadWindow, -1,SDL_RENDERER_ACCELERATED);
	if (res->loadRenderer == NULL ) {
		// In case that the window could not be made
		spLoadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->widgets= spCreateLoadWindowWidgets(res->loadRenderer);
	if (res->widgets == NULL ) {
		// In case that the window could not be made
		spLoadWindowDestroy(res);
		printf("Could not create widgets: %s\n", SDL_GetError());
		return NULL ;
	}

	res->numOfWidgets = LOAD_WIN_WIDG_N;
	res->first = true;
	res->gameToLoad = 1;

	char* path = spGetLoadPath(res->gameToLoad);
	FILE* file = fopen(path, "r");
	if(file == NULL){
		res->isValidLoad = false;
	}
	else{
		fclose(file);
		res->isValidLoad = true;
	}


	free(path);
	return res;
}


Widget** spCreateLoadWindowWidgets(SDL_Renderer* loadRenderer) {
	int i;
	if (loadRenderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = calloc(LOAD_WIN_WIDG_N,sizeof(Widget*));
	if (widgets == NULL ) {
		return NULL ;
	}

	SDL_Rect slot1R = { .x =BUTTON_SHIFT+70, .y = 35, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH+20 };
	SDL_Rect slot2R  = { .x =BUTTON_SHIFT+70, .y = 140, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH +20};
	SDL_Rect slot3R = { .x = BUTTON_SHIFT+70, .y = 245, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH +20};
	SDL_Rect slot4R = { .x = BUTTON_SHIFT+70, .y = 350, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH+20 };
	SDL_Rect slot5R = { .x = BUTTON_SHIFT+70, .y = 455, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH +20};
	SDL_Rect backR = { .x = BUTTON_SHIFT+70, .y = 560, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH+20 };
	SDL_Rect loadR = { .x = BUTTON_SHIFT+70, .y = 665, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH +20};
	widgets[SLOT1_BN] = spCreateSimpleButton(loadRenderer, &slot1R, "./graphics/images/buttons/slot1active.bmp", "./graphics/images/buttons/slot1.bmp",true);
	widgets[SLOT2_BN] = spCreateSimpleButton(loadRenderer, &slot2R, "./graphics/images/buttons/slot2active.bmp","./graphics/images/buttons/slot2.bmp",true);
	widgets[SLOT3_BN] = spCreateSimpleButton(loadRenderer, &slot3R, "./graphics/images/buttons/slot3active.bmp","./graphics/images/buttons/slot3.bmp",true);
	widgets[SLOT4_BN] = spCreateSimpleButton(loadRenderer, &slot4R, "./graphics/images/buttons/slot4active.bmp","./graphics/images/buttons/slot4.bmp",true);
	widgets[SLOT5_BN] = spCreateSimpleButton(loadRenderer, &slot5R, "./graphics/images/buttons/slot5active.bmp","./graphics/images/buttons/slot5.bmp",true);
	widgets[LOADWIN_BACK_BN] = spCreateSimpleButton(loadRenderer, &backR, "./graphics/images/buttons/back.bmp",NULL,true);
	widgets[LOADWIN_LOAD_BN] = spCreateSimpleButton(loadRenderer, &loadR, "./graphics/images/buttons/loadactive.bmp","./graphics/images/buttons/load.bmp",true);

	for(i = 0; i < LOAD_WIN_WIDG_N; i++){
		if(widgets[i] == NULL){
			printf("Could not create widget number %d",i);
			spLoadWindowWidgetsDestroy(widgets);
			break;
		}
	}
	return widgets;
}


void spLoadWindowDraw(SPLoadWin* src) {
	int i;
	if(src == NULL){
		return;
	}
	SDL_SetRenderDrawColor(src->loadRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->loadRenderer);
	for (i = 0; i < SLOTS_NUM; i++){
		((SPSimpleButton*) src->widgets[i]->data)->active = false;
	}

	((SPSimpleButton*) src->widgets[src->gameToLoad-1]->data)->active = true;
	((SPSimpleButton*) src->widgets[LOADWIN_LOAD_BN]->data)->active = false;
	if(src->isValidLoad == true){
		((SPSimpleButton*) src->widgets[LOADWIN_LOAD_BN]->data)->active=true;
	}
	for (i=0; i < src->numOfWidgets; i++) {
		(src->widgets[i])->drawWidget(src->widgets[i]);
	}
	SDL_RenderPresent(src->loadRenderer);
}


void spLoadWindowDestroy(SPLoadWin* src) {
	if (src == NULL) {
		return;
	}
	spLoadWindowWidgetsDestroy(src->widgets);

	if (src->loadRenderer != NULL ) {
		SDL_DestroyRenderer(src->loadRenderer);
	}
	if (src->loadWindow != NULL ) {
		SDL_DestroyWindow(src->loadWindow);
	}
	free(src);
}


void spLoadWindowWidgetsDestroy(Widget** widgets){
	int i;

	for(i = 0; i < LOAD_WIN_WIDG_N; i++){
		if(widgets[i] != NULL){
			destroyWidget(widgets[i]); //NULL SAFE
		}
	}
	free(widgets);
}

SP_LOAD_EVENT spLoadWindowHandleEvent(SPLoadWin* src, SDL_Event* event) {
	char* path;
	if (!event) {
		return LOAD_EVENT_INVALID_ARGUMENT;
	}
	switch (event->type) {

	case SDL_MOUSEBUTTONUP:

		for (int i = 0; i < 5; i++){
			if (src->widgets[i]->handleEvent(src->widgets[i],event) == 1) {
			src->gameToLoad = i+1;
			path = spGetLoadPath(src->gameToLoad);
			FILE* file = fopen(path, "r");
			if(file == NULL){
				src->isValidLoad = false;
			}
			else{
				fclose(file);
				src->isValidLoad = true;
			}

			free(path);
			return LOAD_EVENT_UPDATE;
			}
		}
		if (src->widgets[LOADWIN_BACK_BN]->handleEvent(src->widgets[LOADWIN_BACK_BN],event) == 1)  {
			return LOAD_EVENT_BACK;

		}else if (src->widgets[LOADWIN_LOAD_BN]->handleEvent(src->widgets[6],event) == 1){
			if(src->isValidLoad == true){
				return LOAD_EVENT_START;
			}
			return LOAD_EVENT_UPDATE;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return LOAD_EVENT_QUIT;
		}
		break;
	default:
		return LOAD_EVENT_NONE;
	}
	return LOAD_EVENT_NONE;
}


char* spGetLoadPath(int gameToLoad){

	char* path = (char*)calloc(SP_MAX_LINE_LENGTH,sizeof(char));
	char fileNumberStr[2],fileFormat[SLOTS_NUM-1];
	strcpy(path,"./graphics/files/");

	fileNumberStr[0] = (char)(gameToLoad+1+'0');
	fileNumberStr[1] = '\0';

	strcpy(fileFormat,".txt");
	strcat(path,fileNumberStr);
	strcat(path,fileFormat);
	return path;
}

bool spIsValidLoadPath(char* path){
	FILE* file = fopen(path, "r");
	if(file != NULL){
		return true;
	}
	else{
		fclose(file);
		return false;
	}
}

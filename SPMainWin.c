/*
 * mainWin.c
 */

#include "SPMainWin.h"


/*
 * this function creat the widgets of this specific window.
 * the fucntion return an refernce to an array of the relevenat widgets of this window.
 */
Widget** spCreateMainWindowWidgets(SDL_Renderer* mainRenderer) {
	int i;
	if (mainRenderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = calloc(WIDGETS_MENU_WINDOW_NUM,sizeof(Widget*));
	if (widgets == NULL ) {
		return NULL ;
	} //todo
	SDL_Rect newgameR = { .x =BUTTON_SHIFT+190, .y = 110, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect loadR = { .x = BUTTON_SHIFT+190, .y = 240, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect quitR = { .x = BUTTON_SHIFT+190, .y = 370, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	widgets[NEW_GAME_BN] = spCreateSimpleButton(mainRenderer, &newgameR, "./graphics/images/buttons/newgame.bmp",NULL,true);
	widgets[LOAD_BN] = spCreateSimpleButton(mainRenderer, &loadR, "./graphics/images/buttons/load.bmp",NULL,true);
	widgets[QUIT_BN] = spCreateSimpleButton(mainRenderer, &quitR, "./graphics/images/buttons/quit.bmp",NULL,true);

	for(i = 0; i < WIDGETS_MENU_WINDOW_NUM; i++){
		if(widgets[i] == NULL){
			printf("Could not create widget number %d",i);
			spMainWindowWidgetsDestroy(widgets);
			break;
		}
	}
	return widgets;
}

/*
 * this function destroy an array of widgets.
 * the function doesn't return nothing.
 */
void spMainWindowWidgetsDestroy(Widget** widgets){
	int i;
	for(i = 0;i < WIDGETS_MENU_WINDOW_NUM; i++){
		if(widgets[i] != NULL){
			destroyWidget(widgets[i]); //NULL SAFE
		}
	}
	free(widgets);
}

/*
 * this function creat a window of main menu, it includes the the limits of the window and put on the window the
 * relevant buttons and widegets.
 * the function return a referece to SPMainWin.
 */
SPMainWin* spMainWindowCreate() {
	SPMainWin* res = NULL;
	res = (SPMainWin*)calloc(sizeof(SPMainWin), sizeof(char));
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->mainWindow = SDL_CreateWindow("Main Menu", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			600,                               // width, in pixels
			600,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->mainWindow == NULL ) {
		spMainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1,SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL ) {
		spMainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->widgets = spCreateMainWindowWidgets(res->mainRenderer);
	if (res->widgets == NULL ) {
		spMainWindowDestroy(res);
		printf("Could not create widgets: %s\n", SDL_GetError());
		return NULL ;
	}
	res->numOfWidgets = WIDGETS_MENU_WINDOW_NUM;
	res->first = true;
	return res;
}

/*the function draw a window of main menu.
 * this function doesn't return nothing.
 */
void spMainWindowDraw(SPMainWin* src) {
	int i;
	if(src == NULL){
		return;
	}
	SDL_SetRenderDrawColor(src->mainRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->mainRenderer);
	for (i = 0; i < src->numOfWidgets; i++) {
		(src->widgets[i])->drawWidget(src->widgets[i]);
	}
	SDL_RenderPresent(src->mainRenderer);
}

/*this function free the memory of all the things that relevant to the main menu window.
 * this function doesn't return nothing.
 */
void spMainWindowDestroy(SPMainWin* src) {
	if (src == NULL) {
		return;
	}
	spMainWindowWidgetsDestroy(src->widgets);
	if (src->mainRenderer != NULL ) {
		SDL_DestroyRenderer(src->mainRenderer);
	}
	if (src->mainWindow != NULL ) {
		SDL_DestroyWindow(src->mainWindow);
	}
	free(src);
}
/*
 * this function control all the events in the main menu window and operate them.
 * the function returns an message: SP_MAIN_EVENT, that indicate what exectly happened.
 */

SP_MAIN_EVENT spMainWindowHandleEvent(SPMainWin* src, SDL_Event* event) {
	if (event == NULL) {
		return SP_MAIN_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (src->widgets[NEW_GAME_BN]->handleEvent(src->widgets[NEW_GAME_BN],event) == 1) {
			return SP_MAIN_NEW_GAME;
		}
		else if (src->widgets[LOAD_BN]->handleEvent(src->widgets[LOAD_BN],event) == 1) {
			return SP_MAIN_LOAD;
		}
		else if (src->widgets[QUIT_BN]->handleEvent(src->widgets[QUIT_BN],event) == 1){
		return SP_MAIN_QUIT;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SP_MAIN_QUIT;
		}
		break;
	default:
		return SP_MAIN_NONE;
	}
	return SP_MAIN_NONE;
}
/*
 * this function hide the main menu window.
 * this function doesn't return nothing.
 */
void spMainWindowHide(SPMainWin* src) {
	SDL_HideWindow(src->mainWindow);
}
/*
 * this function show the main menu window.
 * this function doesn't return nothing.
 */
void spMainWindowShow(SPMainWin* src) {
	SDL_ShowWindow(src->mainWindow);
}


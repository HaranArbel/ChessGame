/*
 * SettingsWin.c
 *
 */
#include "SPSettingsWin.h"


/*
 * this function creat the window of the settings, it includes all the options of the settings,
 * it advanced step by setp according to the choose of the user and put on the window the
 * relevant buttons and widegets.
 * the function return a referece to SPSettingsWin.
 */
SPSettingsWin* spSettingsWindowCreate(){
	SPSettingsWin* res = NULL;
	res = (SPSettingsWin*) calloc(sizeof(SPSettingsWin), sizeof(char));
	if(res == NULL){
		return NULL;
	}
	// Create an application window with the following settings:
	res->SettingsWindow = SDL_CreateWindow("Settings", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			600,                               // width, in pixels
			600,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->SettingsWindow == NULL ) {
		spSettingsWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->SettingsRenderer = SDL_CreateRenderer(res->SettingsWindow, -1,SDL_RENDERER_ACCELERATED);
	if (res->SettingsRenderer==NULL ) {
		// In case that the window could not be made
		spSettingsWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	res->widgets = spCreateSettingsWindowWidgets(res->SettingsRenderer);
	if (res->widgets == NULL ) {
		// In case that the window could not be made
		spSettingsWindowDestroy(res);
		printf("Could not create widgets: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("Created widgets successfully\n");
	res->numOfWidgets = WIDGETS_SETTINGS_WINDOW_NUM;
	res->first = true;
	res->gameMode = ONE_PLAYER;
	res->difficulty = 2;
	res->userColor = WHITE;
	printf("Settings window created successfully\n");
	return res;
}


/*
 * this function creat the widgets of this specific setting window.
 * the fucntion return an refernce to an array of the relevenat widgets of this window.
 */
Widget** spCreateSettingsWindowWidgets(SDL_Renderer* SettingsRenderer) {
	int i;
	if (SettingsRenderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = calloc(WIDGETS_SETTINGS_WINDOW_NUM,sizeof(Widget*));
	if (widgets == NULL ) {
		return NULL ;
	}

	SDL_Rect oneplayerR = { .x =BUTTON_SHIFT+190, .y = 30, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect twoplayersR = { .x =BUTTON_SHIFT+190, .y = 140, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };

	// difficulty menu
	SDL_Rect noobR = { .x = BUTTON_SHIFT+90, .y = 50, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect easyR = { .x = BUTTON_SHIFT+290, .y = 50, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect moderateR = { .x = BUTTON_SHIFT+90, .y = 250, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect hardR = { .x = BUTTON_SHIFT+290, .y = 250, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };

	// user color menu
	SDL_Rect blackR = { .x = BUTTON_SHIFT+155, .y = 100, .h = 90, .w = 90 };
	SDL_Rect whiteR = { .x = BUTTON_SHIFT+300, .y = 100, .h = 90, .w = 90 };

	// general
	SDL_Rect nextR = { .x = BUTTON_SHIFT+190, .y = 390, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect startR = { .x = BUTTON_SHIFT+190, .y = 390, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect backR = { .x = BUTTON_SHIFT+190, .y = 500, .h =BUTTON_HEIGTH, .w = BUTTON_WIDTH };

	widgets[ONE_PLAYER_BN] = spCreateSimpleButton(SettingsRenderer, &oneplayerR,"./graphics/images/buttons/oneplayeractive.bmp","./graphics/images/buttons/oneplayer.bmp",false);
	widgets[TWO_PLAYERS_BN] = spCreateSimpleButton(SettingsRenderer, &twoplayersR, "./graphics/images/buttons/twoplayersactive.bmp","./graphics/images/buttons/twoplayers.bmp",false);
	widgets[NEXT_BN] = spCreateSimpleButton(SettingsRenderer, &nextR, "./graphics/images/buttons/next.bmp",NULL,false);
	widgets[START_BN] = spCreateSimpleButton(SettingsRenderer, &startR, "./graphics/images/buttons/startactive.bmp",NULL,false);
	widgets[BACK_BN] = spCreateSimpleButton(SettingsRenderer, &backR, "./graphics/images/buttons/back.bmp",NULL,false);
	widgets[NOOB_BN] = spCreateSimpleButton(SettingsRenderer, &noobR, "./graphics/images/buttons/noobactive.bmp","./graphics/images/buttons/noob.bmp",false);
	widgets[EASY_BN] = spCreateSimpleButton(SettingsRenderer, &easyR, "./graphics/images/buttons/easyactive.bmp","./graphics/images/buttons/easy.bmp",false);
	widgets[MODER_NB] = spCreateSimpleButton(SettingsRenderer, &moderateR, "./graphics/images/buttons/moderateactive.bmp","./graphics/images/buttons/moderate.bmp",false);
	widgets[HARD_BN] = spCreateSimpleButton(SettingsRenderer, &hardR, "./graphics/images/buttons/hardactive.bmp","./graphics/images/buttons/hard.bmp",false);
	widgets[USER_COLOR_B_BN] = spCreateSimpleButton(SettingsRenderer, &blackR, "./graphics/images/pieces/BlackKing.bmp","./graphics/images/pieces/BlackKingSelect.bmp",false);
	widgets[USER_COLOR_W_BN] = spCreateSimpleButton(SettingsRenderer, &whiteR, "./graphics/images/pieces/WhiteKing.bmp","./graphics/images/pieces/WhiteKingSelect.bmp",false);

	for(i = 0 ; i < WIDGETS_SETTINGS_WINDOW_NUM; i++){
		if(widgets[i] == NULL){
			destroyWidget(widgets[i]); // NULL safety
			printf("Could not create widget number %d",i);
			spSettingsWindowWidgetsDestroy(widgets);
			break;
		}
	}
	return widgets;
}


/*the function draw a window of settings.
 * this function doesn't return nothing.
 */
void spSettingsWindowDraw(SPSettingsWin* src) {

	int i;
	if(src == NULL){
		return;
	}

	SDL_SetRenderDrawColor(src->SettingsRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->SettingsRenderer);
	for (i = 0; i < src->numOfWidgets; i++) {
		((SPSimpleButton*)src->widgets[i]->data)->on = false;
		((SPSimpleButton*)src->widgets[i]->data)->active = false;
	}

	switch(src->SettingsMenu){

		case(SP_GAME_MODE_MENU):{
			((SPSimpleButton*) src->widgets[ONE_PLAYER_BN]->data)->on = true;
			((SPSimpleButton*) src->widgets[TWO_PLAYERS_BN]->data)->on = true;

			if(src->gameMode == ONE_PLAYER){
				((SPSimpleButton*) src->widgets[ONE_PLAYER_BN]->data)->active = true;
				((SPSimpleButton*) src->widgets[NEXT_BN]->data)->on = true;
			}
			else if(src->gameMode == TWO_PLAYERS){
				((SPSimpleButton*) src->widgets[TWO_PLAYERS_BN]->data)->active = true;
				((SPSimpleButton*) src->widgets[START_BN]->data)->on = true;
				((SPSimpleButton*) src->widgets[START_BN]->data)->active = true;
			}
		}	break;

		case(SP_DIFFICULTY_MENU):{
			((SPSimpleButton*) src->widgets[NOOB_BN]->data)->on = true;
			((SPSimpleButton*) src->widgets[EASY_BN]->data)->on = true;
			((SPSimpleButton*) src->widgets[MODER_NB]->data)->on = true;
			((SPSimpleButton*) src->widgets[HARD_BN]->data)->on = true;
			((SPSimpleButton*) src->widgets[NEXT_BN]->data)->on = true;
			((SPSimpleButton*) src->widgets[src->difficulty+GAME_MODE_WIDG_N]->data)->active = true;

		}	break;

		case(SP_USER_COLOR_MENU):{
			((SPSimpleButton*) src->widgets[USER_COLOR_B_BN]->data)->on = true;
			((SPSimpleButton*) src->widgets[USER_COLOR_W_BN]->data)->on = true;
			((SPSimpleButton*) src->widgets[src->userColor+GAME_MODE_WIDG_N + DIFFIC_WIDG_N]->data)->active = true;
			((SPSimpleButton*) src->widgets[START_BN]->data)->on = true;
			((SPSimpleButton*) src->widgets[START_BN]->data)->active = true;
		}	break;
	}

	((SPSimpleButton*) src->widgets[BACK_BN]->data)->on = true;

	for (i = 0; i < src->numOfWidgets; i++) { //draw all widgets
		(src->widgets[i])->drawWidget(src->widgets[i]);
	}
	SDL_RenderPresent(src->SettingsRenderer);
}

/*this function free the memory of all the things that relevant to the settings window.
 * this function doesn't return nothing.
 */
void spSettingsWindowDestroy(SPSettingsWin* src) {
	if (src == NULL) {
		return;
	}
	spSettingsWindowWidgetsDestroy(src->widgets);
	if (src->SettingsRenderer != NULL ) {
		SDL_DestroyRenderer(src->SettingsRenderer);
	}
	if (src->SettingsWindow != NULL ) {
		SDL_DestroyWindow(src->SettingsWindow);
	}
	free(src);
}
/*
 * this function destroy an array of widgets.
 * the function doesn't return nothing.
 */
void spSettingsWindowWidgetsDestroy(Widget** widgets){
	int i;
	for(i = 0; i < WIDGETS_SETTINGS_WINDOW_NUM; i++){
		if(widgets[i] != NULL){
			destroyWidget(widgets[i]); //NULL SAFE
		}
	}
	free(widgets);
}

/*
 * this function control all the events in the setting window and operate them.
 * the function returns an message: SP_SETTINGS_EVENT, that indicate what exectly happened.
 */
SP_SETTINGS_EVENT spSettingsWindowHandleEvent(SPSettingsWin* src, SDL_Event* event) {
	if (event == NULL) {
		return SETTINGS_EVENT_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (src->widgets[ONE_PLAYER_BN]->handleEvent(src->widgets[ONE_PLAYER_BN],event) == 1){
			src->gameMode = ONE_PLAYER;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[TWO_PLAYERS_BN]->handleEvent(src->widgets[TWO_PLAYERS_BN],event) == 1){
			src->gameMode = TWO_PLAYERS;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[START_BN]->handleEvent(src->widgets[START_BN],event) == 1){
			return SETTINGS_EVENT_START;
		}
		else if (src->widgets[NEXT_BN]->handleEvent(src->widgets[NEXT_BN],event) == 1){
			src->SettingsMenu = src->SettingsMenu + 1; // moving to next settings menu
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[BACK_BN]->handleEvent(src->widgets[BACK_BN],event) == 1){
			if(src->SettingsMenu == SP_GAME_MODE_MENU){
				return SETTINGS_EVENT_BACK;
			}
			src->SettingsMenu = src->SettingsMenu - 1;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[NOOB_BN]->handleEvent(src->widgets[NOOB_BN],event) == 1){
			src->difficulty = 1;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[EASY_BN]->handleEvent(src->widgets[EASY_BN],event) == 1){
			src->difficulty = 2;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[MODER_NB]->handleEvent(src->widgets[MODER_NB],event) == 1){
			src->difficulty = 3;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[HARD_BN]->handleEvent(src->widgets[HARD_BN],event) == 1){
			src->difficulty = 4;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[USER_COLOR_B_BN]->handleEvent(src->widgets[USER_COLOR_B_BN],event) == 1){
			src->userColor = BLACK;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[USER_COLOR_W_BN]->handleEvent(src->widgets[USER_COLOR_W_BN],event) == 1){
			src->userColor = WHITE;
			return SETTINGS_EVENT_UPDATE;
		}
		break;

	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return SETTINGS_EVENT_QUIT;
		}
		break;

	default:
		return SETTINGS_EVENT_NONE;
	}
	return SETTINGS_EVENT_NONE;
}

/*
 * this function hide the settings window.
 * this function doesn't return nothing.
 */
void spSettingsWindowHide(SPSettingsWin* src) {
	SDL_HideWindow(src->SettingsWindow);
}

/*
 * this function show the settings window.
 * this function doesn't return nothing.
 */
void spSettingsWindowShow(SPSettingsWin* src) {
	SDL_ShowWindow(src->SettingsWindow);
}



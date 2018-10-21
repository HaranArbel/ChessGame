/*
 * SettingsWin.h
 *
 */

#ifndef GRAPHICS_SETTINGSWIN_H_
#define GRAPHICS_SETTINGSWIN_H_
#include <SDL.h>
#include <SDL_video.h>
#include "SPBoard.h"
#include "SPSimpleButton.h"

#define ONE_PLAYER_BN 0
#define TWO_PLAYERS_BN 1
#define NEXT_BN 2
#define START_BN 3
#define BACK_BN 4
#define NOOB_BN 5
#define EASY_BN 6
#define MODER_NB 7
#define HARD_BN 8
#define USER_COLOR_B_BN 9
#define USER_COLOR_W_BN 10

#define GAME_MODE_WIDG_N 4
#define DIFFIC_WIDG_N 4
#define LOAD_WIDG_N
#define USER_COLOR_WIDG_N 4

//this enum include all the situations of settings that we could handle.
typedef enum{
	SP_GAME_MODE_MENU,
	SP_DIFFICULTY_MENU,
	SP_USER_COLOR_MENU
}SP_SETTINGS_MENU;

// this enum save all the options of the events that could happened.
typedef enum {
	SETTINGS_EVENT_ONE_PLAYER,
	SETTINGS_EVENT_TWO_PLAYERS,
	SETTINGS_EVENT_NEXT,
	SETTINGS_EVENT_BACK,
	SETTINGS_EVENT_START,
	SETTINGS_NOOB_LEVEL,
	SETTINGS_EASY_LEVEL,
	SETTINGS_MODERATE_LEVEL,
	SETTINGS_HARD_LEVEL,
	SETTINGS_WHITE_USER_COLOR,
	SETTINGS_BLACK_USER_COLOR,
	SETTINGS_EVENT_QUIT,
	SETTINGS_EVENT_INVALID_ARGUMENT,
	SETTINGS_EVENT_NONE,
	SETTINGS_EVENT_UPDATE,
} SP_SETTINGS_EVENT;

// this struct consists all the relevant fields of the setting window.
typedef struct{
	SDL_Window* SettingsWindow;
	SDL_Renderer* SettingsRenderer;
	Widget** widgets;
	int numOfWidgets;
	bool first;
	int gameMode;
	int difficulty;
	int userColor;
	int SettingsMenu;
}SPSettingsWin;


/*
 * this function creat the window of the settings, it includes all the options of the settings,
 * it advanced step by setp according to the choose of the user and put on the window the
 * relevant buttons and widegets.
 * the function return a referece to SPSettingsWin.
 */
SPSettingsWin* spSettingsWindowCreate();

/*
 * this function creat the widgets of this specific setting window.
 * the fucntion return an refernce to an array of the relevenat widgets of this window.
 */
Widget** spCreateSettingsWindowWidgets(SDL_Renderer* renderer);

/*the function draw a window of settings.
 * this function doesn't return nothing.
 */
void spSettingsWindowDraw(SPSettingsWin*);


/*this function free the memory of all the things that relevant to the settings window.
 * this function doesn't return nothing.
 */
void spSettingsWindowDestroy(SPSettingsWin*);
/*
 * this function destroy an array of widgets.
 * the function doesn't return nothing.
 */
void spSettingsWindowWidgetsDestroy(Widget** widgets);

/*
 * this function control all the events in the setting window and operate them.
 * the function returns an message: SP_SETTINGS_EVENT, that indicate what exectly happened.
 */
SP_SETTINGS_EVENT spSettingsWindowHandleEvent(SPSettingsWin* src, SDL_Event* event);


/*
 * this function hide the settings window.
 * this function doesn't return nothing.
 */
void spSettingsWindowHide(SPSettingsWin* src);

/*
 * this function show the settings window.
 * this function doesn't return nothing.
 */
void spSettingsWindowShow(SPSettingsWin* src);


#endif /* GRAPHICS_SETTINGSWIN_H_ */


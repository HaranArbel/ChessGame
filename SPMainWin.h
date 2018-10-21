/*
 * mainWin.h
 */

#ifndef GRAPHICS_MAINWIN_H_
#define GRAPHICS_MAINWIN_H_
#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include <stddef.h>

#include "SPSimpleButton.h"

#define NEW_GAME_BN 0
#define LOAD_BN 1
#define QUIT_BN 2
// this enum save all the options of the events that could happened.
typedef enum {
	SP_MAIN_NEW_GAME,
	SP_MAIN_QUIT,
	SP_MAIN_LOAD,
	SP_MAIN_INVALID_ARGUMENT,
	SP_MAIN_NONE

}SP_MAIN_EVENT;

// this struct consists all the relevant fields of the main menu window.
typedef struct {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	Widget** widgets;
	int numOfWidgets;
	bool first;
} SPMainWin;

/*
 * this function creat a window of main menu, it includes the the limits of the window and put on the window the
 * relevant buttons and widegets.
 * the function return a referece to SPMainWin.
 */
SPMainWin* spMainWindowCreate();
/*
 * this function creat the widgets of this specific window.
 * the fucntion return an refernce to an array of the relevenat widgets of this window.
 */
Widget** spCreateMainWindowWidgets(SDL_Renderer* renderer);


/*the function draw a window of main menu.
 * this function doesn't return nothing.
 */
void spMainWindowDraw(SPMainWin* src);


/*this function free the memory of all the things that relevant to the main menu window.
 * this function doesn't return nothing.
 */
void spMainWindowDestroy(SPMainWin* src);

/*
 * this function destroy an array of widgets.
 * the function doesn't return nothing.
 */
void spMainWindowWidgetsDestroy(Widget** widgets);
/*
 * this function control all the events in the main menu window and operate them.
 * the function returns an message: SP_MAIN_EVENT, that indicate what exectly happened.
 */
SP_MAIN_EVENT spMainWindowHandleEvent(SPMainWin* src, SDL_Event* event);

/*
 * this function hide the main menu window.
 * this function doesn't return nothing.
 */
void spMainWindowHide(SPMainWin* src);

/*
 * this function show the main menu window.
 * this function doesn't return nothing.
 */
void spMainWindowShow(SPMainWin* src);

#endif /* GRAPHICS_MAINWIN_H_ */

/*
 * LoadWin.h
 *
 */

#ifndef GRAPHICS_LOADWIN_H_
#define GRAPHICS_LOADWIN_H_
#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "SPSimpleButton.h"
#include "SPWidget.h"
#include "MainGame.h"


#define LOAD_WIN_WIDG_N 7
#define SLOTS_NUM 5

#define SLOT1_BN 0
#define SLOT2_BN 1
#define SLOT3_BN 2
#define SLOT4_BN 3
#define SLOT5_BN 4
#define LOADWIN_BACK_BN 5
#define LOADWIN_LOAD_BN 6

// this enum save all the options of the events that could happened.
typedef enum {
	LOAD_EVENT_SLOT_1,
	LOAD_EVENT_SLOT_2,
	LOAD_EVENT_SLOT_3,
	LOAD_EVENT_SLOT_4,
	LOAD_EVENT_SLOT_5,
	LOAD_EVENT_QUIT,
	LOAD_EVENT_BACK,
	LOAD_EVENT_START,
	LOAD_EVENT_INVALID_ARGUMENT,
	LOAD_EVENT_NONE,
	LOAD_EVENT_UPDATE,
} SP_LOAD_EVENT;

// this struct consists all the relevant fields of the load window.
typedef struct{
	SDL_Window* loadWindow;
	SDL_Renderer* loadRenderer;
	Widget** widgets;
	int numOfWidgets;
	bool first;
	int gameToLoad;
	bool isValidLoad;
}SPLoadWin;


/*
 * this function creat a window of load, it includes the the limits of the window and put on the window the
 * relevant buttons and widegets.
 * the function return a referece to SPMainWin.
 */
SPLoadWin* spLoadWindowCreate();

/*
 * this function creat the widgets of this specific load window.
 * the fucntion return an refernce to an array of the relevenat widgets of this window.
 */
Widget** spCreateLoadWindowWidgets(SDL_Renderer* renderer);

/*the function draw a window of load.
 * this function doesn't return nothing.
 */
void spLoadWindowDraw(SPLoadWin* src);

/*this function free the memory of all the things that relevant to the load window.
 * this function doesn't return nothing.
 */
void spLoadWindowDestroy(SPLoadWin* src);

/*
 * this function destroy an array of widgets.
 * the function doesn't return nothing.
 */
void spLoadWindowWidgetsDestroy(Widget** widgets);

/*
 * this function control all the events in the load window and operate them.
 * the function returns an message: SP_LOAD_EVENT, that indicate what exectly happened.
 */
SP_LOAD_EVENT spLoadWindowHandleEvent(SPLoadWin* src, SDL_Event* event);

/*
 * this function give us the path of the load game, it gets game to load- int,
 * and give us an address of the load game.
 * @param gameToLoad- int that mention the game
 * return us a string the represent the path.
 */
char* spGetLoadPath(int gameToLoad);

/*
 * 	Checks if the given path exists or not.
 *  @param-path of a file(or doesn't exists).
 *  @return
 *  true if the path exists, else-false.
 */
bool spIsValidLoadPath(char* path);

#endif /* GRAPHICS_LOADWIN_H_ */

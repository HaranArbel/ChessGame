/*
 * SPChessGUIManager.h
 *
 */

#ifndef GRAPHICS_GUIMANAGER_H_
#define GRAPHICS_GUIMANAGER_H_
#include <stddef.h>
#include <stdbool.h>
#include "SPGameWin.h"
#include "MainGUI.h"
#include "SPLoadWin.h"
#include "SPMainWin.h"
#include "SPSettingsWin.h"


// this enum save all the options of the windows we can be in during the running of the program.
typedef enum{
	SP_MAIN_WINDOW_ACTIVE,
	SP_GAME_WINDOW_ACTIVE,
	SP_SETTINGS_WINDOW_ACTIVE,
	SP_LOAD_WINDOW_ACTIVE,
}SP_ACTIVE_WINDOW;

// this enum save all the options of the events that could happened.
typedef enum{
	SP_MANAGER_QUIT,
	SP_MANAGER_NONE,
	SP_MANAGER_UPDATE,

}SP_MANAGER_EVENET;

// this struct consists all the relevant fields of the gui manager.
typedef struct {
	SPSettingsWin* settingsWin;
	SPLoadWin* loadWin;
	SPGameWin* gameWin;
	SPMainWin* mainWin;
	SP_ACTIVE_WINDOW activeWin;
} SPGuiManager;

/*
 * this function creat the gui manager who responsible of control all the windows of the game in gui situation.
 * the function return a referece to SPGuiManager.
 */
SPGuiManager* spManagerCreate();


/*this function free the memory of all the things that relevant to the gui manager.
 * this function doesn't return nothing.
 */
void spManagerDestroy(SPGuiManager* src);

/*the function draw the specific window that active now.
 * this function doesn't return nothing.
 */
void spManagerDraw(SPGuiManager* src);

/*
 * this function azalyze the events that occured at one of the windows window, and opeate according them.
 * the function returns an message: SP_MANAGER_EVENET, that indicate what exectly happened.
 */
SP_MANAGER_EVENET spHandleManagerEvent(SPGuiManager* src, SDL_Event* event);

/*
 * this function azalyze the events that occured at main menu window, and opeate according them.
 * the function returns an message: SP_MANAGER_EVENET, that indicate what exectly happened.
 */
SP_MANAGER_EVENET spHandleManagerDueToMainEvent(SPGuiManager* src,SP_MAIN_EVENT event);

/*
 * this function azalyze the events that occured at settings window, and opeate according them.
 * the function returns an message: SP_MANAGER_EVENET, that indicate what exectly happened.
 */
SP_MANAGER_EVENET spHandleManagerSettingsEvent(SPGuiManager* src, SP_SETTINGS_EVENT event);

/*
 * this function azalyze the events that occured at load window, and opeate according them.
 * the function returns an message: SP_MANAGER_EVENET, that indicate what exectly happened.
 */
SP_MANAGER_EVENET spHandleManagerDueToLoadEvent(SPGuiManager* src,SP_LOAD_EVENT event);

/*
 * this function azalyze the events that occured at game window, and opeate according them.
 * the function returns an message: SP_MANAGER_EVENET, that indicate what exectly happened.
 */
SP_MANAGER_EVENET spHandleManagerDueToGameEvent(SPGuiManager* src,SP_GAME_EVENT event);

/*
 * this function azalyze the events that occured at othe save box, and opeate according them.
 * the function returns an int, that indicate what exectly happened.
 */
int spHandleSaveBox(bool saved);

/*
 * the function save game at the mode of gui and put it at the relevant slot.
 * @param src - SPGuiManager : the gui manager.
 * the function doesn't return nothing.
 */
void spSaveGame(SPGuiManager* src);


#endif /* GRAPHICS_GUIMANAGER_H_ */

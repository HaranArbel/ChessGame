/*
 * GUIManager.c
 *
 */

#include "SPGUIManager.h"


SPGuiManager* spManagerCreate(){

	SPGuiManager* res = (SPGuiManager*) malloc(sizeof(SPGuiManager));
	if(res == NULL){
		return NULL;
	}
	res->mainWin = spMainWindowCreate();
	if(res->mainWin == NULL){
		free(res);
		return NULL ;
	}
	res->gameWin = NULL;
	res->loadWin = NULL;
	res->settingsWin = NULL;
	res->activeWin = SP_MAIN_WINDOW_ACTIVE;

	return res;
}


void spManagerDestroy(SPGuiManager* src){
	if(src == NULL){
		return;
	}
	if(src->activeWin == SP_GAME_WINDOW_ACTIVE){
		spGameWindowDestroy(src->gameWin);
	}
	spMainWindowDestroy(src->mainWin);
	free(src);
}


void spManagerDraw(SPGuiManager* src){
	if(src == NULL){
		return;
	}
	switch(src->activeWin){
	case SP_MAIN_WINDOW_ACTIVE:
		spMainWindowDraw(src->mainWin);
		break;

	case SP_SETTINGS_WINDOW_ACTIVE:
		spSettingsWindowDraw(src->settingsWin);
		break;

	case SP_LOAD_WINDOW_ACTIVE:
		spLoadWindowDraw(src->loadWin);
		break;

	case SP_GAME_WINDOW_ACTIVE:
		spGameWindowDraw(src->gameWin,true);
		break;
	}
}

SP_MANAGER_EVENET spHandleManagerDueToMainEvent(SPGuiManager* src, SP_MAIN_EVENT event){
	if(src == NULL){
		return SP_MANAGER_NONE;
	}
	switch(event){

	case SP_MAIN_NEW_GAME:
		spMainWindowHide(src->mainWin);
		src->settingsWin = spSettingsWindowCreate();
		src->settingsWin->SettingsMenu = SP_GAME_MODE_MENU;
		if(src->settingsWin == NULL){
			printf("Couldn't create settings window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_SETTINGS_WINDOW_ACTIVE;
		return SP_MANAGER_UPDATE;
		break;

	case SP_MAIN_LOAD:
		spMainWindowHide(src->mainWin);
		src->loadWin = spLoadWindowCreate();
		if(src->loadWin == NULL){
			printf("Couldn't create load window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_LOAD_WINDOW_ACTIVE;
		return SP_MANAGER_UPDATE;
		break;

	case SP_MAIN_QUIT:
		return SP_MANAGER_QUIT;
		break;

	default:
		return SP_MANAGER_NONE;
	}
}

//
SP_MANAGER_EVENET spHandleManagerDueToSettingsEvent(SPGuiManager* src, SP_SETTINGS_EVENT event){
	if(src == NULL){
		return SP_MANAGER_NONE;
	}

	SPChessBoard* chessBoard;
	switch(event){
	case SETTINGS_EVENT_UPDATE:
		return SP_MANAGER_UPDATE;
		break;

	case SETTINGS_EVENT_START:
		chessBoard = spInitGame();
		chessBoard->gameMode = src->settingsWin->gameMode;
		chessBoard->minimaxDepth = src->settingsWin->difficulty;
		chessBoard->userColor = src->settingsWin->userColor;
		src->gameWin = spGameWindowCreate(chessBoard);
		src->activeWin = SP_GAME_WINDOW_ACTIVE;
		spSettingsWindowDestroy(src->settingsWin);
		return SP_MANAGER_UPDATE;
		break;


	case SETTINGS_EVENT_BACK:
		spSettingsWindowDestroy(src->settingsWin);
		spMainWindowShow(src->mainWin);
		src->activeWin = SP_MAIN_WINDOW_ACTIVE;
		src->settingsWin-> SettingsMenu = SP_GAME_MODE_MENU;
		return SP_MANAGER_UPDATE;
		break;

	case SETTINGS_EVENT_QUIT:
		return SP_MANAGER_QUIT;
		break;

	default:
		return SP_MANAGER_NONE;
	}
}


SP_MANAGER_EVENET spHandleManagerDueToLoadEvent(SPGuiManager* src, SP_LOAD_EVENT event){
	SPChessBoard* chessBoard;
	FILE* ifp;
	char* loadPath;
	if(src == NULL){
		return SP_MANAGER_NONE;
	}
	switch(event){
	case LOAD_EVENT_UPDATE:
		return SP_MANAGER_UPDATE;
		break;

	case LOAD_EVENT_BACK:
		spLoadWindowDestroy(src->loadWin);
		if(src->gameWin != NULL){
			spGameWindowShow(src->gameWin);
			src->activeWin = SP_GAME_WINDOW_ACTIVE;
		}
		else{
			spMainWindowShow(src->mainWin);
			src->activeWin = SP_MAIN_WINDOW_ACTIVE;
		}
		return SP_MANAGER_UPDATE;
		break;

	case LOAD_EVENT_START:
		if(src->gameWin != NULL){
			spGameWindowDestroy(src->gameWin);
		}
		loadPath = spGetLoadPath(src->loadWin->gameToLoad+1);
		if ((ifp = fopen(loadPath, "r")) == NULL){
		}
		chessBoard = spCreateNewBoard();
		spLoadSettingsFromFile(chessBoard,ifp);
		free(loadPath);
		spLoadWindowDestroy(src->loadWin);
		src->gameWin = spGameWindowCreate(chessBoard);
		if(src->gameWin == NULL){
			printf("Couldn't create game window\n");
			return SP_MANAGER_QUIT;
		}
		src->gameWin->saved = true;
		spPrintBoard(src->gameWin->chessBoard);
		src->activeWin = SP_GAME_WINDOW_ACTIVE;
		return SP_MANAGER_UPDATE;
		break;

	case LOAD_EVENT_QUIT:
		return SP_MANAGER_QUIT;
		break;

	default:
		return SP_MANAGER_NONE;
	}
}



SP_MANAGER_EVENET spHandleManagerDueToGameEvent(SPGuiManager* src, SP_GAME_EVENT event){
	int i;

	if(src == NULL){
		return SP_MANAGER_NONE;
	}
	switch(event){

	case SP_GAME_EVENT_UPDATE:
		return SP_MANAGER_UPDATE;

	case SP_GAME_EVENT_MAIN_MENU:
		if(src->gameWin->saved == true){
			spGameWindowDestroy(src->gameWin);
			src->gameWin = NULL;
			src->activeWin = SP_MAIN_WINDOW_ACTIVE;
			spMainWindowShow(src->mainWin);
			return SP_MANAGER_UPDATE;
		}

		i = spHandleSaveBox(src->gameWin->saved);
		if (i == 1 || i == -1 ){
			spGameWindowDestroy(src->gameWin);
			src->gameWin = NULL;
			src->activeWin = SP_MAIN_WINDOW_ACTIVE;
			spMainWindowShow(src->mainWin);
			return SP_MANAGER_UPDATE;
		}
		if (i == 0){
			return SP_MANAGER_NONE;
		}
		if (i == 2){
			spSaveGame(src);
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Save","Saved!",NULL);
			src->gameWin->saved = false;
			spGameWindowDestroy(src->gameWin);
			src->gameWin = NULL;
			src->activeWin = SP_MAIN_WINDOW_ACTIVE;
			spMainWindowShow(src->mainWin);
			return SP_MANAGER_UPDATE;
		}

		break;

	case SP_GAME_EVENT_SAVE:
		src->gameWin->saved = true;
		spSaveGame(src);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Save","Saved!",NULL);

		break;

	case SP_GAME_EVENT_LOAD:
		spGameWindowHide(src->gameWin);
		src->loadWin = spLoadWindowCreate();
		if(src->loadWin == NULL){
			printf("Couldn't create load window\n");
			return SP_MANAGER_QUIT;
		}
		src->activeWin = SP_LOAD_WINDOW_ACTIVE;
		return SP_MANAGER_UPDATE;

	case SP_GAME_EVENT_QUIT:
		if(src->gameWin->saved == true){
			return SP_MANAGER_QUIT;
		}
		else{
			i = spHandleSaveBox(src->gameWin->saved);
			if (i == 1 || i == -1 ){
				spGameWindowDestroy(src->gameWin);
				src->gameWin = NULL;
				return SP_MANAGER_QUIT;
			}
			if (i == 0){
				return SP_MANAGER_NONE;
			}
			if (i == 2){
			spSaveGame(src);
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Save","Saved!",NULL);
			src->gameWin->saved = false;
			spGameWindowDestroy(src->gameWin);
			src->gameWin = NULL;
			return SP_MANAGER_QUIT;
			}
			return SP_MANAGER_NONE;
		}
		break;

	case SP_GAME_EVENT_GAMEOVER:
		spGameWindowDestroy(src->gameWin);
		src->gameWin = NULL;
		src->activeWin = SP_MAIN_WINDOW_ACTIVE;
		spMainWindowShow(src->mainWin);
		return SP_MANAGER_UPDATE;
	break;

	default:
		return SP_MANAGER_NONE;
	}
	return SP_MANAGER_NONE;
}


SP_MANAGER_EVENET spHandleManagerEvent(SPGuiManager* src, SDL_Event* event){
	if(src == NULL || event == NULL){
		return SP_MANAGER_NONE;
	}
	SP_MAIN_EVENT mainEvent;
	SP_SETTINGS_EVENT settEvent;
	SP_LOAD_EVENT loadEvent;
	SP_GAME_EVENT gameEvent;
	switch(src->activeWin){

	case SP_MAIN_WINDOW_ACTIVE:
		if(src->mainWin->first == true){
			src->mainWin->first = false;
			spManagerDraw(src);
		}
		mainEvent = spMainWindowHandleEvent(src->mainWin, event);
		return spHandleManagerDueToMainEvent(src, mainEvent);
		break;
	case SP_SETTINGS_WINDOW_ACTIVE:
		if(src->settingsWin->first == true){
			src->settingsWin->first = false;
			spManagerDraw(src);
		}
		settEvent = spSettingsWindowHandleEvent(src->settingsWin, event);
		return spHandleManagerDueToSettingsEvent(src, settEvent);
		break;
//
	case SP_LOAD_WINDOW_ACTIVE:
		if(src->loadWin->first == true){
			src->loadWin->first = false;
			spManagerDraw(src);
		}
		loadEvent = spLoadWindowHandleEvent(src->loadWin, event);
		return spHandleManagerDueToLoadEvent(src, loadEvent);
		break;

	case SP_GAME_WINDOW_ACTIVE:
		if(src->gameWin->startGame == true){
			src->gameWin->startGame = false;
			if(src->gameWin->chessBoard->userColor==BLACK){
				switchTurns(src->gameWin->chessBoard);
				int compColor = getCurrentPlayerColor(src->gameWin->chessBoard);
				Move* move = spGetComputerMoveForGui(src->gameWin->chessBoard, compColor, src->gameWin->chessBoard->minimaxDepth);
				if(src->gameWin->chessBoard->board[move->to.row][move->to.col] != EMPTY_PIECE){
					for(int k=GAMEWIM_WIDG_N; k<src->gameWin->numOfWidgets; k++){
						if(((SPPieceButton*)src->gameWin->widgets[k]->data)->location->x==move->to.col*90 && (((SPPieceButton*)src->gameWin->widgets[k]->data)->location->y==(7-move->to.row)*90)){
							((SPPieceButton*)src->gameWin->widgets[k]->data)->on=false;
						}
					}
				}
				spExecuteMove(src->gameWin->chessBoard,move->from,move->to);
				for(int k = GAMEWIM_WIDG_N; k < src->gameWin->numOfWidgets; k++){
					if(((SPPieceButton*)src->gameWin->widgets[k]->data)->location->x==move->from.col*90 && (((SPPieceButton*)src->gameWin->widgets[k]->data)->location->y==(7-move->from.row)*90)){
						((SPPieceButton*)src->gameWin->widgets[k]->data)->location->x = move->to.col*90;
						((SPPieceButton*)src->gameWin->widgets[k]->data)->location->y = (7 - move->to.row)*90;
					}
				}
			}
		}
			spManagerDraw(src);
		gameEvent = spGameWindowHandleEvent(src->gameWin, event);
		return spHandleManagerDueToGameEvent(src, gameEvent);
		break;

	default:
		return SP_MANAGER_NONE;
	}
}

void spSaveGame(SPGuiManager* src){
	int i,ren;
	char* newPath;

	char* currentPath = spGetLoadPath(SLOT5_BN);
	if(spIsValidLoadPath(currentPath)==true){
		remove(currentPath);
	}
	free(currentPath);
	for(i = SLOT4_BN; i >= SLOT1_BN; i--){

		currentPath = spGetLoadPath(i);

		if(spIsValidLoadPath(currentPath) == true){
			newPath = spGetLoadPath(i+1);
			ren = rename(currentPath,newPath);

			if(ren!=0){
				printf("rename has failed: %s\n",currentPath);}
				free(newPath);
			}
		free(currentPath);
		}
	currentPath = spGetLoadPath(SLOT1_BN);
	spSaveFile(src->gameWin->chessBoard,currentPath);
	free(currentPath);
}

int spHandleSaveBox(bool saved){
	if (saved == false){
		const SDL_MessageBoxButtonData buttons[] = {
		        { /* .flags, .buttonid, .text */        0, 0, "cancel" },
		        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "no" },
		        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "yes" },
		    };
		    const SDL_MessageBoxColorScheme colorScheme = {
		        { /* .colors (.r, .g, .b) */
		            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
		            { 255,   0,   0 },
		            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
		            {   0, 255,   0 },
		            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
		            { 255, 255,   0 },
		            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
		            {   0,   0, 255 },
		            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
		            { 255,   0, 255 }
		        }
		    };
		    const SDL_MessageBoxData messageboxdata = {
		        SDL_MESSAGEBOX_INFORMATION, /* .flags */
		        NULL, /* .window */
		        "Save the game?", /* .title */
		        "Please select a button", /* .message */
		        SDL_arraysize(buttons), /* .numbuttons */
		        buttons, /* .buttons */
		        &colorScheme /* .colorScheme */
		    };
		    int buttonid = -1;
		    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		        SDL_Log("error displaying message box");
		        return 1;
		    }
		    return buttonid;
	}
    return -1;
}


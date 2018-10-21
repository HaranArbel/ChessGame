/*
 * gameWin.h
 *
 */
#ifndef GRAPHICS_GAMEWIN_H_
#define GRAPHICS_GAMEWIN_H_
#include <SDL.h>
#include <SDL_video.h>
#include <stddef.h>
#include "MainGame.h"
#include "SPSimpleButton.h"
#include "SPPieceButton.h"

#define GAMEWIM_WIDG_N 6

// the button numbers
#define UNDO_BN 0
#define SAVE_BN 1
#define GAMEWIN_LOAD_BN 2
#define RESET_BN 3
#define MAIN_MENU_BN 4
#define GAMEWIN_QUIT_BN 5

// this enum save all the options of the events that could happened at window game.
typedef enum {
SP_GAME_EVENT_TIE,
SP_GAME_EVENT_GAMEOVER,
SP_GAME_EVENT_RESET,
SP_GAME_EVENT_SAVE,
SP_GAME_EVENT_LOAD,
SP_GAME_EVENT_UNDO,
SP_GAME_EVENT_MAIN_MENU,
SP_GAME_EVENT_QUIT,
SP_GAME_EVENT_INVALID_ARGUMENT,
SP_GAME_EVENT_NONE,
SP_GAME_EVENT_UPDATE

} SP_GAME_EVENT;

// this struct consists all the relevant fields of the game window.
typedef struct{
	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;
	SDL_Texture* chessBoardTexture;
	Widget** widgets;
	int numOfWidgets;
	bool startGame;
	bool saved;
	bool backToMain;
	SPChessBoard* chessBoard;
}SPGameWin;

/*
 * this function creat the window of the game, it includes all the parametrs of the game,
 * in order to create a window of the game.
 * @param chessBoard - a reference to SPChessBoard
 * the function return a referece to SPGameWin.
 */
SPGameWin* spGameWindowCreate(SPChessBoard* chessBoard);

/*
 * this function creat the widgets of this specific game window.
 * the fucntion return an refernce to an array of the relevenat widgets of this window.
 */
Widget** spCreateGameWindowWidgets(SDL_Renderer* gameRenderer,SPChessBoard* chessBoard);

/*
 * this function creat the widgets of this specific game window.
 * the fucntion return an refernce to an array of the relevenat widgets of this window.
 */
void spGameWindowWidgetsDestroy(Widget** widgets,SPChessBoard* chessBoard);

/*the function draw a window of the game.
 * this function doesn't return nothing.
 */
void spGameWindowDraw(SPGameWin* src,bool update);

/*the function ןis for updating the draw of a the game window.
 * this function doesn't return nothing.
 */
void updateGameForDraw(SPGameWin* src);

/*this function free the memory of all the things that relevant to the game window.
 * this function doesn't return nothing.
 */
void spGameWindowDestroy(SPGameWin*);

/*
 * this function hide the game window.
 * this function doesn't return nothing.
 */
void spGameWindowHide(SPGameWin* src);

/*
 * this function show the game window.
 * this function doesn't return nothing.
 */
void spGameWindowShow(SPGameWin* src);

/*the function ןis for updating the draw of a the game window.
 * this function doesn't return nothing.
 */
void updateForDrawingTheGame(SPGameWin* src);
/*
 * this function control all the events in the game window and operate them.
 * the function returns an message: SP_GAME_EVENT, that indicate what exectly happened.
 */
SP_GAME_EVENT spGameWindowHandleEvent(SPGameWin* src, SDL_Event* event);

/*
 * this function control all the events at the situation the mouse was press button up the game window and operate them.
 * the function returns an message: SP_GAME_EVENT, that indicate what exectly happened.
 */
SP_GAME_EVENT spHandleButtonUpEvent(SPGameWin* src, SDL_Event* event);

/*
 * this function control all the events at the situation the reset button was preesed, the game window and operate them.
 * the function returns an message: SP_GAME_EVENT, that indicate what exectly happened.
 */
SP_GAME_EVENT spHandleResetEventGame(SPGameWin* src);

/*
 * this function control all the events at the situation the mouse was press button down the game window and operate them.
 * the function returns an message: SP_GAME_EVENT, that indicate what exectly happened.
 */
SP_GAME_EVENT spHandleButtonDownEvent(SPGameWin* src,SDL_Event* event);

/*
 * this function get the right move for the computer based on the
 * minimax algorithm and make it appear in GUI mode
 */
Move * spGetComputerMoveForGui(SPChessBoard * chess_board, int computer_color, int minimax_depth);//todo document

/*
 * this function control all the events at the situation we have to check if
 * we get check or check mate or cheak tie. the game window and operate them.
 * the function returns an message: SP_GAME_EVENT, that indicate what exectly happened.
 */
SP_GAME_EVENT spHandleCheckCheckmateOrTie(SPGameWin* src, int res);

#endif /* GRAPHICS_GAMEWIN_H_ */

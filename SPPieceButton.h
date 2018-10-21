/*
 * PiecesButton.h
 *
 */

#ifndef GRAPHICS_PIECESBUTTON_H_
#define GRAPHICS_PIECESBUTTON_H_
#include <SDL.h>
#include <SDL_video.h>
#include "SPBoard.h"
#include "SPCommon.h"
#include "SPWidget.h"

#define BOARD_SHIFT_DOWN 15
#define BOARD_SHIFT_RIGHT 35

#define TOTAL_NUM_OF_CHESS_PIECES 32
#define PIECE_NAME_LEN 15
#define STR_ENDING_LEM 5


typedef struct piecebutton_t SPPieceButton;
// the struct is in order to hold a piece.
struct piecebutton_t{
	SDL_Texture* buttonTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	bool on;
};


/*
 * this function creat a button of piece,of the game, as pwan, rock and the others piece.
 * the function return a referece to Widget.
 */
Widget* spCreatePieceButton(SDL_Renderer* windowRender, SDL_Rect* location, const char* image, bool on);

/*
 * this function destroy an array of widgets that are pieces of the game.
 * the function doesn't return nothing.
 */
void spDestroyPieceButton(Widget* src);

/*
 * the function put texture of the button on the peace- in means it put an image on specific widget.
 * the function doesn't return nothing.
 */
void spSetPieceButtonTexture(Widget* src,const char* image);

/*
 * this function control all the events that relevant to the piece window at the game window.
 * return 1 if it success, and 0 otherwise.
 */
int spHandlePieceButtonEvenet(Widget* src, SDL_Event* event);

/*
 * this function draw a piece on the the window of the game.
 * @param src -Widget- that we will draw.
 * the function doesn't return nothing.
 */
void spDrawPieceButton(Widget* src);

/*
 * the traget of this function is to get the path of the image piece.
 * it means that we gave the function the char that represent the piece and it return
 * us the path for that image.
 * @param piece_name -char*- that represent piece.
 * the function return the path to the image of the piece, as char*
 */
char* spGetPieceImagePath(char* piece_name);

/*
 * the function get unsiged int piece and it tell us which piece it is - string of that.
 * @param piece- unsigned int - that represent piece.
 * return char* the name of the piece.
 */
char* spGetPieceName(unsigned int piece);



#endif /* GRAPHICS_PIECESBUTTON_H_ */

/*
 * board.h
 *
 *
 */

#ifndef SPBOARD_H_
#define SPBOARD_H_

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>


#include "SPArrayList.h"

#define ONE_PLAYER 1
#define TWO_PLAYERS 2

#define BLACK 0
#define WHITE 1

#define BOTTOM_ROW 0
#define TOP_ROW 7

#define MAX_HISTORY 6


// chess pieces

#define EMPTY_PIECE     0
#define PAWN_PIECE 		1
#define ROOK_PIECE 		2
#define KNIGHT_PIECE 	3
#define BISHOP_PIECE 	4
#define QUEEN_PIECE 	5
#define KING_PIECE	 	6

#define PIECE_BIT_MASK	7

#define WHITE_PIECE 8
#define BLACK_PIECE 16

#define PIECE_COLOR_BIT_MASK 24

#define PAWN_VALUE		32
#define ROOK_VALUE 		64
#define KNIGHT_VALUE 	128
#define BISHOP_VALUE 	256
#define QUEEN_VALUE		512
#define KING_VALUE		1024

#define MAX_PAWN_MOVES 4
#define MAX_ROOK_MOVES 14
#define MAX_BISHOP_MOVES 13
#define MAX_KNIGHT_MOVES 8
#define MAX_QUEEN_MOVES 27
#define MAX_KING_MOVES 8

/* this struct represent the chess game: the board, the history, the difficulty, amount of players,
 * the color of the user, who has to play now, and if start after load.
 */
typedef struct SPChessBoard {
	unsigned int board[8][8];
	SPArrayList* history;
	int minimaxDepth;
	int gameMode;
	int userColor;
	int turn;
	bool isLoadedGame;

}SPChessBoard;

// Error handling message
#define errorHandling(function) (printf("ERROR: %s has failed", function));

/*the function get chess board and initilize a new game.Set all the pieces in the right places
 * according to standard chess rules
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spInitBoard(SPChessBoard* chess_board);


/*the function get unsiged int of piece and return char that represnt the specific piece.
 * @param piece -unsigend int that represent piece.
 * the function returns the character that represents a certain piece
 */
char spGetPieceChar(unsigned int piece);

/*the function get char of piece and return unsiged int that represnt the specific piece.
 * @param piece -char that represent piece.
 * the function returns the unsiged int that represents a certain piece
 */
unsigned int spGetPieceFromChar(char c);

/*the function get unsigned int and print the char of the piece that it represent.
 * @param piece -unsigned int that represent piece.
 * the function doesn't return nothing.
 */
void spPrintPieceChar(unsigned int piece);

/*the function get chess board and print the borad of the game.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spPrintBoard(SPChessBoard *chess_board);

/*the function crerat a new board of the game.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
SPChessBoard* spCreateNewBoard();

/*the function frees the board and the history list
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spDestroyBoard(SPChessBoard * chess_board);

/*
 * this function set a squre, it replace the row and the col of the squre with the parametrs that we get.
 * @param square - a reference to Squre.
 * @param row - int that represnt the row.
 * @param col - int that represnt the col.
 * the function return a referece to SPGameWin.
 */
void spSetSquare(Square* square, int row, int col);

/*the function check of the position in the Square that the function get is legal.
 * @param s - Squre.
 * the function return 1 if the place of the squre is legal and 0 otherwise.
 */
bool spIsValidPosition(Square s);

/*the function execute the move from src to dst and updated the board.
 * @param chessBoard - a reference to chess board.
 * @src - Square of the source move.
 * @dst - Square of the destination move.
 * the function doesn't return nothing.
 */
Move * spCreateMove(SPChessBoard * chess_board, Square src, Square dst);

/*the function copy the board of the game and return another reference to SPChessBoard.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
SPChessBoard * spCopyBoard(SPChessBoard* chess_board);

/*the function print the move that we sent him.
 * @param move - a reference to move.
 * the function doesn't return nothing.
 */
void print_move(Move *move);//todo remove

/*the function frees the move.
 * @param move - a reference to move.
 * the function doesn't return nothing.
 */
void spDestroyMove(Move *move);//todo does this function exist?
#endif /* SPBOARD_H_ */

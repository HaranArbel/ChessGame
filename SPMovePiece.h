/*
 * move_piece.h
 *
 */

#ifndef SPMOVEPIECE_H_
#define SPMOVEPIECE_H_

#include <string.h>
#include "SPBoard.h"



#define SP_MAX_LINE_LENGTH 1024

#define ILLEGAL_MOVE_MESSAGE "Illegal move\n"
#define INVALID_POSITION_MESSAGE "Invalid position on the board\n"
#define NOT_YOUR_PIECE_MESSAGE "The specified position does not contain your piece\n"

// Prints the computer's move
#define printCompMove(piece_name) (printf("Computer: move %s at ", piece_name));


#define NONE 0
#define CHECK 1
#define WHITE_WON 2
#define BLACK_WON 3
#define TIE 4

typedef enum MOVE_MESSAGE {
	INVALID_POSITION,
	NO_LEGAL_PIECE,
	ILLEGAL_MOVE,
	LEGAL_MOVE,
} MOVE_MESSAGE;

/*This function checks if a certain move is a legal
 * pawn move according to chess rules.
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 	the function returns 1 if the move is legal
 * 	otherwise, returns 0.
 */
int spIsLegalPawnMove(Square src, Square dst, SPChessBoard *chess_board);

/*This function checks if a certain move is a legal
 * bishop move according to chess rules.
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 	the function returns 1 if the move is legal
 * 	otherwise, returns 0.
 */
int spIsLegalBishopMove(Square src, Square dst, SPChessBoard *chess_board);

/*This function checks if a certain move is a legal
 * rook move according to chess rules.
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 	the function returns 1 if the move is legal
 * 	otherwise, returns 0.
 */
int spIsLegalRookMove(Square src, Square dst, SPChessBoard *chess_board);

/*This function checks if a certain move is a legal
 * knight move according to chess rules.
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 	the function returns 1 if the move is legal
 * 	otherwise, returns 0.
 */
int spIsLegalKnightMove(Square src, Square dst);

/*This function checks if a certain move is a legal
 * queen move according to chess rules.
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 	the function returns 1 if the move is legal
 * 	otherwise, returns 0.
 */
int spIsLegalQueenMove(Square src, Square dst, SPChessBoard *chess_board);

/*This function checks if a certain move is a legal
 * king move according to chess rules.
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 	the function returns 1 if the move is legal
 * 	otherwise, returns 0.
 */
int spIsLegalKingMove(Square src, Square dst, SPChessBoard *chess_board);

/*This function checks if a the path from the
 * source square to the destination square is
 * clear horizontally, to see if a piece is moving over other pieces
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 	the function returns 1 if the move is legal
 * 	otherwise, returns 0.
 */
int spIsPathClearHorizontally(Square src, Square dst, SPChessBoard *chess_board);

/*This function checks if a the path from the
 * source square to the destination square is
 * clear vertically, to see if a piece is moving over other pieces
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 	the function returns 1 if the move is legal
 * 	otherwise, returns 0.
 */
int spIsPathClearVertically(Square src, Square dst, SPChessBoard *chess_board);

/*This function checks if a the path from the
 * source square to the destination square is
 * clear diagonally, to see if a piece is moving over other pieces
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 	the function returns 1 if the move is legal
 * 	otherwise, returns 0.
 */
int spIsPathClearDiagonally(Square src, Square dst, SPChessBoard *SPChessBoard);

/*This function checks if the game is over
 * clear diagonally, to see if a piece is moving over other pieces
 * @param - chess_board - the game board
 * 			player_color - the color of the player
 * 			that we want that has just played
 * 	the function returns either CHECK if there is a check
 * 	TIE if the game is tied, WHITE_WON/BLACK_WON if
 * 	there is a winner or NONE if neither.
 */
int spIsGameOver(SPChessBoard *chess_board, int player_color);

/*This function checks if the player's king is
 * under check.
 * clear diagonally, to see if a piece is moving over other pieces
 * @param - chess_board - the game board
 * 			player_color - the color of the player
 * 			that we want to check
 * 	the function returns 1 if there is a check
 * 	otherwise, returns 0.
 */
int spIsChessCheck(SPChessBoard *chess_board, int player_color);
/*This function checks if making a move from the source
 * to the destination square will result in a chess check
 * meaning that the move isn't legal
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 			player_color - the color of the player
 * the function returns 1 if there is a check and 0 otherwise
 */
int spIsNextTurnCheck(SPChessBoard *chess_board, Square src, Square dst, int player_color);
/*This function checks if a move is legal
 * according to chess rules
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * 			player_color - the color of the player
 * the function returns 1 if the move is legal and 0 otherwise
 */
MOVE_MESSAGE spIsLegalMove(SPChessBoard *chess_board, Square src, Square dst, int user_color);
/*This function executes a move from the source
 * to the destination square
 * @param - src - the source sqaure
 * 			dst - the destination square
 * 			chess_board - the game board
 * the function returns nothing
 */
void spExecuteMove(SPChessBoard *chess_board, Square src, Square dst);
/*This function checks if there are still any legal
 * moves that the player can perform
 * @param - chess_board - the game board
 * 			player_color - the color of the player
 * the function returns 1 if there are still moves and 0 othwerwise
 */
int spNoMoreLegalMoves(SPChessBoard * chess_board, unsigned int player_color);
/*This function updates the history list with the
 * move that the player has performed
 * @param - move - the move that eas performed
 * 			chess_board - the game board
 * the function returns nothing
 */
void spUpdateHistory(Move *move, SPChessBoard *chess_board);
/*This function returns the number of all the
 * moves that the player can perform
 * @param - chess_board - the game board
 * 			player_color - the color of the player
 */
int spAllMovesAmount(SPChessBoard* chess_board, int player_color);
/*This function returns a list of all the
 * moves that the player can perform
 * @param - chess_board - the game board
 * 			player_color - the color of the player
 */
SPArrayList * spGetAllMoves(SPChessBoard* chess_board, int player_color);
/*This function returns a list of all the
 * moves that can be performed at a specified square
 * @param - chess_board - the game board
 * 			player_color - the color of the player
 * 			row - the row
 * 			col - the column
 */
SPArrayList * spGetMoves(SPChessBoard* chess_board, int row, int col, int player_color);
/*This function returns a list of all the legal moves that
 * the pawn in the source square can perform
 * @param - src - the source sqaure
 * 			player_color - the color of the current player
 * 			chess_board - the game board
 */
SPArrayList * spGetPawnMoves(SPChessBoard* chess_board, Square src, int player_color);
/*This function returns a list of all the legal moves that
 * the rook in the source square can perform
 * @param - src - the source sqaure
 * 			player_color - the color of the current player
 * 			chess_board - the game board
 */
SPArrayList * spGetRookMoves(SPChessBoard* chess_board, Square src, int player_color);
/*This function returns a list of all the legal moves that
 * the bishop in the source square can perform
 * @param - src - the source sqaure
 * 			player_color - the color of the current player
 * 			chess_board - the game board
 */
SPArrayList * spGetBishopMoves(SPChessBoard* chess_board, Square src, int player_color);
/*This function returns a list of all the legal moves that
 * the knight in the source square can perform
 * @param - src - the source sqaure
 * 			player_color - the color of the current player
 * 			chess_board - the game board
 */
SPArrayList * spGetKnightMoves(SPChessBoard* chess_board, Square src, int player_color);
/*This function returns a list of all the legal moves that
 * the queen in the source square can perform
 * @param - src - the source sqaure
 * 			player_color - the color of the current player
 * 			chess_board - the game board
 */
SPArrayList * spGetQueenMoves(SPChessBoard* chess_board, Square src, int player_color);
/*This function returns a list of all the legal moves that
 * the king in the source square can perform
 * @param - src - the source sqaure
 * 			player_color - the color of the current player
 * 			chess_board - the game board
 */
SPArrayList * spGetKingMoves(SPChessBoard* chess_board, Square src, int player_color);

/*This function prints the move of the computer
 * @param - move - the relevent move
 */
void spPrintMove(Move *move);

#endif /* SPMOVEPIECE_H_ */

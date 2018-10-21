/*
 * minimax.h
 *
 */

#ifndef SPMINIMAX_H_
#define SPMINIMAX_H_
#include "limits.h"
#include <stdbool.h>
#include "SPMovePiece.h"
#include "MainGame.h"


// Find and return the maximum between two items
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
// Find and return the minimum between two items
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

/*
 * this funtion returns the best move for the computer according
 * to the minimax algorithm
 *
 */
Move * spGetComputerMove(SPChessBoard * chess_board, int computerColor, int minimax_depth);
/*
 * this function is the minmax algorithm that
 * we use in order to find the best move for a player
 * the function is recursive
 */
int spMinimax(SPChessBoard * chess_board, int alpha, int beta, bool maximizingPlayer, int playerColor, int depth);
/*
 * this function calculates the score of
 * a specified board and returns the score
 * the score is calculated by the pieces on
 * the board
 */
int spGetScore(SPChessBoard * chess_board);


/*this function retunrs the colr of the computer
 * @param chessBoard - the game board
 */
int spGetCompColor(SPChessBoard* chessBoard);

#endif /* SPMINIMAX_H_ */

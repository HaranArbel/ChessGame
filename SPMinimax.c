/*
 * minimax.c
 *
 */

#include "SPMinimax.h"

// get best move for computer useing minimax algorithm
//------------------------------------------------------------

Move * spGetComputerMove(SPChessBoard * chess_board, int computer_color, int minimax_depth) {

	SPArrayList* all_moves = spGetAllMoves(chess_board, computer_color);
	if (!all_moves)
		return NULL;
	Move *best_move, *curr_move;



	int best_value = INT_MIN;
	int curr_move_score = best_value;

	for (int i=0; i<all_moves->actualSize; i++)
	{
		curr_move = spArrayListGetAt(all_moves, i);
		spExecuteMove(chess_board, curr_move->from, curr_move->to);
		curr_move_score = spMinimax(chess_board, INT_MIN, INT_MAX, false, computer_color^PIECE_COLOR_BIT_MASK ,minimax_depth-1);
		spUndoPrevMove(chess_board, curr_move);
		if(curr_move_score >= best_value) {
			best_value = curr_move_score;
			best_move = curr_move;
			if (curr_move_score == INT_MAX)
			{
				spPrintMove(best_move);
				spExecuteMove(chess_board, best_move->from, best_move->to);
				spArrayListDestroy(all_moves);
				return best_move;
			}
		}
	}
	spPrintMove(best_move);
	spExecuteMove(chess_board, best_move->from, best_move->to);
	spArrayListDestroy(all_moves);
	return best_move;
}



//Minimax
//----------

int spMinimax(SPChessBoard * chess_board, int alpha, int beta, bool maximizingPlayer, int playerColor,int depth) {

	// player_color = WHITE_PIECE or BLACK_PIECE is the color of the current player
	// max_player_color is the color of the player that we want to maximize

	Move *curr_move;
	int score = 0, result, i;

	result = spIsGameOver(chess_board, playerColor^PIECE_COLOR_BIT_MASK);

	if (result == BLACK_WON || result == WHITE_WON){
		if (maximizingPlayer)
			return INT_MIN;
		return INT_MAX;
	}

	if((depth == 0) || (result == TIE)){
		score = spGetScore(chess_board);
		return score;
	}


	SPArrayList* all_moves = spGetAllMoves(chess_board, playerColor);
	int total_moves = all_moves->actualSize;

	if(maximizingPlayer){
		for (i = 0; i < total_moves; i++) {
			curr_move = spArrayListGetAt(all_moves, i);
			spExecuteMove(chess_board, curr_move->from, curr_move->to);
			alpha = MAX(alpha, spMinimax(chess_board, alpha, beta, false, playerColor^PIECE_COLOR_BIT_MASK, depth-1));
			spUndoPrevMove(chess_board, curr_move);
			if(beta <= alpha)
				break;
		}
		spArrayListDestroy(all_moves);
		return alpha;
	}
	else{
		for (i = 0; i < total_moves; i++) {
			curr_move = spArrayListGetAt(all_moves, i);
			spExecuteMove(chess_board, curr_move->from, curr_move->to);
			beta = MIN(beta, spMinimax(chess_board, alpha, beta, true, playerColor^PIECE_COLOR_BIT_MASK, depth-1));
			spUndoPrevMove(chess_board, curr_move);
			if(beta <= alpha)
				break;
		}
		spArrayListDestroy(all_moves);
		return beta;
	}
}




int spGetScore(SPChessBoard * chess_board) {
	int  i, j, whitePoints = 0, blackPoints = 0, piece;
	int computer_color = spGetCompColor(chess_board);
	for(i = 0 ; i < 8 ; i++) {
		for(j = 0 ; j < 8 ; j++) {
			piece = chess_board->board[i][j];
			if (piece != EMPTY_PIECE){

				int piece_type = (piece & PIECE_BIT_MASK);
				int piece_color = (piece & PIECE_COLOR_BIT_MASK);

				if (piece_type == PAWN_PIECE){
					if (piece_color == WHITE_PIECE){
						whitePoints += 1;
					}
					else{
						blackPoints += 1;
					}
				}
				else if((piece_type == KNIGHT_PIECE) || (piece_type == BISHOP_PIECE)){
					if (piece_color == WHITE_PIECE){
						whitePoints += 3;
					}
					else{
						blackPoints += 3;
					}
				}
				else if(piece_type == ROOK_PIECE){
					if (piece_color == WHITE_PIECE){
						whitePoints += 5;
					}
					else{
						blackPoints +=5;
					}
				}
				else if(piece_type == QUEEN_PIECE){
					if (piece_color == WHITE_PIECE){
						whitePoints += 9;
					}
					else{
						blackPoints += 9;
					}
				}
				else if(piece_type == KING_PIECE){
					if (piece_color == WHITE_PIECE){
						whitePoints += 100;
					}
					else{
						blackPoints += 100;
					}
				}

			}
		}
	}

	int score = (computer_color == WHITE_PIECE) ? whitePoints - blackPoints : blackPoints - whitePoints;
	return score;

}


int spGetCompColor(SPChessBoard* chess_board){
	if (chess_board->userColor == WHITE)
		return BLACK_PIECE;
	return WHITE_PIECE;
}


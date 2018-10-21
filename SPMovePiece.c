/*
 * pieces.c
 *
 */

#include "SPMovePiece.h"
#include "MainGame.h"


// Check if a move is valid

MOVE_MESSAGE spIsLegalMove(SPChessBoard *chess_board, Square src, Square dst, int player_color){
	unsigned int movedPiece = chess_board->board[src.row][src.col];
	unsigned int movedPieceType = movedPiece & PIECE_BIT_MASK;


	// Check if a position is in the board
	if (!spIsValidPosition(src) || !spIsValidPosition(dst)){
		return INVALID_POSITION;
	}
	// check if the player is trying to perform a move on an empty square
	if (chess_board->board[src.row][src.col] == EMPTY_PIECE){
		return NO_LEGAL_PIECE;
	}

	// Check if the piece in the source square belongs to the player
	if ((chess_board->board[src.row][src.col] & PIECE_COLOR_BIT_MASK) != (unsigned)player_color){
		return NO_LEGAL_PIECE;
	}

	// Check if the player is trying to move a piece to a square already occupied by his own piece
	if ((chess_board->board[src.row][src.col] & PIECE_COLOR_BIT_MASK) == (chess_board->board[dst.row][dst.col] & PIECE_COLOR_BIT_MASK)){
		return ILLEGAL_MOVE;
	}

	// Check if the destination square equals the source square
	if ((src.row == dst.row) && (src.col == dst.col)){
		return ILLEGAL_MOVE;
	}


	// Check if the move is valid according to pieces rules
	// ----------------------------------------------------

	if (((movedPieceType == PAWN_PIECE) && (!spIsLegalPawnMove(src, dst, chess_board)))
			||

			((movedPieceType == ROOK_PIECE) && (!spIsLegalRookMove(src, dst, chess_board)))
			||

			((movedPieceType == KNIGHT_PIECE) && (!spIsLegalKnightMove(src, dst)))
			||

			((movedPieceType == BISHOP_PIECE) && (!spIsLegalBishopMove(src, dst, chess_board)))
			||

			((movedPieceType == QUEEN_PIECE) && (!spIsLegalQueenMove(src, dst, chess_board)))
			||

			((movedPieceType == KING_PIECE) && (!spIsLegalKingMove(src, dst, chess_board)))){

		return ILLEGAL_MOVE;
	}

	return LEGAL_MOVE;
}


int spIsLegalPawnMove(Square src, Square dst, SPChessBoard *chess_board){

	unsigned int movedPiece = chess_board->board[src.row][src.col];
	unsigned int movedPieceColor = movedPiece & PIECE_COLOR_BIT_MASK;
	unsigned int takenPiece = chess_board->board[dst.row][dst.col];
	unsigned int takenPieceColor = takenPiece & PIECE_COLOR_BIT_MASK;

	// Check if there is an invalid "eating attempt"
	// if the user is trying to move a piece into the opponent's
	// place to eat it
	if ((takenPieceColor != movedPieceColor) && (takenPiece != EMPTY_PIECE)){

		// Pawns cans only eat one square diagonally forward:
		if (((movedPieceColor == WHITE_PIECE) && (dst.row - src.row != 1 || abs(dst.col - src.col) != 1))
				||
				((movedPieceColor == BLACK_PIECE) && (dst.row - src.row != -1 || abs(dst.col - src.col) != 1))
		)
		{
			return 0;
		}
	}

	else{

		// Pawns can only move one square forward or two squares on first move:
		if (
				(src.col != dst.col)
				||
				((movedPieceColor == WHITE_PIECE) && src.row > 1 && (dst.row - src.row) != 1)
				||
				((movedPieceColor == WHITE_PIECE) && src.row == 1 && abs(dst.row - src.row) > 2)
				||
				((movedPieceColor == BLACK_PIECE) && src.row < 6 && (dst.row - src.row) != -1)
				||
				((movedPieceColor == BLACK_PIECE) && src.row == 6 && abs(dst.row - src.row) > 2)

		)
		{
			return 0;
		}
	}

	return 1;
}


int spIsLegalBishopMove(Square src, Square dst, SPChessBoard *chess_board){

	// Check if a move is strictly diagonal:
	if (abs(src.row - dst.row) != abs(src.col - dst.col)){
		return 0;
	}

	// Check if bishop is moving over any piece
	if (!spIsPathClearDiagonally(src,dst,chess_board)){
		return 0;
	}

	return 1;
}


int spIsLegalRookMove(Square src, Square dst, SPChessBoard *chess_board){

	// Check if the move is strictly horizontal or vertical
	if (src.row != dst.row && src.col != dst.col){
		return 0;
	}

	// Check if rook is jumping over any piece:

	// If the move is horizontal
	if (src.row == dst.row && (!spIsPathClearHorizontally(src,dst,chess_board))){
		return 0;
	}
	// If the move is vertical
	if (src.col == dst.col && (!spIsPathClearVertically(src,dst,chess_board))){
		return 0;
	}
	return 1;
}


int spIsLegalKnightMove(Square src, Square dst)
{
	// Knight can move :
	// - 2 up or 2 down and then 1 left or 1 right
	// - 1 up or 1 down and then 2 left or 2 right

	// 1st option: 2 up or 2 down and then 1 left or 1 right
	if (abs(dst.row - src.row) == 2 && abs(dst.col - src.col) == 1){
		return 1;
	}
	// 2nd option: 1 up or 1 down and then 2 left or 2 right
	if (abs(dst.row - src.row) == 1 && abs(dst.col - src.col) == 2){
		return 1;
	}

	return 0;
}


int spIsLegalQueenMove(Square src, Square dst, SPChessBoard *chess_board){

	// Queen can move horizontally or vertically or diagonally
	if (!(src.row == dst.row || src.col == dst.col || (abs(src.row - dst.row) == abs(src.col - dst.col)))){
		return 0;
	}
	if (src.row == dst.row && (!spIsPathClearHorizontally(src,dst,chess_board))){
		return 0;
	}
	else if (src.col == dst.col && (!spIsPathClearVertically(src,dst,chess_board))){
		return 0;
	}
	else if (abs(src.col - dst.col) == abs(src.col - dst.col) && (!spIsPathClearDiagonally(src,dst,chess_board))){
		return 0;
	}
	return 1;
}


int spIsLegalKingMove(Square src, Square dst, SPChessBoard *chess_board){

	// King can move horizontally or vertically or diagonally one square
	if (!( (src.row == dst.row && abs(src.col - dst.col) == 1 )
			|| (src.col == dst.col && abs(src.row - dst.row) == 1)
			|| (abs(src.row - dst.row) == 1 && abs(src.col - dst.col) == 1))){
		return 0;
	}
	if (src.row == dst.row && (!spIsPathClearHorizontally(src,dst,chess_board))){
		return 0;
	}
	else if (src.col == dst.col && (!spIsPathClearVertically(src,dst,chess_board))){
		return 0;
	}
	else if (abs(src.col - dst.col) == abs(src.col - dst.col) && (!spIsPathClearDiagonally(src,dst,chess_board))){
		return 0;
	}
	return 1;
}


int spIsPathClearHorizontally(Square src, Square dst, SPChessBoard *chess_board){
	int i;

	for(i = src.col; i != dst.col;){
		if (dst.col > src.col) i++;
		if (dst.col < src.col) i--;

		if (i != dst.col && chess_board->board[src.row][i] > 0)
			return 0;
	}
	return 1;
}

int spIsPathClearVertically(Square src, Square dst, SPChessBoard *chess_board){
	int i;

	for(i = src.row; i != dst.row;){
		if (dst.row > src.row) i++;
		if (dst.row < src.row) i--;

		if (i != dst.row && chess_board->board[i][src.col] > 0){
			return 0;
		}
	}
	return 1;
}

int spIsPathClearDiagonally(Square src, Square dst, SPChessBoard *SPChessBoard){
	int i, j;

	j = src.col;
	for (i = src.row; i != dst.row;){
		if (dst.row > src.row) i++;
		if (dst.row < src.row) i--;
		if (dst.col > src.col) j++;
		if (dst.col < src.col) j--;

		if ((SPChessBoard->board[i][j] != 0) && (i != dst.row) && (j != dst.col)){
			return 0;
		}
	}
	return 1;
}


int spNoMoreLegalMoves(SPChessBoard * chess_board,unsigned int playerColor){
	SPArrayList * moves = NULL;
	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if ((chess_board->board[i][j] & PIECE_COLOR_BIT_MASK) == playerColor){
				moves = spGetMoves(chess_board,i,j,playerColor); // get all unchecked moves
				if (!spArrayListIsEmpty(moves)){
					spArrayListDestroy(moves);
					return 0;
				}
			}
		}
	}
	spArrayListDestroy(moves);
	return 1;
}


int spIsGameOver(SPChessBoard *chess_board, int player_color){
	int is_check = 0;
	int no_more_moves;
	if (!chess_board){
		return -1;
	}

	// check if the other player's king is under threat
	is_check = spIsChessCheck(chess_board,player_color^PIECE_COLOR_BIT_MASK);

	// check if the other player can perform any move
	// that will remove the threat
	no_more_moves = spNoMoreLegalMoves(chess_board,player_color^PIECE_COLOR_BIT_MASK);

	if (is_check){
		if (no_more_moves){
			if (player_color == WHITE_PIECE){
				return WHITE_WON;
			}
			else{
				return BLACK_WON;
			}
		}
		else{
			return CHECK;
		}
	}

	else
		if (no_more_moves){
			return TIE;
		}
	return NONE;
}


//checks if the king is under threat
//----------------------------------

int spIsChessCheck(SPChessBoard *chess_board, int player_color){
	int i,j;
	Square curr;
	int piece;
	Square king_pos;


	// Find the player's king

	for (i=0; i<8; i++){
		for (j=0; j<8; j++){
			piece = chess_board->board[i][j];
			if ((piece & PIECE_BIT_MASK) == KING_PIECE && (piece & PIECE_COLOR_BIT_MASK) == player_color){
				king_pos.row = i, king_pos.col = j;
			}
		}
	}

	// scan the board and check for valid eating attempts in any direction around the king
	//----------------------

	for(i = 0; i < 8; i++){
		for (j = 0; j <8; j++){
			piece = chess_board->board[i][j];
			if((piece != EMPTY_PIECE) && ((piece & PIECE_COLOR_BIT_MASK) == (player_color^PIECE_COLOR_BIT_MASK))){
				curr.row = i, curr.col = j;
				if (spIsLegalMove(chess_board,curr,king_pos,player_color^PIECE_COLOR_BIT_MASK) == LEGAL_MOVE){
					return 1;
				}
			}
		}
	}

	return 0;
}


int spIsNextTurnCheck(SPChessBoard *chess_board, Square src, Square dst, int player_color) {

	SPChessBoard* board_cpy = spCopyBoard(chess_board);

	// Move the actual piece in the clone board
	board_cpy->board[dst.row][dst.col] = chess_board->board[src.row][src.col];
	board_cpy->board[src.row][src.col] = EMPTY_PIECE;

	int res = spIsChessCheck(board_cpy,player_color);
	spDestroyBoard(board_cpy);
	return res;
}

void spExecuteMove(SPChessBoard *chess_board, Square src, Square dst){
	unsigned int moved_piece;
	Move* new_move;

	// Assign the taken piece and the moved piece
	moved_piece = chess_board->board[src.row][src.col];

	// Update moves history
	new_move = spCreateMove(chess_board,src,dst);
	spUpdateHistory(new_move, chess_board);

	// Move the actual piece on the board
	chess_board->board[dst.row][dst.col] = moved_piece;
	chess_board->board[src.row][src.col] = EMPTY_PIECE;
	switchTurns(chess_board);

}


void spUpdateHistory(Move *move, SPChessBoard *chess_board) {
	if (spArrayListIsFull(chess_board->history)) {
		spArrayListRemoveLast(chess_board->history);
	}
	spArrayListAddFirst(chess_board->history, move);

}

int spAllMovesAmount(SPChessBoard* chess_board, int player_color){
	if (!chess_board){
		return -1;
	}
	int moves_num = 0;
	for (int x=0; x<8; x++){
		for (int y=0; y<8; y++){
			int piece = chess_board->board[x][y];
			if ((piece & PIECE_COLOR_BIT_MASK) == player_color){
				SPArrayList* curr_piece_moves = spGetMoves(chess_board, x, y, player_color);
				if (curr_piece_moves){
					moves_num += curr_piece_moves->actualSize;
				}
				spArrayListDestroy(curr_piece_moves);
			}
		}
	}
	return moves_num;
}

SPArrayList * spGetAllMoves(SPChessBoard* chess_board, int player_color){

	if (!chess_board){
		return NULL;
	}
	int move_amount = spAllMovesAmount(chess_board, player_color);

	if (move_amount <= 0)
		return NULL;
	SPArrayList* all_moves = spArrayListCreate(move_amount);
	if (!all_moves)
		return NULL;

	int added_moves = 0;

	for (int x=0; x<8; x++){
		for (int y=0; y<8; y++){
			int curr_piece = chess_board->board[x][y];
			if ((curr_piece & PIECE_COLOR_BIT_MASK) == player_color){
				SPArrayList* curr_piece_moves = spGetMoves(chess_board, x, y, player_color);
				if (!curr_piece_moves){
					return NULL;
				}
				int size = curr_piece_moves->actualSize;
				for (int i=0; i<size; i++){
					Move* moveCopy = spCreateMove(chess_board,curr_piece_moves->moves[i]->from,curr_piece_moves->moves[i]->to);
					spArrayListAddAt(all_moves, moveCopy, added_moves);
					added_moves++;
				}
				spArrayListDestroy(curr_piece_moves);
			}
		}
	}
	return all_moves;
}

SPArrayList * spGetMoves(SPChessBoard* chess_board, int row, int col, int player_color){
	unsigned int piece;
	Square src;

	if (!chess_board){
		return NULL;
	}
	src.row = row;
	src.col = col;
	piece = chess_board->board[row][col];

	switch(piece & PIECE_BIT_MASK){

	case PAWN_PIECE:
		return spGetPawnMoves(chess_board,src, player_color);
		break;

	case ROOK_PIECE:
		return spGetRookMoves(chess_board,src, player_color);
		break;

	case BISHOP_PIECE:
		return spGetBishopMoves(chess_board,src, player_color);
		break;

	case KNIGHT_PIECE:
		return spGetKnightMoves(chess_board,src, player_color);
		break;

	case QUEEN_PIECE:
		return spGetQueenMoves(chess_board,src, player_color);
		break;

	case KING_PIECE:
		return spGetKingMoves(chess_board,src,player_color);
		break;
	}
	return NULL;//shouldn't get here if everything's ok.
}

SPArrayList * spGetPawnMoves(SPChessBoard* chess_board, Square src, int player_color){
	Square dst;
	int j;
	int piece;
	Move * move;

	piece = chess_board->board[src.row][src.col];
	SPArrayList *moves = spArrayListCreate(MAX_PAWN_MOVES);
	if (!moves){
		spDestroyBoard(chess_board);
		exit(EXIT_FAILURE);
	}
	j = ((piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) ? 1 : (-1) ;
	dst.row = src.row + j;

	for (int i = -1; i <= 1; i++){
		dst.col = src.col + i;
		if ((spIsLegalMove(chess_board, src, dst, player_color) == LEGAL_MOVE)
				&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))

		{
			move = spCreateMove(chess_board, src, dst);
			spArrayListAddLast(moves, move);
		}
	}

	j = ((piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) ? 2 : (-2) ;

	dst.row = src.row +j;
	dst.col = src.col;
	if ((spIsLegalMove(chess_board,src,dst,player_color) == LEGAL_MOVE )
			&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))
	{
		Move * move = spCreateMove(chess_board, src, dst);
		spArrayListAddLast(moves, move);
	}
	return moves;
}

SPArrayList * spGetRookMoves(SPChessBoard* chess_board, Square src, int player_color){
	Square dst;
	Move* move;
	int i;

	if (!chess_board){
		return NULL;
	}
	SPArrayList *moves = spArrayListCreate(MAX_ROOK_MOVES);
	if (!moves){
		return NULL;
	}
	dst.row = src.row;
	for (i = -7; i <= 7; i++){
		dst.col = src.col + i;
		if ((spIsLegalMove(chess_board, src, dst, player_color) == LEGAL_MOVE)
				&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))
		{
			move = spCreateMove(chess_board, src, dst);
			spArrayListAddLast(moves, move);
		}
	}

	dst.col = src.col;
	for (i = -7; i <= 7; i++){
		dst.row = src.row + i;
		if ((spIsLegalMove(chess_board, src, dst, player_color) == LEGAL_MOVE)
				&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))
		{
			move = spCreateMove(chess_board, src, dst);
			spArrayListAddLast(moves, move);
		}
	}
	return moves;
}

SPArrayList * spGetBishopMoves(SPChessBoard *chess_board, Square src, int player_color){
	int i;
	Square dst;
	Move* move;
	if (!chess_board){
		return NULL;
	}
	SPArrayList *moves = spArrayListCreate(MAX_BISHOP_MOVES);
	if (!moves){
		return NULL;
	}

	for (i = 0; i < 7; i++){
		dst.row = src.row+i;
		dst.col = src.col+i;
		if ((spIsLegalMove(chess_board, src, dst,player_color) == LEGAL_MOVE)
				&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))
		{
			move = spCreateMove(chess_board, src, dst);
			spArrayListAddLast(moves, move);
		}
		dst.row = src.row+i;
		dst.col = src.col-i;
		if ((spIsLegalMove(chess_board, src, dst,player_color) == LEGAL_MOVE)
				&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))
		{
			move = spCreateMove(chess_board, src, dst);
			spArrayListAddLast(moves, move);
		}
		dst.row = src.row-i;
		dst.col = src.col+i;
		if ((spIsLegalMove(chess_board, src, dst,player_color) == LEGAL_MOVE)
				&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))
		{
			move = spCreateMove(chess_board, src, dst);
			spArrayListAddLast(moves, move);
		}
		dst.row = src.row-i;
		dst.col = src.col-i;
		if ((spIsLegalMove(chess_board, src, dst,player_color) == LEGAL_MOVE)
				&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))
		{
			move = spCreateMove(chess_board, src, dst);
			spArrayListAddLast(moves, move);
		}

	}
	return moves;
}

SPArrayList * spGetKnightMoves(SPChessBoard *chess_board, Square src, int player_color){
	Move *move1;
	Move *move2;
	int i,j;
	SPArrayList *moves;
	Square dst;

	moves = spArrayListCreate(MAX_KNIGHT_MOVES);
	if (!moves){
		return NULL;
	}
	for (i = -2; i <= 2; i++){
		if (i != 0 ){
			j = (abs(i) == 1) ? 2 : 1;

			dst.row = src.row + i;
			dst.col = src.col + j;

			if((spIsLegalMove(chess_board,src,dst,player_color) == LEGAL_MOVE)
					&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))
			{
				move1 = spCreateMove(chess_board, src, dst);
				spArrayListAddLast(moves, move1);
			}
			dst.row = src.row - i;
			dst.col = src.col - j;

			if((spIsLegalMove(chess_board,src,dst,player_color) == LEGAL_MOVE)
					&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))
			{
				move2 = spCreateMove(chess_board, src, dst);
				spArrayListAddLast(moves, move2);
			}
		}
	}
	return moves;
}

SPArrayList* spGetQueenMoves(SPChessBoard *chess_board, Square src, int player_color){

	if (!chess_board){
		return NULL;
	}
	SPArrayList* queen_moves;
	SPArrayList* rook_moves = spGetRookMoves(chess_board, src, player_color);
	SPArrayList* bishop_moves = spGetBishopMoves(chess_board,src, player_color);

	queen_moves = spArrayListConcat(rook_moves,bishop_moves);
	return queen_moves;
}

SPArrayList* spGetKingMoves(SPChessBoard *chess_board, Square src, int player_color){
	SPArrayList *moves;
	Square dst;

	if (!chess_board){
		return NULL;
	}
	moves = spArrayListCreate(MAX_KING_MOVES);
	if (!moves){
		return NULL;
	}
	for (int i = -1; i <= 1; i++){
		for (int j = -1; j <= 1; j++){
			dst.row = src.row + i;
			dst.col = src.col + j;

			if ((spIsLegalMove(chess_board,src,dst,player_color) == LEGAL_MOVE)
					&& (!spIsNextTurnCheck(chess_board,src,dst,player_color)))
			{
				Move * move = spCreateMove(chess_board, src, dst);
				spArrayListAddLast(moves, move);
			}
		}
	}
	return moves;
}



void spPrintMove(Move *move) {
	if (!move){
	return;
	}
	char* piece_name;
	int moved_piece = move->moved_piece;

	switch (moved_piece & PIECE_BIT_MASK){
		case(PAWN_PIECE):
			piece_name = "pawn";
			break;

		case(ROOK_PIECE):
			piece_name = "rook";
			break;

		case(BISHOP_PIECE):
			piece_name = "bishop";
			break;

		case(KNIGHT_PIECE):
			piece_name = "knight";
			break;

		case(QUEEN_PIECE):
			piece_name = "queen";
			break;

		case(KING_PIECE):
			piece_name = "king";
			break;
	}

	printCompMove(piece_name);
	printf("<%d,%c> to ",move->from.row+1, (char)(move->from.col+'A'));
	printf("<%d,%c>",move->to.row+1, (char)(move->to.col+'A'));
	printf("\n");
}

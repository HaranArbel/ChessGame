/*
 * board.c
 *
 */
#include "SPBoard.h"



void spInitBoard(SPChessBoard* chess_board){

	// Set the board with empty spaces
	// -------------------
	for(int i=7;i>=0; i--)
		for(int j =0;j<8; j++)
			chess_board->board[i][j] = EMPTY_PIECE;

	// Set the white pieces
	// -----------------------
	chess_board->board[BOTTOM_ROW][0] = WHITE_PIECE  |  ROOK_PIECE  |  ROOK_VALUE;
	chess_board->board[BOTTOM_ROW][1] = WHITE_PIECE  |  KNIGHT_PIECE  |  KNIGHT_VALUE;
	chess_board->board[BOTTOM_ROW][2] = WHITE_PIECE  |  BISHOP_PIECE  |  BISHOP_VALUE;
	chess_board->board[BOTTOM_ROW][3] = WHITE_PIECE  |  QUEEN_PIECE  |  QUEEN_VALUE;
	chess_board->board[BOTTOM_ROW][4] = WHITE_PIECE  |  KING_PIECE  |  KING_VALUE;
	chess_board->board[BOTTOM_ROW][5] = WHITE_PIECE  |  BISHOP_PIECE  |  BISHOP_VALUE;
	chess_board->board[BOTTOM_ROW][6] = WHITE_PIECE  |  KNIGHT_PIECE  |  KNIGHT_VALUE;
	chess_board->board[BOTTOM_ROW][7] = WHITE_PIECE  |  ROOK_PIECE  |  ROOK_VALUE;

	for (int i = 0; i < 8; i++){
		chess_board->board[BOTTOM_ROW+1][i] = WHITE_PIECE  |  PAWN_PIECE  |  PAWN_VALUE;
	}

	// Set the black pieces
	// -----------------------
	chess_board->board[TOP_ROW][0] = BLACK_PIECE  |  ROOK_PIECE |  ROOK_VALUE;
	chess_board->board[TOP_ROW][1] = BLACK_PIECE  |  KNIGHT_PIECE |  KNIGHT_VALUE;
	chess_board->board[TOP_ROW][2] = BLACK_PIECE  |  BISHOP_PIECE |  BISHOP_VALUE;
	chess_board->board[TOP_ROW][3] = BLACK_PIECE  |  QUEEN_PIECE |  QUEEN_VALUE;
	chess_board->board[TOP_ROW][4] = BLACK_PIECE  |  KING_PIECE |  KING_VALUE;
	chess_board->board[TOP_ROW][5] = BLACK_PIECE  |  BISHOP_PIECE |  BISHOP_VALUE;
	chess_board->board[TOP_ROW][6] = BLACK_PIECE  |  KNIGHT_PIECE |  KNIGHT_VALUE;
	chess_board->board[TOP_ROW][7] = BLACK_PIECE  |  ROOK_PIECE |  ROOK_VALUE;

	for (int i = 0; i < 8; i++){
		chess_board->board[TOP_ROW-1][i] = BLACK_PIECE  |  PAWN_PIECE |  PAWN_VALUE;
	}

}

char spGetPieceChar(unsigned int piece){

	if ((piece & PIECE_BIT_MASK) == PAWN_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return('m');
	else if ((piece & PIECE_BIT_MASK) == ROOK_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return('r');
	else if ((piece & PIECE_BIT_MASK) == KNIGHT_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return('n');
	else if ((piece & PIECE_BIT_MASK) == BISHOP_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return('b');
	else if ((piece & PIECE_BIT_MASK) == QUEEN_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return('q');
	else if ((piece & PIECE_BIT_MASK) == KING_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return('k');

	else if ((piece & PIECE_BIT_MASK) == PAWN_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return('M');
	else if ((piece & PIECE_BIT_MASK) == ROOK_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return('R');
	else if ((piece & PIECE_BIT_MASK) == KNIGHT_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return('N');
	else if ((piece & PIECE_BIT_MASK) == BISHOP_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return('B');
	else if ((piece & PIECE_BIT_MASK) == QUEEN_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return('Q');
	else if ((piece & PIECE_BIT_MASK) == KING_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return('K');
	else return ('_');
}

unsigned int spGetPieceFromChar(char c){

	if (c == 'r') return WHITE_PIECE  |  ROOK_PIECE  |  ROOK_VALUE;
	else if (c == 'n') return WHITE_PIECE  |  KNIGHT_PIECE  |  KNIGHT_VALUE;
	else if (c == 'b') return WHITE_PIECE  |  BISHOP_PIECE  |  BISHOP_VALUE;
	else if (c == 'q') return WHITE_PIECE  |  QUEEN_PIECE  |  QUEEN_VALUE;
	else if (c == 'k') return WHITE_PIECE  |  KING_PIECE  |  KING_VALUE;
	else if (c == 'm') return WHITE_PIECE  |  PAWN_PIECE  |  PAWN_VALUE;

	else if (c == 'R') return BLACK_PIECE  |  ROOK_PIECE |  ROOK_VALUE;
	else if (c == 'N') return BLACK_PIECE  |  KNIGHT_PIECE |  KNIGHT_VALUE;
	else if (c == 'B') return BLACK_PIECE  |  BISHOP_PIECE |  BISHOP_VALUE;
	else if (c == 'Q') return BLACK_PIECE  |  QUEEN_PIECE |  QUEEN_VALUE;
	else if (c == 'K') return BLACK_PIECE  |  KING_PIECE |  KING_VALUE;
	else if (c == 'M') return BLACK_PIECE  |  PAWN_PIECE  |  PAWN_VALUE;
	else return EMPTY_PIECE;

}

void spPrintPieceChar(unsigned int piece){
	printf("%c",spGetPieceChar(piece));
}

void spPrintBoard(SPChessBoard *chess_board){
	int i,j;

	printf("8| ");
	for(i = TOP_ROW; i >= 0; i--)
	{
		for(j = 0 ;j < 8; j++)
		{
			spPrintPieceChar(chess_board->board[i][j]);
			printf(" ");
		}
		printf("|");
		if (i >= 1)
			printf("\n%d| ",i);
	}

	// Print the columns letters
	printf("\n  -----------------");
	printf("\n   A B C D E F G H");
	printf("\n");
}


SPChessBoard* spCreateNewBoard(){

	SPChessBoard* chess_board = (SPChessBoard*) malloc(sizeof(SPChessBoard));
	if (chess_board == NULL){
		//errorHandling("malloc");
		free(chess_board);
		exit(EXIT_FAILURE);
	}

	if(chess_board == NULL) {
		return NULL;
	}
	spInitBoard(chess_board);
	SPArrayList* history = spArrayListCreate(MAX_HISTORY);
	chess_board->history = history;
	chess_board->isLoadedGame = false;
	return chess_board;
}

void spDestroyBoard(SPChessBoard * chess_board){
	if (chess_board == NULL){
		return;
	}
	if (chess_board->history){
		spArrayListDestroy(chess_board->history);
	}
	free(chess_board);

}

// Sets the square with the specified row and column
void spSetSquare(Square* square, int row, int col){
	square->row = row;
	square->col = col;
}


SPChessBoard * spCopyBoard(SPChessBoard* chess_board){
	unsigned int i, j;

	SPChessBoard *board_cpy = (SPChessBoard *)malloc(sizeof(SPChessBoard));
	if(board_cpy == NULL) {
		//errorHandling("malloc");
		free(board_cpy);
		free(chess_board);
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < 8; i++){
		for (j = 0;j < 8; j++){
			board_cpy->board[i][j] = chess_board->board[i][j];
		}
	}
	board_cpy->history = spArrayListCopy(chess_board->history);
	return board_cpy;
}


bool spIsValidPosition(Square s){
	if ((s.row > 7) || (s.row < 0) || (s.col > 7) || (s.col < 0)){
		return false;
	}
	return true;
}


Move * spCreateMove(SPChessBoard * chess_board, Square src, Square dst){

	Move *move = (Move*)malloc(sizeof(Move));
	if (!move){
		return NULL;
	}
	move->from = src;
	move->to = dst;
	move->moved_piece = chess_board->board[src.row][src.col];
	move->taken_piece = chess_board->board[dst.row][dst.col];
	return move;

}

void spDestroyMove(Move *move){
	free(move);
}



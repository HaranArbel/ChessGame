/*
 * main_game.c

 *
 */
#include "MainGame.h"


int spMainGame(){

	SPChessBoard* chessBoard = spInitGame();
	spSettingsState(chessBoard);
	return 0;
}

SPChessBoard* spInitGame(){

	SPChessBoard* chessBoard = spCreateNewBoard();
	spSettingsToDefault(chessBoard);
	return chessBoard;
}

void spSettingsState(SPChessBoard * chessBoard){
	setbuf(stdout,NULL);//todo
	char* fileName;
	FILE* ifp = NULL;
	printf(SETTINGS_STATE_MESSAGE);
	fflush(stdout);//todo

	while(true){
		SPCommand *command = spGetCommand();
		if (command == NULL){
			spDestroyBoard(chessBoard);
			exit(EXIT_FAILURE);
		}
		switch(command->cmd){

		case SP_GAME_MODE:
			spSetGameMode(command, chessBoard);
			break;

		case SP_DIFFICULTY:
			spSetDifficulty(command, chessBoard);
			break;

		case SP_USER_COLOR:
			spSetUserColor(command, chessBoard);
			break;

		case SP_LOAD:
			fileName = command->path;
			if((ifp = fopen(fileName, "r")) == NULL) {
				//free(command);//todo
				printf(FILE_NOT_FOUND_MESSAGE);
				//free(file_name);
			}
			else {
				spLoadSettingsFromFile(chessBoard, ifp);
				fclose(ifp);
				//free todo
			}
			break;

		case SP_DEFAULT:
			spSettingsToDefault(chessBoard);
			break;

		case SP_PRINT_SETTING:
			spPrintSettings(chessBoard);
			break;

		case SP_QUIT:
			free(command);
			spQuit(chessBoard);
			break;

		case SP_START:
			free(command);
			spGameState(chessBoard);
			break;

		case SP_INVALID_LINE:
			printf("Invalid command\n");
			break;
		}
		free(command);
	}
}


void spGameState(SPChessBoard * chessBoard){
	char *command, *fileName, *error;
	Square src, dst;
	setbuf(stdout,NULL);
	fflush(stdout);//todo

	if(!chessBoard->isLoadedGame) {
		spSetInitialTurn(chessBoard);
	}
	spOpeningBoardPrint(chessBoard);

	while(true) {
		while(chessBoard->turn == USER) {
			spPrintUserMessage(chessBoard);

			command = spGetInputString();
			if (!command){
				spDestroyBoard(chessBoard);
				exit(EXIT_FAILURE);
			}
			if(strncmp(command, "move", 4) == 0){
				if (spParseMoveCommand(command, &error) == -1){
					free(command);
					printf("%s",error);
					continue;
				}
				else{
					src.row = command[6]-'1';
					src.col = command[8]-'A';

					dst.row = command[15]-'1';
					dst.col = command[17]-'A';
					MOVE_MESSAGE message;
					free(command);
					int player_color = getCurrentPlayerColor(chessBoard);
					if(spMovePiece(chessBoard, src, dst,player_color, &message) == 1) {	// the move was executed correctly

						switch(spIsGameOver(chessBoard, player_color)) {
						case(CHECK):
							if (chessBoard->userColor == BLACK){
								printf("%s",BLACK_KING_CHECK_MESSAGE);
							}
							else{
								printf("%s",WHITE_KING_CHECK_MESSAGE);
							}
						break;

						case(WHITE_WON):
							printf("%s",WHITE_PLAYER_WINS_MESSAGE);
							spDestroyBoard(chessBoard);
							exit(EXIT_SUCCESS);
						break;

						case(BLACK_WON):
							printf("%s",BLACK_PLAYER_WINS_MESSAGE);
							spDestroyBoard(chessBoard);
							exit(EXIT_SUCCESS);
						break;

						case(TIE):
							printf("%s",TIE_MESSAGE);
							spDestroyBoard(chessBoard);
							exit(EXIT_SUCCESS);

						case(NONE):
							break;
						}

						if (chessBoard->gameMode == TWO_PLAYERS){
							spPrintBoard(chessBoard);
						}
					}
					else{ // if the move is not valid
						spPrintMoveMessage(message);
						continue;
					}
				}
			}
			else if(strncmp(command, "save", 4) == 0){
				fileName = strtok(command, " \t\r\n"); // get the save file path
				fileName = strtok(NULL, " \t\r\n");
				free(command);
				spSaveFile(chessBoard, fileName);
			}
			else if(strncmp(command, "undo", 4) == 0){
				free(command);
				if (spUndoMove(chessBoard) == -1){
					continue;
				}
				else{
					spPrintBoard(chessBoard);
				}
			}
			else if(strncmp(command, "reset", 5) == 0){
				free(command);
				printf("Restarting...\n");
				chessBoard = spInitGame();
				spSettingsState(chessBoard);
			}
			else if(strncmp(command, "quit", 4) == 0){
				free(command);
				spQuit(chessBoard);
			}
			else{
				free(command);
				printf("Invalid command\n");
			}
		}

		while(chessBoard->turn == COMPUTER) {
			// we are finding the computer's playerColor and passing it to get_comp_move
			int compColor = getCurrentPlayerColor(chessBoard);
			spGetComputerMove(chessBoard, compColor,chessBoard->minimaxDepth);
			switch(spIsGameOver(chessBoard, compColor)) {
				case(CHECK):
					printf("%s",COMPUTER_CHECK_MESSAGE);
					break;
				case(WHITE_WON):
					printf("%s",WHITE_PLAYER_WINS_MESSAGE);
					spDestroyBoard(chessBoard);
					exit(EXIT_SUCCESS);
					break;
				case(BLACK_WON):
					printf("%s",BLACK_PLAYER_WINS_MESSAGE);
					spDestroyBoard(chessBoard);
					exit(EXIT_SUCCESS);
					break;
				case(TIE):
					printf("%s",COMPUTER_TIE_MESSAGE);
					spDestroyBoard(chessBoard);
					exit(EXIT_SUCCESS);
				case(NONE):
					break;
			}
			spPrintBoard(chessBoard);
		// we already switched turns in "spExecuteMove"
		}
	}
}

void spOpeningBoardPrint(SPChessBoard* chessBoard){
	if (chessBoard->gameMode == TWO_PLAYERS){
		spPrintBoard(chessBoard);
		}
	else{
		if (chessBoard->turn == USER){
			spPrintBoard(chessBoard);
		}
	}
}

void spSetInitialTurn(SPChessBoard* chessBoard){
	if(chessBoard->gameMode == ONE_PLAYER) {
		if(chessBoard->userColor == WHITE) {
			chessBoard->turn = USER;
		}
		else {
			chessBoard->turn = COMPUTER;
		}
	}
	else { // gameMode == TWO_PLAYERS
		chessBoard->turn = USER;
	}
}


void spSetUserColor(SPCommand* command, SPChessBoard* chessBoard){
	if (chessBoard->gameMode == TWO_PLAYERS){
			printf(INVALID_COMMAND_MESSAGE);
		}
	else{
		if ((!command->validArg) || ((command->arg != BLACK) && (command->arg != WHITE))){
			printf(WRONG_COLOR_MESSAGE);
		}
		else{
			chessBoard->userColor = command->arg; //sets the user color to either white or black
		}
	}
}


void spSetDifficulty(SPCommand* command, SPChessBoard* chessBoard){
	if (chessBoard->gameMode == TWO_PLAYERS){
		printf("%s",INVALID_COMMAND_MESSAGE);
	}
	else{
		if ((!command->validArg) || (command->arg < 1) || (command->arg > 5)){
			printf("%s",WRONG_DIFFICULTY_MESSAGE);
			return;
		}
		if (command->arg == 5){
			printf(EXPERT_NOT_SUPPORTED_MESSAGE);
			return;
		}
		else{
			chessBoard->minimaxDepth = command->arg; //sets the difficulty to 1,2,3 or 4
		}
	}
}
void spSetGameMode(SPCommand* command, SPChessBoard* chessBoard){
	if ((!command->validArg) || ((command->arg != 2) && (command->arg != 1))){
		printf(WRONG_GAME_MODE_MESSAGE);
	}
	else{
		chessBoard->gameMode = command->arg;
		if (command->arg == ONE_PLAYER){
			printf(GAME_MODE_ONE_PLAYER_MESSAGE);
		}
		else{
			printf(GAME_MODE_TWO_PLAYERS_MESSAGE);
		}
	}
}

void spPrintUserMessage(SPChessBoard* chessBoard){
	if (chessBoard == NULL){
		return;
	}
	if(chessBoard->userColor == WHITE) {
		printf("%s",WHITE_PLAYER_TURN_MESSAGE);
	}
	else {
		printf("%s",BLACK_PLAYER_TURN_MESSAGE);
	}
}
void spPrintMoveMessage(MOVE_MESSAGE message)
{
	switch (message)
	{
	case INVALID_POSITION:
		printf("%s",INVALID_POSITION_MESSAGE);
		break;
	case NO_LEGAL_PIECE:
		printf("%s",NOT_YOUR_PIECE_MESSAGE);
		break;
	case ILLEGAL_MOVE:
		printf("%s", ILLEGAL_MOVE_MESSAGE);
		break;
	default:
		return;
	}
}
void spQuit(SPChessBoard* chessBoard){
	if (chessBoard == NULL){
		return;
	}
	printf("Exiting...\n");
	spDestroyBoard(chessBoard);
	exit(EXIT_SUCCESS);
}

void spPrintSettings(SPChessBoard* chessBoard){
	if (chessBoard == NULL){
		return;
	}
	if (chessBoard->gameMode == TWO_PLAYERS){
		printf("SETTINGS:\n");
		printf("GAME_MODE: 2\n");
	}
	else{
		printf("SETTINGS:\n");
		printf("GAME_MODE: 1\n");
		printf("DIFFICULTY_LVL: %d\n", chessBoard->minimaxDepth);
		printf("USER_CLR: %s\n", spColorToUpperCaseStr(chessBoard->userColor));
	}
}

void spSettingsToDefault(SPChessBoard* chessBoard){
	chessBoard->gameMode = ONE_PLAYER;
	chessBoard->minimaxDepth = 2;
	chessBoard->userColor = WHITE;
	chessBoard->turn = WHITE;
}

int spUndoMove(SPChessBoard* chessBoard){
	char* error = (char*) malloc(SP_MAX_LINE_LENGTH * sizeof(char));
	if (chessBoard == NULL){
		errorHandling("malloc");
		exit(EXIT_FAILURE);
	}
	if (spIsValidUndoMove(chessBoard,&error) == 1){
		Move* computer_move = spArrayListGetFirst(chessBoard->history);
		printf("Undo move for player %s : <%c,%c> -> <%c,%c>\n",
				spColorToStr(!chessBoard->userColor),
				(char)(computer_move->to.row)+'1',
				(char)(computer_move->to.col)+'A',
				(char)(computer_move->from.row)+'1',
				(char)(computer_move->from.col)+'A');
		spUndoPrevMove(chessBoard, computer_move);

		Move* user_move = spArrayListGetFirst(chessBoard->history);
		printf("Undo move for player %s : <%c,%c> -> <%c,%c>\n",
				spColorToStr(chessBoard->userColor),
				(char)(user_move->to.row)+'1',
				(char)(user_move->to.col)+'A',
				(char)(user_move->from.row)+'1',
				(char)(user_move->from.col)+'A');
		spUndoPrevMove(chessBoard, user_move);

		free(error);
		return 1;
	}
	else{
		printf("%s",error);
		free(error);
		return -1;
	}
}

int spIsValidUndoMove(SPChessBoard *chessBoard, char** error){

	if (chessBoard->gameMode == TWO_PLAYERS){
		sprintf(*error,"Undo command not available in 2 players mode\n");
		return -1;
	}
	if (spArrayListIsEmpty(chessBoard->history)) {
		sprintf(*error, "Empty history, move cannot be undone\n");
		return -1;
	}
	return 1;
}

void spUndoPrevMove(SPChessBoard *chessBoard, Move* move){
	if (!chessBoard){
		return;
	}
	chessBoard->board[move->to.row][move->to.col] = move->taken_piece;
	chessBoard->board[move->from.row][move->from.col] = move->moved_piece;
	spArrayListRemoveFirst(chessBoard->history);
	switchTurns(chessBoard);
}


int getCurrentPlayerColor(SPChessBoard* chessBoard){
	int turn = chessBoard->turn;
	if (turn == USER){
		if (chessBoard->userColor == WHITE){
			return WHITE_PIECE;
		}
		else{
			return BLACK_PIECE;
		}
	}
	else{
		if (chessBoard->userColor == WHITE){
			return BLACK_PIECE;
		}
		else{
			return WHITE_PIECE;
		}
	}
}

void switchTurns(SPChessBoard* chessBoard){
	if(chessBoard->gameMode == TWO_PLAYERS) {
		chessBoard->userColor = !chessBoard->userColor;
	}
	else if (chessBoard-> turn == USER){
		chessBoard->turn = COMPUTER;
	}
	else{
		chessBoard->turn = USER;
	}
}


void spLoadSettingsFromFile(SPChessBoard* chessBoard, FILE* ifp) {
	int i,j;
	char boardRow[SP_MAX_LINE_LENGTH];
	int currentTurn, gameMode, minimaxDepth, userColor, rowNum1, rowNum2;

	fscanf(ifp,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<game>\n");
	fscanf(ifp,"\t<current_turn>%d</current_turn>\n",&currentTurn);
	fscanf(ifp,"\t<game_mode>%d</game_mode>\n",&gameMode);
	if(gameMode == ONE_PLAYER){
		fscanf(ifp,"\t<difficulty>%d</difficulty>\n",&minimaxDepth);
		fscanf(ifp,"\t<user_color>%d</user_color>\n",&userColor);
	}
	else{
		minimaxDepth = 2;
		userColor = 1;
	}
	fscanf(ifp,"\t<board>\n");
	chessBoard->gameMode = gameMode;
	chessBoard->minimaxDepth = minimaxDepth;
	chessBoard->userColor = userColor;

	spSetTurnAfterLoad(chessBoard,currentTurn,userColor);
	for(i = 0;i < 8 ; i++){
		fscanf(ifp,"\t\t<row_%d>%s</row_%d>\n",&rowNum1,boardRow,&rowNum2);
		for(j=0; j < 8; j++){
			char c = boardRow[j];
			chessBoard->board[7-i][j] = spGetPieceFromChar(c);
		}
	}
	fscanf(ifp,"\t</board>\n</game>\n");
	chessBoard->isLoadedGame = true;
}

void spSetTurnAfterLoad(SPChessBoard* chessBoard, int currentTurn, int userColor){


	if (chessBoard->gameMode == ONE_PLAYER){
		if (currentTurn == WHITE){
			chessBoard->turn = ((userColor == WHITE) ? USER : COMPUTER);
		}
		else{
			chessBoard->turn = ((userColor == BLACK) ? USER : COMPUTER);
		}
	}
	else{ // if we are in two players mode then turn will always be USER's
		chessBoard->turn = USER;
		if (currentTurn == WHITE){
			chessBoard->userColor = WHITE;
		}
		else{
			chessBoard->userColor = BLACK;
		}
	}

}

int spMovePiece(SPChessBoard * chessBoard, Square src, Square dst,
		int player_color, MOVE_MESSAGE* message){

	*message = spIsLegalMove(chessBoard, src, dst, player_color);
	if (*message == LEGAL_MOVE
			&& (!spIsNextTurnCheck(chessBoard,src,dst,player_color)))
	{
		spExecuteMove(chessBoard, src, dst);//handle errors
		return 1;
	}
	if (*message == LEGAL_MOVE)	//in case is_valid_move returned LEGAL_MOVE
								//but is_next_turn_check = true
		*message = ILLEGAL_MOVE;
	return 0;
}



void spSaveFile(SPChessBoard * chessBoard, char* fileName) {
	FILE *ofp = NULL;
	int i;
	char* boardRow;

	ofp = fopen(fileName,"w");
	if(ofp == NULL) {
		printf("%s",FILE_CANNOT_BE_CREATED);
		fclose(ofp);
		//free(file_name);
		spDestroyBoard(chessBoard);
		exit(EXIT_FAILURE);
	}

	fprintf(ofp, "%s\n", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	fprintf(ofp, "%s\n", "<game>");
	fprintf(ofp,"\t<current_turn>%d</current_turn>\n", chessBoard->turn);
	fprintf(ofp,"\t<game_mode>%d</game_mode>\n", chessBoard->gameMode);
	if (chessBoard->gameMode == ONE_PLAYER){
		fprintf(ofp,"\t<difficulty>%d</difficulty>\n",chessBoard->minimaxDepth);
		fprintf(ofp,"\t<user_color>%d</user_color>\n",chessBoard->userColor);
	}
	fprintf(ofp,"\t<board>\n\t");

	boardRow = (char*)malloc((8+1)*sizeof(char));
	if(boardRow == NULL) {
		errorHandling("malloc");
		free(boardRow);
		//free(file_name);
		fclose(ofp);
		spDestroyBoard(chessBoard);
		exit(EXIT_FAILURE);
	}
	for(i = 8 ; i > 0 ; i--) {
		spGetBoardRow(chessBoard, i, boardRow);
		fprintf(ofp, "\t%s%d%c%s%s%d%c\n\t","<row_",i,'>',boardRow,"</row_",i,'>');
	}
	fprintf(ofp,"</board>\n</game>");
	fclose(ofp);
}


void spGetBoardRow(SPChessBoard * chessBoard, int row, char* board_row) {
	int i;
	for(i = 0 ; i < 8 ; i++){
		board_row[i] = spGetPieceChar(chessBoard->board[row-1][i]);
	}
	board_row[i] = '\0';
}

const char* spColorToStr(int color)
{
	if (color == BLACK)
		return "black";
	return "white";
}

const char* spColorToUpperCaseStr(int color)
{
	if (color == BLACK)
		return "BLACK";
	return "WHITE";
}

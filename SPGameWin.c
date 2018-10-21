/*
 * gameWin.c
 *
 */
#include "SPGameWin.h"


SPGameWin* spGameWindowCreate(SPChessBoard* chessBoard) {

	SPGameWin* res = (SPGameWin*) calloc(sizeof(SPGameWin), sizeof(char));
	SDL_Surface* loadingSurface = NULL; // Used as temp surface
	if (res == NULL ) {
		return NULL ;
	}
	res->chessBoard = chessBoard;
	if (res->chessBoard == NULL ) {
		spGameWindowDestroy(res);
		return NULL ;
	}
	// Create an application window with the following settings:
	res->gameWindow = SDL_CreateWindow("SPChess", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			950,                               // width, in pixels
			720,								// height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->gameWindow == NULL) {
		// In case that the window could not be made
		printf("Could not create window: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->gameRenderer = SDL_CreateRenderer(res->gameWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->gameRenderer == NULL ) {
		printf("Could not create a renderer: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}

	//Create a background texture:
	loadingSurface = SDL_LoadBMP("./graphics/images/chessboard.bmp");
	if (loadingSurface == NULL ) {
		printf("Could not create a surface: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->chessBoardTexture = SDL_CreateTextureFromSurface(res->gameRenderer,
			loadingSurface);
	if (res->chessBoardTexture == NULL ) {
		printf("Could not create a texture: %s\n", SDL_GetError());
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);

	res->widgets = spCreateGameWindowWidgets(res->gameRenderer, res->chessBoard);
		if (res->widgets == NULL){
			// In case that the window could not be made
			spGameWindowDestroy(res);
			printf("Could not create widgets: %s\n", SDL_GetError());
			return NULL ;
		}
		printf("Created widgets successfully\n");
		res->numOfWidgets = GAMEWIM_WIDG_N + TOTAL_NUM_OF_CHESS_PIECES;
		res->startGame = true;
		res->saved = true;
		res->backToMain = false;
		return res;
}



Widget** spCreateGameWindowWidgets(SDL_Renderer* gameRenderer,SPChessBoard* chessBoard){
	int i,j, lastToInsert= 0;
	if (gameRenderer == NULL){
		return NULL ;
	}

	Widget** widgets = calloc(GAMEWIM_WIDG_N + TOTAL_NUM_OF_CHESS_PIECES+32, sizeof(Widget*));
	if (widgets == NULL){
		return NULL ;
	}
	SDL_Rect undo = {.x = 750,.y = 360,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect save = {.x =750,.y = 140,.h=BUTTON_HEIGTH,.w = BUTTON_WIDTH };
	SDL_Rect load = {.x = 750,.y = 250,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect restart = {.x = 750,.y = 30,.h = BUTTON_HEIGTH,.w = BUTTON_WIDTH };
	SDL_Rect mainMenu = {.x = 750,.y = 470,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect quit = {.x = 750,.y = 580,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	widgets[UNDO_BN] = spCreateSimpleButton(gameRenderer, &undo, "./graphics/images/buttons/undoactive.bmp","./graphics/images/buttons/undo.bmp",true);
	widgets[SAVE_BN] = spCreateSimpleButton(gameRenderer, &save, "./graphics/images/buttons/save.bmp",NULL,true);
	widgets[GAMEWIN_LOAD_BN] = spCreateSimpleButton(gameRenderer, &load, "./graphics/images/buttons/load.bmp",NULL,true);
	widgets[RESET_BN] = spCreateSimpleButton(gameRenderer, &restart, "./graphics/images/buttons/restart.bmp",NULL,true);
	widgets[MAIN_MENU_BN] = spCreateSimpleButton(gameRenderer, &mainMenu, "./graphics/images/buttons/main.bmp",NULL,true);
	widgets[GAMEWIN_QUIT_BN] = spCreateSimpleButton(gameRenderer, &quit, "./graphics/images/buttons/quit.bmp",NULL,true);

	lastToInsert = GAMEWIM_WIDG_N;

	for (i = 0; i < 8; i++){
		for (j= 0; j < 8; j++){
			if ((chessBoard->board[i][j] & PIECE_BIT_MASK) != EMPTY_PIECE){
				if((chessBoard->board[i][j] & PIECE_COLOR_BIT_MASK)==WHITE_PIECE){
					SDL_Rect pieceR = {.x = (j)*90, .y = (7-i)*90, .h = 90, .w = 90};
					char* path = spGetPieceImagePath(spGetPieceName(chessBoard->board[i][j]));
					widgets[lastToInsert] = spCreatePieceButton(gameRenderer, &pieceR,path,true);
					free(path);
					lastToInsert++;
				}
			}
		}
	}
	for (i = 0; i < 8; i++){
		for (j= 0; j < 8; j++){
			if ((chessBoard->board[i][j] & PIECE_BIT_MASK) != EMPTY_PIECE){
				if((chessBoard->board[i][j] & PIECE_COLOR_BIT_MASK)==BLACK_PIECE){
					SDL_Rect pieceR = {.x = (j)*90, .y = (7-i)*90, .h = 90, .w = 90};
					char* path = spGetPieceImagePath(spGetPieceName(chessBoard->board[i][j]));
					widgets[lastToInsert] = spCreatePieceButton(gameRenderer, &pieceR,path,true);
					free(path);
					lastToInsert++;
				}
			}
		}
	}
	for(i=0; i < GAMEWIM_WIDG_N+TOTAL_NUM_OF_CHESS_PIECES; i++){
		if(widgets[i] == NULL){
			printf("Could not create widget number %d",i);
			spGameWindowWidgetsDestroy(widgets,chessBoard);
			break;
		}
	}
	return widgets;
}

void spGameWindowDraw(SPGameWin* src,bool update){
	int i;
	if(src == NULL){
		return;
	}

	SDL_Rect rec = {.x = 0,.y = 0,.w = 720,.h = 720 };
	SDL_SetRenderDrawColor(src->gameRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->gameRenderer);
	SDL_RenderCopy(src->gameRenderer, src->chessBoardTexture, NULL, &rec);

	if(update == true){
		updateForDrawingTheGame(src);
	}

	// draws the widgets
	//-------------------
	for(i = 0; i <src->numOfWidgets; i++){
		(src->widgets[i])->drawWidget(src->widgets[i]);
	}

	SDL_RenderPresent(src->gameRenderer);

}
void updateForDrawingTheGame(SPGameWin* src){
	((SPSimpleButton*) src->widgets[UNDO_BN]->data)->active=false;
	int historysize=spArrayListSize(src->chessBoard->history);
	if(historysize != 0){
		if(src->chessBoard->gameMode == ONE_PLAYER){
			((SPSimpleButton*) src->widgets[UNDO_BN]->data)->active=true;
		}
	}
}

SP_GAME_EVENT spGameWindowHandleEvent(SPGameWin* src, SDL_Event* event){
	if (event == NULL || src == NULL){
		return SP_GAME_EVENT_INVALID_ARGUMENT;
	}
	switch (event->type){
	case SDL_MOUSEBUTTONUP:
		return spHandleButtonUpEvent(src,event);
		break;

	case SDL_MOUSEBUTTONDOWN:{
		return spHandleButtonDownEvent(src,event);
		break;
	}

	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE){
			return SP_GAME_EVENT_QUIT;
		}
		break;

	default:
		return SP_GAME_EVENT_NONE;
	}
	return SP_GAME_EVENT_NONE;
}


SP_GAME_EVENT spHandleButtonUpEvent(SPGameWin* src, SDL_Event* event){
	char* error = (char*) malloc(SP_MAX_LINE_LENGTH * sizeof(char));
	if (src->chessBoard == NULL){
		errorHandling("malloc");
		exit(EXIT_FAILURE);
	}
	if (src->widgets[RESET_BN]->handleEvent(src->widgets[RESET_BN],event) == 1){
		return spHandleResetEventGame(src);
	}
	else if (src->widgets[SAVE_BN]->handleEvent(src->widgets[SAVE_BN],event) == 1){
		src->saved = true;
		return SP_GAME_EVENT_SAVE;
	}
	else if (src->widgets[GAMEWIN_LOAD_BN]->handleEvent(src->widgets[GAMEWIN_LOAD_BN],event) == 1){
		src->saved = true;
		return SP_GAME_EVENT_LOAD;
	}
	else if (src->widgets[UNDO_BN]->handleEvent(src->widgets[UNDO_BN],event) == 1){
		if (spIsValidUndoMove(src->chessBoard,&error) == 1){
			Move* computer_move = spArrayListGetFirst(src->chessBoard->history);
			spUndoPrevMove(src->chessBoard, computer_move);
			if(src->chessBoard->board[computer_move->from.row][computer_move->from.col] != EMPTY_PIECE){
				for(int k=GAMEWIM_WIDG_N; k<src->numOfWidgets; k++){
					if(((SPPieceButton*)src->widgets[k]->data)->location->x==computer_move->from.col*90 && (((SPPieceButton*)src->widgets[k]->data)->location->y==(7-computer_move->from.row)*90)){
						((SPPieceButton*)src->widgets[k]->data)->on=false;
					}
				}
			}
			for(int k = GAMEWIM_WIDG_N; k < src->numOfWidgets; k++){
				if(((SPPieceButton*)src->widgets[k]->data)->location->x==computer_move->to.col*90 && (((SPPieceButton*)src->widgets[k]->data)->location->y==(7-computer_move->to.row)*90)){
					((SPPieceButton*)src->widgets[k]->data)->location->x = computer_move->from.col*90;
					((SPPieceButton*)src->widgets[k]->data)->location->y = (7 - computer_move->from.row)*90;
					((SPPieceButton*)src->widgets[k]->data)->on=true;
				}
			}
			Move* user_move = spArrayListGetFirst(src->chessBoard->history);
			spUndoPrevMove(src->chessBoard, user_move);
			if(src->chessBoard->board[user_move->from.row][user_move->from.col] != EMPTY_PIECE){
				for(int k=GAMEWIM_WIDG_N; k<src->numOfWidgets; k++){
					if(((SPPieceButton*)src->widgets[k]->data)->location->x==user_move->from.col*90 && (((SPPieceButton*)src->widgets[k]->data)->location->y==(7-user_move->from.row)*90)){
						((SPPieceButton*)src->widgets[k]->data)->on=false;
					}
				}
			}
			for(int k = GAMEWIM_WIDG_N; k < src->numOfWidgets; k++){
				if(((SPPieceButton*)src->widgets[k]->data)->location->x==user_move->to.col*90 && (((SPPieceButton*)src->widgets[k]->data)->location->y==(7-user_move->to.row)*90)){
					((SPPieceButton*)src->widgets[k]->data)->location->x = user_move->from.col*90;
					((SPPieceButton*)src->widgets[k]->data)->location->y = (7 - user_move->from.row)*90;
					((SPPieceButton*)src->widgets[k]->data)->on=true;
				}
			}
			src->saved = false;
		}
		return SP_GAME_EVENT_UPDATE;
	}
	else if (src->widgets[MAIN_MENU_BN]->handleEvent(src->widgets[MAIN_MENU_BN],event) == 1){
		src->backToMain = true;
		return SP_GAME_EVENT_MAIN_MENU;
	}

	else if (src->widgets[GAMEWIN_QUIT_BN]->handleEvent(src->widgets[GAMEWIN_QUIT_BN],event) == 1){
		return SP_GAME_EVENT_QUIT;
	}

	return SP_GAME_EVENT_NONE;
}


SP_GAME_EVENT spHandleResetEventGame(SPGameWin* src){
	spGameWindowWidgetsDestroy(src->widgets,src->chessBoard);
	SPChessBoard* tempChessBoard = src->chessBoard;
	SPChessBoard* newChessBoard = spInitGame();
	src->chessBoard = newChessBoard;
	src->chessBoard->gameMode = tempChessBoard->gameMode;
	src->chessBoard->minimaxDepth = tempChessBoard->minimaxDepth;
	src->chessBoard->userColor = tempChessBoard->userColor;
	spDestroyBoard(tempChessBoard);
	spPrintBoard(src->chessBoard);
	src->saved = false;
	src->widgets = spCreateGameWindowWidgets(src->gameRenderer, src->chessBoard);
		if(src->chessBoard->gameMode == ONE_PLAYER){	// in case the computer's color is white - comp should start
			if(src->chessBoard->userColor == BLACK){
				switchTurns(src->chessBoard);
				int compColor = getCurrentPlayerColor(src->chessBoard);
				spGetComputerMove(src->chessBoard, compColor,src->chessBoard->minimaxDepth);
//				int res = spIsGameOver(src->chessBoard, compColor);
//				if(spHandleCheckCheckmateOrTie(src,res)==SP_GAME_EVENT_GAMEOVER){
//					return SP_GAME_EVENT_GAMEOVER;
//				}

			}
		}
	return SP_GAME_EVENT_UPDATE;
}

SP_GAME_EVENT spHandleButtonDownEvent(SPGameWin* src,SDL_Event* event){
	int i;
	int clickedPieceIndex = -1;
	Square from;
	Square to;

	unsigned int playerColor =  getCurrentPlayerColor(src->chessBoard);
	for (i = 0; i < src->numOfWidgets-GAMEWIM_WIDG_N; i++){
		if (src->widgets[i+GAMEWIM_WIDG_N]->handleEvent(src->widgets[i+GAMEWIM_WIDG_N],event) == 1){
			from.col = event->button.x/90;
			from.row = 7 - event->button.y/90;
			if(src->chessBoard->board[from.row][from.col] != EMPTY_PIECE){
				if((src->chessBoard->board[from.row][from.col] & PIECE_COLOR_BIT_MASK) == playerColor){
					clickedPieceIndex = i+GAMEWIM_WIDG_N;
				}
			}
		}
	}
	if (clickedPieceIndex != -1){
			SDL_Event motionEvent;
			SDL_WaitEvent(&motionEvent);
			while(motionEvent.type == SDL_MOUSEMOTION){
				((SPPieceButton*)src->widgets[clickedPieceIndex]->data)->location->x = motionEvent.motion.x-45;
				((SPPieceButton*)src->widgets[clickedPieceIndex]->data)->location->y = motionEvent.motion.y-45;
				spGameWindowDraw(src,false);
				SDL_WaitEvent(&motionEvent);
				if(motionEvent.type != SDL_MOUSEMOTION){
					if(src->chessBoard->board[7 - motionEvent.button.y/90][motionEvent.button.x/90]!=EMPTY_PIECE){
						for(int k = GAMEWIM_WIDG_N; k<src->numOfWidgets; k++){
							if(((SPPieceButton*)src->widgets[k]->data)->location->x==motionEvent.button.x/90 && (((SPPieceButton*)src->widgets[k]->data)->location->y== 7-motionEvent.button.y/90)){
								((SPPieceButton*)src->widgets[k]->data)->on=false;

							}
						}
					}
					break;
				}
			}
			to.col = motionEvent.button.x/90;
			to.row = 7 - motionEvent.button.y/90;
			MOVE_MESSAGE message;
			message = spIsLegalMove(src->chessBoard, from,to,playerColor);
			if (message == LEGAL_MOVE&&(!spIsNextTurnCheck(src->chessBoard,from,to,playerColor)))
				{
				if(src->chessBoard->board[to.row][to.col]!=EMPTY_PIECE){
					for(int k = GAMEWIM_WIDG_N; k < src->numOfWidgets;k++){
						if(((SPPieceButton*)src->widgets[k]->data)->location->x==to.col*90 && (((SPPieceButton*)src->widgets[k]->data)->location->y==(7-to.row)*90)){
							((SPPieceButton*)src->widgets[k]->data)->on=false;
						}
					}
				}
				spExecuteMove(src->chessBoard, from,to);
				for(int k = GAMEWIM_WIDG_N; k < src->numOfWidgets;k++){
					if(((SPPieceButton*)src->widgets[k]->data)->location->x==from.col*90 && (((SPPieceButton*)src->widgets[k]->data)->location->y==(7-from.row)*90)){
						((SPPieceButton*)src->widgets[k]->data)->location->x =to.col*90;
						((SPPieceButton*)src->widgets[k]->data)->location->y = (7 -to.row)*90;
					}
				}
				src->saved = false;
				((SPPieceButton*)src->widgets[clickedPieceIndex]->data)->location->x = to.col*90;
				((SPPieceButton*)src->widgets[clickedPieceIndex]->data)->location->y = (7 - to.row)*90;
				spGameWindowDraw(src,true);
				int res = spIsGameOver(src->chessBoard, playerColor);
				if(spHandleCheckCheckmateOrTie(src,res)==SP_GAME_EVENT_GAMEOVER){
					return SP_GAME_EVENT_GAMEOVER;
				}
				if (src->chessBoard->gameMode == ONE_PLAYER){
					int compColor = getCurrentPlayerColor(src->chessBoard);
					Move* move = spGetComputerMoveForGui(src->chessBoard, compColor, src->chessBoard->minimaxDepth);
					if(src->chessBoard->board[move->to.row][move->to.col] != EMPTY_PIECE){
						for(int k=GAMEWIM_WIDG_N; k<src->numOfWidgets; k++){
							if(((SPPieceButton*)src->widgets[k]->data)->location->x==move->to.col*90 && (((SPPieceButton*)src->widgets[k]->data)->location->y==(7-move->to.row)*90)){
								((SPPieceButton*)src->widgets[k]->data)->on=false;
							}
						}
					}
					spExecuteMove(src->chessBoard,move->from,move->to);
					for(int k = GAMEWIM_WIDG_N; k < src->numOfWidgets; k++){
						if(((SPPieceButton*)src->widgets[k]->data)->location->x==move->from.col*90 && (((SPPieceButton*)src->widgets[k]->data)->location->y==(7-move->from.row)*90)){
							((SPPieceButton*)src->widgets[k]->data)->location->x = move->to.col*90;
							((SPPieceButton*)src->widgets[k]->data)->location->y = (7 - move->to.row)*90;
						}
					}
					((SPPieceButton*)src->widgets[clickedPieceIndex]->data)->location->x = to.col*90;
					((SPPieceButton*)src->widgets[clickedPieceIndex]->data)->location->y = (7 - to.row)*90;
					spGameWindowDraw(src,true);
					int res = spIsGameOver(src->chessBoard, playerColor);
					if(spHandleCheckCheckmateOrTie(src,res)==SP_GAME_EVENT_GAMEOVER){
						return SP_GAME_EVENT_GAMEOVER;
					}
				}
			}
			else{
				((SPPieceButton*)src->widgets[clickedPieceIndex]->data)->location->x = from.col*90;
				((SPPieceButton*)src->widgets[clickedPieceIndex]->data)->location->y = (7-from.row)*90;
			}
			spPrintBoard(src->chessBoard);
		}
		return SP_GAME_EVENT_UPDATE;
}

SP_GAME_EVENT spHandleCheckCheckmateOrTie(SPGameWin* src, int res){
	switch(res) {
		case(CHECK):
			if (src->chessBoard->userColor == WHITE){
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Check!","Black king is under check",NULL);
			}
			else{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Check!","White king is under check",NULL);
			}
			break;
		case(WHITE_WON):
				if (src->chessBoard->gameMode == ONE_PLAYER){
					if (src->chessBoard->userColor == WHITE){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Game over!","YOU'VE WON!",NULL);
					}
					else{
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Game over!","YOU'VE LOST!",NULL);
					}
				}
				else if (src->chessBoard->gameMode == TWO_PLAYERS){
					if (src->chessBoard->userColor == WHITE){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Game over!","WHITE WINS!",NULL);
					}
					else{
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Game over!","BLACK WINS!",NULL);
					}
				}
		return SP_GAME_EVENT_GAMEOVER;
		break;

		case (BLACK_WON):
			if (src->chessBoard->gameMode == ONE_PLAYER){
				if (src->chessBoard->userColor == BLACK){
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Game over!","YOU'VE WON!",NULL);
				}
				else{
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Game over!","YOU'VE LOST!",NULL);
				}
			}
			else if (src->chessBoard->gameMode == TWO_PLAYERS){
				if (src->chessBoard->userColor == BLACK){
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Game over!","BLACK WINS!",NULL);
				}
				else{
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Game over!","WHITE WINS!",NULL);
				}
			}
		return SP_GAME_EVENT_GAMEOVER;
		break;

	  case(TIE):
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Game over!","IT'S A TIE!",NULL);
	  	  	return SP_GAME_EVENT_GAMEOVER;
	  break;

	  case(NONE):
			return SP_GAME_EVENT_NONE;
		break;
	}

		return SP_GAME_EVENT_NONE;
}

void spGameWindowWidgetsDestroy(Widget** widgets,SPChessBoard* chessBoard){
	int i;

	if (chessBoard == NULL){
		return;
	}
	for(i = 0; i < GAMEWIM_WIDG_N + TOTAL_NUM_OF_CHESS_PIECES; i++){
		if(widgets[i] != NULL){
			destroyWidget(widgets[i]); //NULL SAFE
		}
	}
	free(widgets);
}


void spGameWindowDestroy(SPGameWin* src){
	if (src == NULL){
		return;
	}
	spGameWindowWidgetsDestroy(src->widgets, src->chessBoard);
	spDestroyBoard(src->chessBoard);
	if (src->chessBoardTexture != NULL){
		SDL_DestroyTexture(src->chessBoardTexture);
	}
	if (src->gameRenderer != NULL){
		SDL_DestroyRenderer(src->gameRenderer);
	}
	if (src->gameWindow != NULL){
		SDL_DestroyWindow(src->gameWindow);
	}
	free(src);

}


void spGameWindowHide(SPGameWin* src){
	SDL_HideWindow(src->gameWindow);
}

void spGameWindowShow(SPGameWin* src){
	SDL_ShowWindow(src->gameWindow);
}

Move * spGetComputerMoveForGui(SPChessBoard * chess_board, int computer_color, int minimax_depth) {

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

		if(computer_color == WHITE_PIECE) {
			curr_move_score = spMinimax(chess_board, INT_MIN, INT_MAX, BLACK_PIECE , computer_color,minimax_depth-1);
		}
		else {
			curr_move_score = spMinimax(chess_board, INT_MIN, INT_MAX, WHITE_PIECE , computer_color, minimax_depth-1);
		}
		spUndoPrevMove(chess_board, curr_move);
		if(curr_move_score >= best_value) {
			best_value = curr_move_score;
			best_move = curr_move;
			if (curr_move_score == INT_MAX)
			{
				spArrayListDestroy(all_moves);
				return best_move;
			}
		}
	}
	spPrintMove(best_move);
	spArrayListDestroy(all_moves);
	return best_move;
}

/*
 * main_game.h
 *
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SPArrayList.h"
#include "SPMinimax.h"
#include "SPParser.h"
#include "SPMovePiece.h"

#define USER 1
#define COMPUTER 0

// All game messages
#define SETTINGS_STATE_MESSAGE "Specify game setting or type 'start' to begin a game with the current setting:\n"
#define WRONG_GAME_MODE_MESSAGE "Wrong game mode\n"
#define GAME_MODE_ONE_PLAYER_MESSAGE "Game mode is set to 1 player\n"
#define GAME_MODE_TWO_PLAYERS_MESSAGE "Game mode is set to 2 players\n"
#define FILE_NOT_FOUND_MESSAGE	"Error: File doesn't exist or cannot be opened\n"
#define FILE_CANNOT_BE_CREATED "File doesn’t exist or cannot be opened\n"
#define WRONG_DIFFICULTY_MESSAGE "Wrong difficulty level. The value should be between 1 to 5\n"
#define EXPERT_NOT_SUPPORTED_MESSAGE "Expert level not supported, please choose a value between 1 to 4:\n"
#define INVALID_COMMAND_MESSAGE "Invalid command\n"
#define WRONG_COLOR_MESSAGE "Wrong color. The value should be either 0 or 1\n"
#define WHITE_PLAYER_TURN_MESSAGE "white player - enter your move:\n"
#define BLACK_PLAYER_TURN_MESSAGE "black player - enter your move:\n"
#define WHITE_KING_CHECK_MESSAGE "Check: white King is threatened!\n"
#define BLACK_KING_CHECK_MESSAGE "Check: black King is threatened!\n"
#define COMPUTER_CHECK_MESSAGE "Check!\n"
#define WHITE_PLAYER_WINS_MESSAGE "Checkmate! white player wins the game\n"
#define BLACK_PLAYER_WINS_MESSAGE "Checkmate! black player wins the game\n"
#define COMPUTER_TIE_MESSAGE "The game ends in a tie\n"
#define TIE_MESSAGE "The game is tied\n"


/*this enum inculdes all the options of things or instructions that the user can do during
 * a game of chess.
 */
typedef enum {
	SP_MOVE,
	SP_GET_MOVES,
	SP_SAVE,
	SP_UNDO,
	SP_RESET,
	SP_INVALID_COMMAND
}SP_GAME_COMMAND;


// Error handling message
#define errorHandling(function) (printf("ERROR: %s has failed", function));

/*this function inits new chess game according to settings that the user specifies.
 */
int spMainGame();

/* the function initilizes a new game according to the default settings: one human player against the computer,
 * the user play as the white player, the difficulty is easy and the turn is of the user.
 * the function return reference of SPChessBoard.
 */
SPChessBoard* spInitGame();

/*the function get from the user the instructions of the setting state mode and update
 * the chess board according them.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spSettingsState(SPChessBoard *chess_board);

/*
 * this function define who will start the game, who will play first: the user or the computer.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spSetInitialTurn(SPChessBoard* chessBoard);

/*
 * this function print the board of the chess game. this function is called before the game acatually started to play.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spOpeningBoardPrint(SPChessBoard* chessBoard);
/*
 * this function set the color of the user: black or white.
 * @param chessBoard - a reference to chess board.
 * @param command - a reference to SPCommand: that the user type.
 * the function doesn't return nothing.
 */
void spSetUserColor(SPCommand* command, SPChessBoard* chessBoard);

/*
 * this function set the level of difficulty of the game of : noob, easy, moderate or expert.
 * @param chessBoard - a reference to chess board.
 * @param command - a reference to SPCommand: that the user type.
 * the function doesn't return nothing.
 */
void spSetDifficulty(SPCommand* command, SPChessBoard* chessBoard);

/*
 * this function set the how much human beings play the game : one user or two users.
 * @param chessBoard - a reference to chess board.
 * @param command - a reference to SPCommand: that the user type.
 * the function doesn't return nothing.
 */
void spSetGameMode(SPCommand* command, SPChessBoard* chessBoard);

/* this function print the current configuration of the setting as the game has right now.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spPrintSettings(SPChessBoard* chessBoard);

/* this function prints messages to the user according who will execute the turn: the white player or the black player.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spPrintUserMessage(SPChessBoard* chessBoard);

/* this function tell us who is play now: black or white.
 * @param chessBoard - a reference to chess board.
 * the return int: 8- White_PIECE  OR 16: Black piece.
 */
int getCurrentPlayerColor(SPChessBoard* chessBoard);

/*the traget of this function is to quit a game.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spQuit(SPChessBoard* chessBoard);

/* this function load the settings from a file and updated the game according them.
 * @param chessBoard - a reference to chess board.
 * @prama ifp - a refernce to file.
 * the function doesn't return nothing.
 */
void spLoadSettingsFromFile(SPChessBoard *chess_board, FILE *ifp);

/*this function execute move of a piece if it is valid.
 * @param chessBoard - a reference to chess board.
 * @ src - the Squre of the source.
 * @ dst - the Squre of the destination.
 * @ player_color - the color of the player that want to move.
 * the function return int: 1 if everyting was work fine -1 else
 */
int spMovePiece(SPChessBoard *chess_board, Square src, Square dst, int player_color, MOVE_MESSAGE* message);

/*this functions is the heart of a running game, it means that it stimulates a legal chess game.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spGameState(SPChessBoard *chess_board);

/*
 * this function prints a relevant message according
 * to the move the player is trying to do
 * and weather the move is valid or not
 * @param - message - the appropriate message
 */

void spPrintMoveMessage(MOVE_MESSAGE message);

/* this function change the setting to the default settings.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void spSettingsToDefault(SPChessBoard* chess_board);

/*this function crerate new file with the name of file_name and save the chessBoard in it.
 * @param chessBoard - a reference to chess board.
 * @ file_name - char* of the name of the file.
 * the function doesn't return nothing.
 */
void spSaveFile(SPChessBoard * chess_board, char* file_name);


/*this function take a chess board and translate one specific row
 * @param chessBoard - a reference to chess board.
 * @ row - int that mention of specific row.
 * @ board_row - char* that we uptaded according to the operate of the function.
 * the function doesn't return nothing.
 */
void spGetBoardRow(SPChessBoard *chess_board, int row, char* board_row);

/* this function change the turn: from white turn to black turn and form black turn to white turn.
 * @param chessBoard - a reference to chess board.
 * the function doesn't return nothing.
 */
void switchTurns(SPChessBoard* chessBoard);
/* this function execute an undo move, it means that it cancel the last move of the user and the last
 * move of the computer.
 * @param chessBoard - a reference to chess board.
 * the function return int: 1 if everyting was work fine -1 else
 */
int spUndoMove(SPChessBoard* chessBoard);

/* this function check if it valid to execute an undo move.
 * @param error - a refernce to a string.
 * @param chessBoard - a reference to chess board.
 * the function return int: 1 if everyting was work fine -1 else
 */
int spIsValidUndoMove(SPChessBoard *chessBoard, char** error);

/* this function acatually execute an undo move.
 * @param move - the move that we have to undo.
 * @param chessBoard - a reference to chess board.
 * the function return int: 1 if everyting was work fine -1 else
 */
void spUndoPrevMove(SPChessBoard *chess_board, Move* move);

/*this function gets an int and return which color it represent.
 * @param color - int of the color
 * the function return "black" if color == 0 and "white" otherwise.
 */
const char* spColorToStr(int color);

/*this function gets an int and return which color it represent in upper case.
 * @param color - int of the color
 * the function return "BLACK" if color == 0 and "WHITE" otherwise.
 */
const char* spColorToUpperCaseStr(int color);

/*this function sets the current turn of the game by the parmeters
 * @param currentTurn - the color of the user who is about to play
 * userColor - the user's color
 */
void spSetTurnAfterLoad(SPChessBoard* chessBoard, int currentTurn, int userColor);


#endif /* MAIN_H_ */

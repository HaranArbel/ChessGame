/*
 * parser.h
 *
 */

#ifndef SPPARSER_H_
#define SPPARSER_H_


//specify the maximum line length
#define SP_MAX_LINE_LENGTH 1024
#include <stdbool.h>
#include <stddef.h>


//a type used to represent a command
typedef enum {
	SP_GAME_MODE,
	SP_DIFFICULTY,
	SP_USER_COLOR,
	SP_LOAD,
	SP_DEFAULT,
	SP_PRINT_SETTING,
	SP_START,
	SP_INVALID_LINE,
	SP_QUIT
}SP_COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	SP_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg;
	char *path;
} SPCommand;


/**
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool spParserIsInt(const char* str);

/**
 * Parses a specified line. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true. In any other case then 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command is add_disc and the integer argument
 *              is valid
 *   arg      - the integer argument in case validArg is set to true
 */
SPCommand * spParserPraseLine(const char* str);

/**
 * Checks if a specified string represents a valid command.
 *
 * @return
 * the type of the command if the string represents a valid command, and SP_INVALID_LINE otherwise.
 */
int spParserIsCommand(char* command);

/*
 * this function get a command and a string that
 * represents a number and converts it to int
 * the function then updates the command argument
 * to be the convertet int
 */
void spParserSetArg(char* token, SPCommand* command);
/*
 * this function gets an input string from the
 * user.
 *
 */
char * spGetInputString();

/*this function gets a input string from the
 * parses it and make it into a command
 * the function returns the command
 */

SPCommand * spGetCommand ();

/*this function parses the move command
 * according to the format
 * returns 1 if the command is valid,
 * -1 otherwise.
 */

int spParseMoveCommand(char *command, char **error);

#endif /* SPPARSER_H_ */

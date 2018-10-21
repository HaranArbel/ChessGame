/*
 * parser.c
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include "SPParser.h"

bool spParserIsInt(const char* str){

	if (*str == '-') 	// if the number is negative
		++str;

	if (!*str)
		return false;

	while (*str)		// if some character is not a digit
		if (!isdigit(*str))
			return false;
		else ++str;

	return true;
}

SPCommand* spParserPraseLine(const char* str){
	setbuf(stdout,NULL);

	char* stringCopy = (char*) malloc(strlen(str)+1);
	strcpy(stringCopy, str);
	SPCommand *command = (SPCommand*)malloc(sizeof(SPCommand));
	if (!command){
		return NULL;
	}

	command->cmd = SP_INVALID_LINE;
	command->validArg = false;

	bool gotCommand = false;
	bool is_game_mode = false;
	bool is_difficulty = false;
	bool is_user_color = false;
	bool is_load = false;

	char* token = strtok(stringCopy, " \t\r\n");

	if (strcmp(str,"")==0){
		command->cmd = SP_INVALID_LINE;
		return command;
	}

	while (token){

		if (!gotCommand){

			gotCommand = true;
			int cmd = spParserIsCommand(token);

			switch(cmd){

			case SP_INVALID_LINE:
				command->cmd = SP_INVALID_LINE;
				break;
			case SP_GAME_MODE:
				is_game_mode = true;
				break;
			case SP_DIFFICULTY:
				is_difficulty = true;
				break;
			case SP_USER_COLOR:
				is_user_color = true;
				break;
			case SP_LOAD:
				is_load = true;
				break;
			case SP_DEFAULT:
				command->cmd = cmd;
				break;
			case SP_PRINT_SETTING:
				command->cmd = cmd;
				break;
			case SP_QUIT:
				command->cmd = cmd;
				break;
			case SP_START:
				command->cmd = cmd;
				break;

			}

		}
		else if(is_game_mode)
		{
			spParserSetArg(token, command);
			command->cmd = SP_GAME_MODE;
			break;
		}
		else if(is_difficulty)
		{
			spParserSetArg(token, command);
			command->cmd = SP_DIFFICULTY;
			break;
		}
		else if(is_user_color)
		{
			spParserSetArg(token, command);
			command->cmd = SP_USER_COLOR;
			break;
		}
		else if(is_load)
		{
			command->path = (char*) malloc(strlen(token)+1);
			strcpy(command->path,token);
			command->cmd = SP_LOAD;
			break;
		}
		else
			command->cmd = SP_INVALID_LINE;

		token = strtok(NULL, " \t\r\n");

	}
	free(stringCopy);
	return command;

}

void spParserSetArg(char* token, SPCommand* command){

	int input_num = atoi(token);
	if(!spParserIsInt(token))
		command->validArg = false;
	else
		command->validArg = true;

	command->arg = input_num;
}

int spParserIsCommand(char* command){

	if (strcmp(command, "game_mode") == 0)
		return SP_GAME_MODE;
	if (strcmp(command,"difficulty") == 0)
		return SP_DIFFICULTY;
	if (strcmp(command,"user_color") == 0)
		return SP_USER_COLOR;
	if (strcmp(command,"load") == 0)
		return SP_LOAD;
	if (strcmp(command,"default") == 0)
		return SP_DEFAULT;
	if (strcmp(command,"print_setting") == 0)
		return SP_PRINT_SETTING;
	if (strcmp(command,"quit") == 0)
		return SP_QUIT;
	if (strcmp(command,"start") == 0)
		return SP_START;


	return SP_INVALID_LINE;
}



char * spGetInputString(){
	char *str;
	str = (char*) malloc(SP_MAX_LINE_LENGTH * sizeof(char));
	if(!str){
		//errorHandling("malloc");
		return NULL;
	}
	if (fgets(str, SP_MAX_LINE_LENGTH, stdin) ==NULL){
		//errorHandling("fgets");
		free(str);
		return NULL;
	}
	return str;
}

SPCommand * spGetCommand (){
	char *str;
	str = spGetInputString();
	if (str == NULL){
		return NULL;
	}
	SPCommand *command = spParserPraseLine(str);
	free(str);
	return command;
}


int spParseMoveCommand(char *command, char **error){

	if (command[6] > '8'|| command[6] < '1'|| command[8] > 'H'|| command[8] < 'A'||
				command[15] > '8' || command[15] < '1' || command[17] > 'H' || command[17] < 'A' ){
				*error = (char*) malloc(SP_MAX_LINE_LENGTH * sizeof(char));
				sprintf(*error,"Invalid position on the board\n");
				return -1;
	}

	if(command[5] != '<' || command[7] != ',' ||command[9] != '>' || command[11] != 't'
				|| command[12] != 'o' || command[14] != '<' || command[16] != ',' || command[18] != '>') {
			*error = (char*) malloc(SP_MAX_LINE_LENGTH * sizeof(char));
			sprintf(*error,"Invalid command\n");
			return -1;
	}



	return 1;
}

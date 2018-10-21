/*
 * main.c
 */

#include "MainGame.h"
#include "MainGUI.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]){

	// start in console mode
	if (argc == 1 || ( argc == 2 && (strcmp(argv[1],"-c") == 0))){
		spMainGame();
	}
	// start in GUI mode
	else{
		main_gui();
	}
}


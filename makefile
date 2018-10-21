CC = gcc
OBJS = SPArrayList.o SPBoard.o MainGame.o Main.o SPMinimax.o SPMovePiece.o SPParser.o MainGUI.o SPCommon.o SPGameWin.o SPGUIManager.o SPLoadWin.o SPMainWin.o SPPieceButton.o SPSettingsWin.o SPSimpleButton.o SPWidget.o
EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -g
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
SPArrayList.o: SPArrayList.c SPArrayList.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPBoard.o: SPBoard.c SPBoard.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
MainGame.o: MainGame.c MainGame.h SPParser.h SPArrayList.h SPMinimax.h SPMovePiece.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Main.o: Main.c MainGame.h MainGUI.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPMinimax.o: SPMinimax.c SPMinimax.h SPMovePiece.h MainGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPMovePiece.o: SPMovePiece.c SPMovePiece.h SPBoard.h SPArrayList.h MainGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPParser.o: SPParser.c SPParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
MainGUI.o: MainGUI.c MainGUI.c MainGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPCommon.o: SPCommon.c SPCommon.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPGameWin.o: SPGameWin.c SPGameWin.h MainGame.h SPSimpleButton.h SPPieceButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPGUIManager.o: SPGUIManager.c SPGUIManager.h SPGameWin.h SPSettingsWin.h SPMainWin.h SPLoadWin.h MainGUI.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPLoadWin.o: SPLoadWin.c SPLoadWin.h SPWidget.h SPSimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPMainWin.o: SPMainWin.c SPMainWin.h SPSimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPPieceButton.o: SPPieceButton.c SPPieceButton.h SPBoard.h SPCommon.h SPWidget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPSettingsWin.o: SPSettingsWin.c SPSettingsWin.h SPSimpleButton.h SPBoard.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPSimpleButton.o: SPSimpleButton.c SPSimpleButton.h SPCommon.h SPWidget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPWidget.o: SPWidget.c SPWidget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)

/*
 * PiecesButton.c
 *
 */
#include "SPPieceButton.h"


Widget* spCreatePieceButton(SDL_Renderer* windowRender, SDL_Rect* location, const char* image, bool on) {
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	SPPieceButton* data = (SPPieceButton*) malloc(sizeof(SPPieceButton));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	if (res == NULL || data == NULL || loadingSurface == NULL|| buttonTexture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //Surface is not actually needed after texture is created
	data->buttonTexture = buttonTexture;
	data->location = CopyRect(location);
	data->windowRenderer = windowRender;
	data->on = on;
	res->destroyWidget = spDestroyPieceButton;
	res->drawWidget =spDrawPieceButton;
	res->handleEvent = spHandlePieceButtonEvenet;
	res->data = data;
	return res;
}


void spDestroyPieceButton(Widget* src) {
	if (src == NULL ) {
		return;
	}
	SPPieceButton* castData = (SPPieceButton*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->buttonTexture);
	free(castData);
	free(src);
}

void setPieceButtonTexture(Widget* src, const char* image){
	if (src == NULL) {
		return;
	}
	SPPieceButton* castData = (SPPieceButton*) src->data;
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(castData->windowRenderer,loadingSurface);
	if (loadingSurface == NULL|| buttonTexture == NULL) {
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
		return ;
	}
	SDL_FreeSurface(loadingSurface);
	castData->buttonTexture = buttonTexture;

}

int spHandlePieceButtonEvenet(Widget* src, SDL_Event* event) {
	if (src == NULL || event == NULL) {
		return 0;
	}

	SPPieceButton* castData = (SPPieceButton*) src->data;
	if(castData->on == false){
		return 0;
	}

	if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			return 1;
		}
	}
	return 0;
}

void spDrawPieceButton(Widget* src) {
	if (src == NULL ) {
		return;
	}
	SPPieceButton* castData = (SPPieceButton*) src->data;
	if(castData->on == true){
		SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,	castData->location);
	}
}

char* spGetPieceImagePath(char* piece_name){
	char* path = (char*)calloc(50,sizeof(char));
	char name[PIECE_NAME_LEN],ending[STR_ENDING_LEM];
	strcpy(path,"./graphics/images/pieces/");
	strcpy(name,piece_name);
	strcpy(ending,".bmp");
	strcat(path,name);
	strcat(path,ending);
	return path;
}

char* spGetPieceName(unsigned int piece){
	if ((piece & PIECE_BIT_MASK) == PAWN_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return "WhitePawn";
	else if ((piece & PIECE_BIT_MASK) == ROOK_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return "WhiteRook";
	else if ((piece & PIECE_BIT_MASK) == BISHOP_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return "WhiteBishop";
	else if ((piece & PIECE_BIT_MASK) == KNIGHT_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return "WhiteKnight";
	else if ((piece & PIECE_BIT_MASK) == QUEEN_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return "WhiteQueen";
	else if ((piece & PIECE_BIT_MASK) == KING_PIECE && (piece & PIECE_COLOR_BIT_MASK) == WHITE_PIECE) return "WhiteKing";

	else if ((piece & PIECE_BIT_MASK) == PAWN_PIECE && (piece & PIECE_COLOR_BIT_MASK )== BLACK_PIECE) return "BlackPawn";
	else if ((piece & PIECE_BIT_MASK) == ROOK_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return "BlackRook";
	else if ((piece & PIECE_BIT_MASK) == BISHOP_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return "BlackBishop";
	else if ((piece & PIECE_BIT_MASK) == KNIGHT_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return "BlackKnight";
	else if ((piece & PIECE_BIT_MASK) == QUEEN_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return "BlackQueen";
	else if ((piece & PIECE_BIT_MASK) == KING_PIECE && (piece & PIECE_COLOR_BIT_MASK) == BLACK_PIECE) return "BlackKing";
	else return NULL;
}

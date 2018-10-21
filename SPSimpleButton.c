/*
 * SimpleButton.c
 *
 */
#include <stdlib.h>
#include <stdio.h>

#include "SPSimpleButton.h"
/*this function generate a simple button, is get all the info in order to create a specific button:
 *  renderer, rectengle of sdl, image, place.
 *  the function create the button and return an reference to Widget.
 *
 */
Widget* spCreateSimpleButton(SDL_Renderer* windowRender, SDL_Rect* locationR ,const char* image ,const char* clickedImage,bool on){
	if (windowRender == NULL || locationR == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	SPSimpleButton* data = (SPSimpleButton*) malloc(sizeof(SPSimpleButton));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image);
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	if(clickedImage == NULL){
		data->clickedButtonTexture = NULL;
		data->active=true;
	}
	else{
		SDL_Surface* loadingClickedSurface = SDL_LoadBMP(clickedImage);
		SDL_Texture* clickedButtonTexture = SDL_CreateTextureFromSurface(windowRender,loadingClickedSurface);
		SDL_FreeSurface(loadingClickedSurface);
		data->clickedButtonTexture = clickedButtonTexture;
		data->active=false;
		if(clickedButtonTexture == NULL){
			free(res);
			free(data);
			SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
			SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
			return NULL ;
		}
	}
	if (res == NULL || data == NULL || loadingSurface == NULL || buttonTexture == NULL){
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	data->buttonTexture = buttonTexture;
	data->windowRenderer = windowRender;
	data->location = CopyRect(locationR);
	data->on = on;
	res->destroyWidget = spDestroySimpleButton;
	res->drawWidget = spDrawSimpleButton;
	res->handleEvent = spHandleSimpleButtonEvenet;
	res->data = data;
	return res;
}

void spDestroySimpleButton(Widget* src) {
	if (src == NULL){
		return;
	}
	SPSimpleButton* castData = (SPSimpleButton*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->buttonTexture);
	SDL_DestroyTexture(castData->clickedButtonTexture);
	free(castData);
	free(src);
}


int spHandleSimpleButtonEvenet(Widget* src, SDL_Event* event) {
	if (src == NULL || event == NULL) {
		return 0;
	}
	SPSimpleButton* castData = (SPSimpleButton*) src->data;
	if(castData->on == false){
		return 0;
	}
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			return 1;
		}
	}
	return 0;
}

void spDrawSimpleButton(Widget* src) {
	if (src == NULL){
		return;
	}
	SPSimpleButton* castData = (SPSimpleButton*) src->data;
	if(castData->on == true){
		if(castData->clickedButtonTexture == NULL){
			SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,	castData->location);
		}
		else{
			if(castData->active == true){
				SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,	castData->location);
			}
			else{
				SDL_RenderCopy(castData->windowRenderer, castData->clickedButtonTexture, NULL,	castData->location);
			}
		}

	}
}


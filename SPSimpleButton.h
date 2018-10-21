/*
 * SimpleButton.h
 *
 */

#ifndef GRAPHICS_SIMPLEBUTTON_H_
#define GRAPHICS_SIMPLEBUTTON_H_
#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>

#include "SPCommon.h"
#include "SPWidget.h"

#define WIDGETS_GAME_WINDOW_NUM 6
#define WIDGETS_SETTINGS_WINDOW_NUM 11
#define WIDGETS_LOAD_WINDOW_NUM 7
#define WIDGETS_SAVE_WINDOW_NUM 3
#define WIDGETS_MENU_WINDOW_NUM 3
#define BUTTON_HEIGTH 80
#define BUTTON_WIDTH 160
#define BUTTON_SHIFT 30


typedef struct simplebutton_t{
	SDL_Texture* buttonTexture;
	SDL_Texture* clickedButtonTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	bool active;
	bool on;
}SPSimpleButton;
/**
 *  Creates a new button for a window.
 *  @param - windowRender-the renderer for the new button,
 *  location-the location of the button(for presenting it on a window),
 *  image-the image of the new button,
 *  imagepressed-the image if the image is already pressed,
 *  show-if we would like to show the button at a window.
 *  @return
 *  An instant of the new button.
 */
Widget* spCreateSimpleButton(SDL_Renderer* windowRender, SDL_Rect* location, const char* image,const char* clickedImage ,bool on);//,const char* imagepressed
/**
 *  Destroys the button.
 *  @param-src - the button that needs to been destroyed.
 */
void spDestroySimpleButton(Widget* src);
/**
 * 	Handles all possible events for the button.
 *  @param-src - the button, event - the event that the user activated.
 *  @return
 *  1 at a legal event, else - 0.
 */
int spHandleSimpleButtonEvenet(Widget* src, SDL_Event* event);
/**
 *  Draws the button at the window.
 *  @param-src - the piece button that needs to been draw.
 */
void spDrawSimpleButton(Widget* src);


#endif /* GRAPHICS_SIMPLEBUTTON_H_ */

/*
 * Widget.h
 *
 */

#ifndef GRAPHICS_WIDGET_H_
#define GRAPHICS_WIDGET_H_
#include <SDL.h>
#include <SDL_video.h>
#include<SDL_mouse.h>
#include <stdlib.h>
typedef struct widget_t Widget;
/*
 * this struct has made in order to give the fields of widget.
 */
struct widget_t {
	void (*drawWidget)(Widget*);
	int (*handleEvent)(Widget*, SDL_Event*);
	void (*destroyWidget)(Widget*);
	void* data;
};


/*
 * the function frees all the memory have allocated with the widget.
 * the function doesn't return nothing.
*/
void destroyWidget(Widget* src);


#endif /* GRAPHICS_WIDGET_H_ */

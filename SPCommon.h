/*
 * Common.h
 *
 */

#ifndef GRAPHICS_COMMON_H_
#define GRAPHICS_COMMON_H_
#include <SDL.h>
#include <SDL_video.h>
#include <stddef.h>

/*
 * copy given rectangle to a new rectangle.
 * @param src - the source rectangle.
 * return SDL_Rect that was copyed.
 */
SDL_Rect* CopyRect(SDL_Rect* src);


#endif /* GRAPHICS_COMMON_H_ */

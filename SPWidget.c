/*
 * Widget.c
 *
 */

#include "SPWidget.h"
/*
 * the function frees all the memory have allocated with the widget.
 * the function doesn't return nothing.
 */
void destroyWidget(Widget* src) {
	if (src == NULL) {
		return;
	}
	src->destroyWidget(src);
}

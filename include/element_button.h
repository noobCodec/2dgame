#ifndef __ELEMENT__BUTTON_H__
#define __ELEMENT__BUTTON_H__

#include "elements.h"

typedef enum
{
	BE_HIDDEN,
	BE_TEXT,
	BE_ACTOR,
	BE_BOTH
}BE_Style;


typedef struct
{
	Element *label;
	Element *actor;
	BE_Style style;
	Color highColor;
	Color pressColor;
	
} ButtonElement;

void element_make_button(Element *e, ButtonElement *button);

ButtonElement *element_button_new_full(Element *label, Element *actor, Color highColor, Color pressColor);

void element_load_button_from_config(Element *e, SJson *json);

#endif

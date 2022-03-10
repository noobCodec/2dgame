#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "gf2d_sprite.h"
#include "gfc_list.h"
#include "gfc_text.h"
#include "gfc_vector.h"
#include "shape.h"
#include "gfc_color.h"
#include "simple_json.h"
typedef enum
{
	ES_IDLE,
	ES_DISABLE,
	ES_HIGHLIGHT,
	ES_HIDDEN,
	ES_ACTIVE
}ElementState;

typedef enum
{
	EUR_NONE,
	EUR_HANDLED
}ElementUpdateReturn;

typedef enum
{
	ET_LIST,
	ET_LABEL,
	ET_ACTOR,
	ET_BUTTON,
	ET_ENTRY,
	ET_PERCENT
}ElementTypes;

typedef struct Element_S
{
	int index;
	TextLine name;
	ShapeRect bounds;
	Color color;
	int state;
	int type;
	void (*draw)	(struct Element_S *element,Vector2D offset);
	List *(*update) (struct Element_S *element, Vector2D offset);
	void (*free_data) (struct Element_S *element);
	void *data;	
}Element;

Element *element_new();

Element *element_new_full(int index, TextLine name, ShapeRect bounds, Color color, int state);

void element_free(Element *element);

void element_draw(Element *element, Vector2D offset);

List *element_update(Element *element, Vector2D offset);

Element element_loaf_from_config(SJson *json);

ShapeRect element_get_absolute_bounds(Element *element, Vector2D offset);

#endif


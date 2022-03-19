#ifndef __ELEMENT_LIST_H__
#define __ELEMENT_LIST_H__
#include "gfc_list.h"
#include "elements.h"
#include "simple_json.h"
typedef enum
{
	LS_HORIZONTAL,
	LS_VERTICAL
} ListStyle;

typedef struct
{
	List *list;
	Vector2D itemSize;
	ListStyle listStyle;
	int wraps;
	int scrolls;
} ListElement;

ListElement *element_list_new_full(Vector2D itemSize, ListStyle ls,int wraps, int scrolls);

void element_list_add_item(Element *e,Element *item);

void element_make_list(Element *e, ListElement *lst);

void element_load_list_from_config(Element *e,SJson *json);

#endif

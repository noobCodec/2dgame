#include <stdlib.h>
#include <stdio.h>
#include "element_list.h"
#include "simple_logger.h"

Vector2D element_get_item_position(Element *element,int i)
{
    ListElement* list;
    Vector2D position = {0};
    int itemsPerLine;
    if (!element)return position;
    list = (ListElement*)element->data;
    if (!list)return position;
    if ((list->listStyle == LS_HORIZONTAL) && (list->wraps))
    {
        itemsPerLine = element->bounds.w / list->itemSize.x;
        position.x = element->bounds.x + ((i % itemsPerLine) * list->itemSize.x);
        position.y = element->bounds.y + ((i / itemsPerLine) * list->itemSize.y);
        return position;
    }
    if ((list->listStyle == LS_VERTICAL) && (list->wraps))
    {
        itemsPerLine = element->bounds.h / list->itemSize.y;
        position.x = element->bounds.x + ((i / itemsPerLine) * list->itemSize.x);
        position.y = element->bounds.y + ((i % itemsPerLine) * list->itemSize.y);
        return position;
    }
    if (list->listStyle == LS_HORIZONTAL)
    {
        position.x = element->bounds.x + (i * list->itemSize.x);
        position.y = element->bounds.y;
        return position;
    }
    if (list->listStyle == LS_VERTICAL)
    {
        position.x = element->bounds.x;
        position.y = element->bounds.y + (i * list->itemSize.y);
        return position;
    }
    return position;
}

void element_list_draw(Element *element,Vector2D offset)
{
    ListElement *list;
    Vector2D position;
    int count,i;
    Element *e;
    if (!element)return;
    list = (ListElement*)element->data;
    if (!list)return;
    count = gfc_list_get_count(list->list);
    for (i = 0; i < count; i++)
    {
        e = (Element *)gfc_list_get_nth(list->list,i);
        if (!e)continue;
        position = element_get_item_position(element,i);
        vector2d_add(position,position,offset);
        element_draw(e, position);
    }
}

List *element_list_update(Element *element,Vector2D offset)
{
    ListElement *list;
    Vector2D position;
    int count,i;
    Element *e;
    List *ret = NULL;
    List *updated;
    if (!element)return NULL;
    list = (ListElement*)element->data;
    if (!list)return NULL;
    vector2d_add(position,offset,element->bounds);
    count = gfc_list_get_count(list->list);
    for (i = 0; i < count; i++)
    {
        e = (Element *)gfc_list_get_nth(list->list,i);
        if (!e)continue;
        position = element_get_item_position(element,i);
        vector2d_add(position,position,offset);
        updated = element_update(e, position);
        if (updated != NULL)
        {
            if (ret == NULL)
            {
                ret = gfc_list_new();
            }
            gfc_list_concat_free(ret,updated);
        }
    }
    return ret;
}

void element_list_free(Element *element)
{
    int count,i;
    Element *e;
    ListElement *list;
    if (!element)return;
    list = (ListElement*)element->data;
    if (list != NULL)
    {
        /*for each item, free it*/
        count = gfc_list_get_count(list->list);
        for (i = 0; i < count; i++)
        {
            e = (Element *)gfc_list_get_nth(list->list,i);
            if (!e)continue;
            element_free(e);
        }
        gfc_list_delete(list->list);
        free(list);
    }
}

ListElement *element_list_new()
{
    ListElement *list;
    list = (ListElement *)malloc(sizeof(ListElement));
    if (!list)
    {
        slog("failed to allocate memory for list");
        return NULL;
    }
    memset(list,0,sizeof(ListElement));
    list->list = gfc_list_new();
    return list;
}


ListElement *element_list_new_full(Vector2D itemSize,ListStyle ls,int wraps,int scrolls)
{
    ListElement *list;
    list = element_list_new();
    if (!list)
    {
        return NULL;
    }
    vector2d_copy(list->itemSize,itemSize);
    list->listStyle = ls;
    list->wraps = wraps;
    list->scrolls = scrolls;
    return list;
}

void element_make_list(Element *e,ListElement *list)
{
    if ((!e)||(!list))return;// no op
    e->data = list;
    e->type = ET_LIST;
    e->draw = element_list_draw;
    e->update = element_list_update;
    e->free_data = element_list_free;
}

void element_list_remove_item(Element *e,Element *item)
{
    ListElement *list;
    if ((!e)||(!item))return;// no op
    list = (ListElement *)e->data;
    gfc_list_delete_data(list->list,(void*)item);
}

void element_list_add_item(Element *e,Element *item)
{
    ListElement *list;
    if ((!e)||(!item))return;// no op
    list = (ListElement *)e->data;
    gfc_list_append(list->list,(void*)item);
}

void element_load_list_from_config(Element *e,SJson *json)
{
	int x,y;
    SJson *value = NULL;
    SJson *item = NULL;
    ListElement *list = NULL;
    Vector2D vector = {0};
    ListStyle ls = 0;
    int i,count;
    const char *style = NULL;
    short int wraps = 0,scrolls = 0;
    if ((!e) || (!json))
    {
        slog("call missing parameters");
        return;
    }
        
        
    value = sj_object_get_value(json,"style");
    style = sj_get_string_value(value);
    if (style)
    {
        if (strcmp(style,"horizontal") == 0)
        {
            ls = LS_HORIZONTAL;
        }
        if (strcmp(style,"vertical") == 0)
        {
            ls = LS_VERTICAL;
        }
    }
    value = sj_object_get_value(json,"wraps");
    sj_get_bool_value(value,&wraps);
    value = sj_object_get_value(json,"scrolls");
    sj_get_bool_value(value,&scrolls);
    
    value = sj_object_get_value(json,"item_size");
    sj_get_integer_value(sj_array_get_nth(value,0),&x);
    sj_get_integer_value(sj_array_get_nth(value,1),&y);
    vector = vector2d(x,y);
    list = element_list_new_full(vector,ls,wraps,scrolls);
    element_make_list(e,list);
    
    value = sj_object_get_value(json,"elements");
    count = sj_array_get_count(value);
    for (i = 0; i < count; i++)
    {
        item = sj_array_get_nth(value,i);
        if (!item)continue;
        element_list_add_item(e,element_load_from_config(item));
    }
}
/*eol@eof*/

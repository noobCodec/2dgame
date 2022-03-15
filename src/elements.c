#include <stdlib.h>
#include <string.h>
#include "elements.h"
#include "element_actor.h"
#include "element_button.h"
#include "element_list.h"

#include "simple_logger.h"

Element *element_new()
{
    Element *e;
    e = (Element *)malloc(sizeof(Element));
    if (!e)
    {
        slog("failed to allocate a new window element");
        return NULL;
    }
    memset(e,0,sizeof(Element));
    return e;
}

Element *element_new_full(
    int      index,
    TextLine name,
    ShapeRect bounds,
    Color color,
    int state
)
{
    Element *e;
    e = element_new();
    if (!e)return NULL;
    gfc_line_cpy(e->name,name);
    e->index = index;
    e->color = color;
    e->state = state;
    e->bounds = bounds;
    return e;
}

void element_free(Element *e)
{
    if (!e)return;
    if (e->free_data)
    {
        e->free_data(e);
    }
    free(e);
}

void element_draw(Element *e, Vector2D offset)
{
   // Rect rect;
    if (!e)
    {
        return;
    }
    if (e->draw)e->draw(e,offset);
//    gf2d_rect_set(rect,offset.x + e->bounds.x,offset.y + e->bounds.y,e->bounds.w,e->bounds.h);
 //   gf2d_rect_draw(rect,gf2d_color8(100,255,100,255));
}

List * element_update(Element *e, Vector2D offset)
{
    if (!e)
    {
        return NULL;
    }
    if (e->draw)return e->update(e,offset);
    return NULL;
}

Element *element_load_from_config(SJson *json)
{
	int x,y,z,k;
	SJson *tmp;
    Element *e = NULL;
    SJson *value;
    const char *type;
    Vector4D vector;
    if (!sj_is_object(json))return NULL;
    e = element_new();
    if (!e)return NULL;
    value = sj_object_get_value(json,"name");
    gfc_line_cpy(e->name,sj_get_string_value(value));
    
    value = sj_object_get_value(json,"id");
    sj_get_integer_value(value,&e->index);

    value = sj_object_get_value(json,"state");
    sj_get_integer_value(value,&e->index);

    tmp = sj_object_get_value(json,"color");
    vector4d_set(vector,255,255,255,255);
    sj_get_integer_value(sj_array_get_nth(tmp,0),&x);
    sj_get_integer_value(sj_array_get_nth(tmp,1),&y);
    sj_get_integer_value(sj_array_get_nth(tmp,2),&z);
    sj_get_integer_value(sj_array_get_nth(tmp,3),&k);
    vector = vector4d(x,y,z,k);
    e->color = gfc_color_from_vector4(vector);
        
    tmp = sj_object_get_value(json,"bounds");
    sj_get_integer_value(sj_array_get_nth(tmp,0),&x);
    sj_get_integer_value(sj_array_get_nth(tmp,1),&y);
    sj_get_integer_value(sj_array_get_nth(tmp,2),&z);
    sj_get_integer_value(sj_array_get_nth(tmp,3),&k);
    vector = vector4d(x,y,z,k);
    gfc_rect_set(e->bounds,vector.x,vector.y,vector.z,vector.w);
    
    value = sj_object_get_value(json,"type");
    type = sj_get_string_value(value);
    if (strcmp(type,"list") == 0)
    {
        element_load_list_from_config(e,json);
    }
    else if (strcmp(type,"label") == 0)
    {
        element_load_label_from_config(e,json);
    }
    else if (strcmp(type,"actor") == 0)
    {
        element_load_actor_from_config(e,json);
    }
    else if (strcmp(type,"button") == 0)
    {
        element_load_button_from_config(e,json);
    }
    return e;
}

ShapeRect element_get_absolute_bounds(Element *element,Vector2D offset)
{
    ShapeRect r = {0};
    if (!element)return r;
    r.x = element->bounds.x + offset.x;
    r.y = element->bounds.y + offset.y;
    r.w = element->bounds.w;
    r.h = element->bounds.h;
    return r;
}

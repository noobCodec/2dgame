#include "element_actor.h"
#include "element_button.h"
#include "mouse.h"
#include "simple_logger.h"
#include "gfc_list.h"

void element_button_draw(Element *element,Vector2D offset)
{
    ButtonElement *button;
    Vector2D position;
    Actor *actor;
    if (!element)return;
    button = (ButtonElement*)element->data;
    if (!button)return;
    vector2d_add(position,offset,element->bounds);
    actor = element_actor_get_actor(button->actor);
    switch(element->state)
    {
        case ES_HIDDEN:
        case ES_DISABLE:
            return;
        case ES_IDLE:
            gem_actor_set_action(actor,"idle");
            break;
        case ES_HIGHLIGHT:
            gem_actor_set_action(actor,"high");
            break;
        case ES_ACTIVE:
            gem_actor_set_action(actor,"press");
            break;
    }
    element_draw(button->actor,position);
    element_draw(button->label,position);
}

List *element_button_update(Element *element,Vector2D offset)
{
	
    Actor *actor;
    ShapeRect bounds;
    List *list;
    ButtonElement *button;
    if (!element)return NULL;
    button = (ButtonElement*)element->data;
    if (!button)return NULL;
    actor = element_actor_get_actor(button->actor);
    gem_actor_next_frame(actor);
    bounds = element_get_absolute_bounds(element,offset);
    if(mouse_in_rect(bounds))
    {
        element->state = ES_HIGHLIGHT;
        if (mouse_button_state(0))
        {
        	
            element->state = ES_ACTIVE;
        }
        else if (mouse_button_released(0))
        {
            list = gfc_list_new();
            gfc_list_append(list,element);
            return list;
        }
    }
    else
    {
        element->state = ES_IDLE;
    }

    return NULL;
}

void element_button_free(Element *element)
{
    ButtonElement *button;
    if (!element)return;
    button = (ButtonElement*)element->data;
    if (button != NULL)
    {
        element_free(button->label);
        element_free(button->actor);
        free(button);
    }
}

ButtonElement *element_button_new()
{
    ButtonElement *button;
    button = (ButtonElement *)malloc(sizeof(ButtonElement));
    if (!button)
    {
        slog("failed to allocate memory for button");
        return NULL;
    }
    memset(button,0,sizeof(ButtonElement));
    return button;
}


void element_make_button(Element *e,ButtonElement *button)
{
    if (!e)return;
    e->data = button;
    e->type = ET_BUTTON;
    e->draw = element_button_draw;
    e->update = element_button_update;
    e->free_data = element_button_free;
}

ButtonElement *element_button_new_full(Element *label,Element *actor,Color highColor,Color pressColor)
{
    ButtonElement *button;
    button = element_button_new();
    if (!button)return NULL;
    button->label = label;
    button->actor = actor;
    button->highColor = highColor;
    button->pressColor = pressColor;
    return button;
}

void element_load_button_from_config(Element *e,SJson *json)
{
	int x,y,z,k;
	SJson *tmp = NULL;
    Vector4D highColor = {255,255,255,255},pressColor = {255,255,255,255};
    Element *label = NULL;
    Element *actor = NULL;
    SJson *value;
    
    if ((!e) || (!json))
    {
        slog("call missing parameters");
        return;
    }
    
    tmp = sj_object_get_value(json,"highColor");
    sj_get_integer_value(sj_array_get_nth(tmp,0),&x);
    sj_get_integer_value(sj_array_get_nth(tmp,1),&y);
    sj_get_integer_value(sj_array_get_nth(tmp,2),&z);
    sj_get_integer_value(sj_array_get_nth(tmp,3),&k);
    highColor = vector4d(x,y,z,k);
	tmp = sj_object_get_value(json,"pressColor");
    sj_get_integer_value(sj_array_get_nth(tmp,0),&x);
    sj_get_integer_value(sj_array_get_nth(tmp,1),&y);
    sj_get_integer_value(sj_array_get_nth(tmp,2),&z);
    sj_get_integer_value(sj_array_get_nth(tmp,3),&k);
    pressColor = vector4d(x,y,z,k);


    value = sj_object_get_value(json,"label");
    if (value)
    {
        label = element_load_from_config(value);
    }
    value = sj_object_get_value(json,"actor");
    if (value)
    {
        actor = element_load_from_config(value);
    }
    element_make_button(e,element_button_new_full(label,actor,gfc_color_from_vector4(highColor),gfc_color_from_vector4(pressColor)));
}


/*eol@eof*/

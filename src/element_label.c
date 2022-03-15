#include <stdlib.h>
#include <stdio.h>
#include "element_label.h"
#include "simple_logger.h"
#include "font.h"

void element_label_draw(Element *element,Vector2D offset)
{
    LabelElement *label;
    Vector2D position;
    Font *char_font = font_load("images/text.ttf",32);
    Vector2D size = {0};
    if (!element)return;
    label = (LabelElement*)element->data;
    if (!label)return;
    //TODO: FIX THIS
    //size = gfc_text_get_bounds(label->text,label->style);
    size = vector2d(5,5);
    if (size.x < 0)
    {
        return;
    }
    // adjust position to top left
    vector2d_add(position,offset,element->bounds);
    switch(label->justify)
    {
        case LJ_Left:
            break;
        case LJ_Center:
            position.x += (element->bounds.w - size.x)/2 ;
            break;
        case LJ_Right:
            position.x += (element->bounds.w - size.x);
            break;
    }
    switch(label->alignment)
    {
        case LA_Top:
            break;
        case LA_Middle:
            position.y += (element->bounds.h - size.y)/2 ;
            break;
        case LA_Bottom:
            position.y += (element->bounds.h - size.y);
            break;
    }
    font_render(char_font,label->text,position,element->color);
}

List *element_label_update(Element *element,Vector2D offset)
{
    return NULL;
}

void element_label_free(Element *element)
{
    LabelElement *label;
    if (!element)return;
    label = (LabelElement*)element->data;
    if (label != NULL)
    {
        free(label);
    }
}

LabelElement *element_label_new()
{
    LabelElement *label;
    label = (LabelElement *)malloc(sizeof(LabelElement));
    if (!label)
    {
        slog("failed to allocate memory for label");
        return NULL;
    }
    memset(label,0,sizeof(LabelElement));
    return label;
}


LabelElement *element_label_new_full(char *text,Color color,int style,int justify,int align)
{
    LabelElement *label;
    label = element_label_new();
    if (!label)
    {
        return NULL;
    }
    gfc_block_cpy(label->text,text);
    label->bgcolor = color;
    label->style = style;
    label->justify = justify;
    label->alignment = align;
    return label;
}

void element_make_label(Element *e,LabelElement *label)
{
    if (!e)return;
    e->data = label;
    e->type = ET_LABEL;
    e->draw = element_label_draw;
    e->update = element_label_update;
    e->free_data = element_label_free;
}

void element_load_label_from_config(Element *e,SJson *json)
{
	int x,y,z,k;
    SJson *value;
    Vector4D vector;
    Color color;
    const char *buffer;
    int style = FT_Normal;
    int justify = LJ_Left;  
    int align = LA_Top;
    if ((!e) || (!json))
    {
        slog("call missing parameters");
        return;
    }
    value = sj_object_get_value(json,"style");
    buffer = sj_get_string_value(value);
    if (buffer)
    {
        if (strcmp(buffer,"normal") == 0)
        {
            style = FT_Normal;
        }
        else if (strcmp(buffer,"small") == 0)
        {
            style = FT_Small;
        }
        else if (strcmp(buffer,"H1") == 0)
        {
            style = FT_H1;
        }
        else if (strcmp(buffer,"H2") == 0)
        {
            style = FT_H2;
        }
        else if (strcmp(buffer,"H3") == 0)
        {
            style = FT_H3;
        }
        else if (strcmp(buffer,"H4") == 0)
        {
            style = FT_H4;
        }
        else if (strcmp(buffer,"H5") == 0)
        {
            style = FT_H5;
        }
        else if (strcmp(buffer,"H6") == 0)
        {
        style = FT_H6;
        }
    }

    value = sj_object_get_value(json,"justify");
    buffer = sj_get_string_value(value);
    if (buffer)
    {
        if (strcmp(buffer,"left") == 0)
        {
            justify = LJ_Left;
        }
        else if (strcmp(buffer,"center") == 0)
        {
            justify = LJ_Center;
        }
        else if (strcmp(buffer,"right") == 0)
        {
            justify = LJ_Right;
        }
    }

    value = sj_object_get_value(json,"align");
    buffer = sj_get_string_value(value);
    if (buffer)
    {
        if (strcmp(buffer,"top") == 0)
        {
            align = LA_Top;
        }
        else if (strcmp(buffer,"middle") == 0)
        {
            align = LA_Middle;
        }
        else if (strcmp(buffer,"bottom") == 0)
        {
            align = LA_Bottom;
        }
    }
    value = sj_object_get_value(json,"color");
    sj_get_integer_value(sj_array_get_nth(value,0),&x);
    sj_get_integer_value(sj_array_get_nth(value,1),&y);
    sj_get_integer_value(sj_array_get_nth(value,2),&z);
    sj_get_integer_value(sj_array_get_nth(value,3),&k);
    vector4d_set(vector,255,255,255,255);
    vector = vector4d(x,y,z,k);
    color = gfc_color_from_vector4(vector);

    value = sj_object_get_value(json,"text");
    buffer = sj_get_string_value(value);
    element_make_label(e,element_label_new_full((char *)buffer,color,style,justify,align));
}


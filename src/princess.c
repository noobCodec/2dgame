#include "princess.h"
#include "warrior.h"
#include "element_button.h"
#include <SDL.h>
#include "simple_logger.h"

static int access_code = 0;
void princess_label(Element *e, char *text)
{
    if(!e) return;
    LabelElement *j = ((LabelElement*)((ButtonElement*)e->data)->label->data);
    strncpy(j->text,text,strlen(j->text));


}
static int updates = 0;
static int choice = 0;
int princess_update(Window *win,List *updateList)
{
    int i,count;
    Element *e;
    Element *j;
    LabelElement *title;
    LabelElement *princess;
    Element *option1,*option2;
    Element *other_option;

    if (!win)return 0;
    if (!updateList)return 0;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList,i);
        for (int k = 0; k < gfc_list_get_count(win->elements);k++)
        {
            j = gfc_list_get_nth(win->elements,0);
            List *tmp = ((ListElement *) j->data)->list;
            title = ((Element*)gfc_list_get_nth(tmp,0))->data;
            princess = ((Element*)gfc_list_get_nth(tmp,2))->data;
            j = gfc_list_get_nth(tmp,4);
            tmp = ((ListElement *) j->data)->list;
            option1 = ((Element *)gfc_list_get_nth(tmp,0));
            option2 = ((Element *)gfc_list_get_nth(tmp,1));
        }
        slog("%s",title->text);
        switch(updates)
        {
            case 0:
            switch(e->index)
            {
                //No PROBLEM
                case 61:
                    strncpy(princess->text,"Should I Join Party?",strlen(princess->text));
                    re_label(option2,"YES");
                    re_label(e,"NO");
                    choice = 0;
                    break;
                //DIE
                case 62:
                    strncpy(princess->text,"NOOOOOOOOOOO",strlen(princess->text));
                    re_label(option1,"EXIT");
                    re_label(e,"EXIT");
                    choice = 1;
                    break;
            }
            break;
            case 1:
            if(choice == 0)
            {
            switch(e->index)
            {
                case 61:
                    window_free(win);
                    access_code = 1;
                    return 1;
                    break;
                case 62:
                    re_label(e,"NO");
                    break;
            }
            }
            else
            {
                window_free(win);
                access_code = 1;
                return 1;

            }
            break;
        }
        updates++;
    }
    return 0;
}
void princess_think(Entity *self)
{

    Entity *guard = gfc_list_get_nth(self->guards,0);
    if(guard->_inuse==0 && self->blocked == 0)
    {
        SJson *sj = sj_load("json/dialogue/tmp.json");
        if(!sj)
        {
            entity_free(self);
            return;
        }
        Window *window = window_load_from_json(sj);
        if(!window)
        {
            entity_free(self);
            return;
        }
        window->update = princess_update;
        window->active = 1;
        self->win = window;
        self->blocked = 1;
    }
    if(access_code == 1)
    {
        entity_free(self);
    }
}


Entity *princess_ent_new(Vector2D position)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }
    //ent->elapsed = SDL_GetTicks();
    ent->old_health = 1000;
    ent->team = 244;
    ent->sprite = gf2d_sprite_load_image("images/princess.png");
    //ent->think = crystal_think;
    //ent->range = shape_circle(position.x,position.y,fire_range);
    ent->draw_scale = vector2d(0.08,0.08);
    ent->velocity = vector2d(0,0);
    ent->immune = 1;
    ent->think = princess_think;
    List *guards = gfc_list_new();
    Entity *bodyguard = warrior_ent_new(vector2d(position.x+20,position.y),30);
    bodyguard->team = 244;
    gfc_list_append(guards,bodyguard);
    ent->guards = guards;
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->damage = 10;
    ent->health = 1000;
    ent->path = NULL;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x-16,position.y-16,32,32));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}


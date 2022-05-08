 
#include "shirt.h"
#include "ranger.h"
#include "element_button.h"
#include "game_instance.h"
#include <SDL.h>
#include "simple_logger.h"

static int access_code = 0;
static int isdead = 0;

int is_dead(){return isdead;}
void shirt_label(Element *e, char *text)
{
    if(!e) return;
    LabelElement *j = ((LabelElement*)((ButtonElement*)e->data)->label->data);
    strncpy(j->text,text,strlen(j->text));


}
int shirt_update(Window *win,List *updateList)
{
    int i,count;
        Element *e;
        ButtonElement *here;
        LabelElement *j;
        if (!win)return 0;
        if (!updateList)return 0;
        count = gfc_list_get_count(updateList);
        for (i = 0; i < count; i++)
        {
            e = gfc_list_get_nth(updateList,i);
            switch(e->index)
            {
                case 61:
                    access_code = 1;
                    window_free(win);
                    break;
            }
        }
}
void shirt_think(Entity *self)
{

    Entity *guard = gfc_list_get_nth(self->guards,0);
    if(guard->_inuse==0 && self->blocked == 0)
    {
        SJson *sj = sj_load("json/dialogue/dialogue2.json");
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
        window->update = shirt_update;
        window->active = 1;
        self->win = window;
        self->blocked = 1;
    }
    if(access_code == 1)
    {
        isdead = 1;
        get_game(0)->resources += 100;
        entity_free(self);
    }
}


Entity *shirt_ent_new(Vector2D position)
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
    ent->team = 255;
    ent->id = 9;
    ent->sprite = gf2d_sprite_load_image("images/shirt.png");
    //ent->think = crystal_think;
    //ent->range = shape_circle(position.x,position.y,fire_range);
    ent->draw_scale = vector2d(0.08,0.08);
    ent->velocity = vector2d(0,0);
    ent->immune = 1;
    ent->think = shirt_think;
    List *guards = gfc_list_new();
    Entity *bodyguard = ranger_ent_new(vector2d(position.x,position.y),30);
    bodyguard->team = 255;
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


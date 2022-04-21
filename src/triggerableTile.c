#include <stdio.h>
#include <string.h>
#include "entity.h"
#include "simple_logger.h"
#include "gfc_text.h"
#include <time.h>
#include <sys/stat.h>
static time_t modified = 0;

int trigger_update(Window *win,List *updateList)
{
    int i,count;
    Element *e;
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
            	win->active = 0;
                win->dead = 1;
            	break;
        }
    }
    return 0;
}

void trigger_ent_think(Entity *self)
{
    struct stat file_stat;
    stat("json/menus/building_menu.json", &file_stat);

    if(file_stat.st_mtime > modified)
    {
        modified = file_stat.st_mtime;
        window_free(self->win);
        SJson *sj = sj_load("json/menus/building_menu.json");
        Window *window = window_load_from_json(sj);
        window->update = trigger_update;
        sj_free(sj);
        self->win = window;
    }
    gf2d_draw_rect(shape_rect_to_sdl_rect(self->bounding),vector4d(0,255,0,255));
    List *touched = touch(self);
    if(gfc_list_get_count(touched) && !self->win->dead)
    {
     self->win->active = 1;
     return;
    }

}
Entity *trigger_ent_new(Vector2D position,char *text)
{
    struct stat file_stat;
    stat("json/menus/building_menu.json", &file_stat);
    modified = file_stat.st_mtime;
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for triggers");
        return NULL;
    }
    SJson *sj = sj_load(text);
    if(!sj)
    {
     entity_free(ent);
     return NULL;
    }
    Window *window = window_load_from_json(sj);
    if(!window)
    {
        entity_free(ent);
        return NULL;
    }
    window->update = trigger_update;
    ent->draw_scale = vector2d(1,1);
    ent->velocity = vector2d(0,0);
    ent->draw_offset.x = 0;
    ent->draw_offset.y = -16;
    ent->rotation.x = 16;
    ent->rotation.y = 16;
    ent->team = 0;
    ent->harvest = 1;
    ent->damage = 0;
    ent->flip.x = 0;
    ent->flip.y = 0;
    ent->blocked = 0;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x-8,position.y-8,32,32));
    ent->enemy = NULL;
    ent->think = trigger_ent_think;
    ent->win = window;
    vector2d_copy(ent->position,position);
    return ent;

}


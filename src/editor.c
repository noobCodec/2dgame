#ifndef __EDITOR_H__
#define __EDITOR_H__
#include "editor.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "windows.h"
#include "element_button.h"
#include "game_ops.h"
#include "simple_json.h"
#include "simple_logger.h"
#include "mouse.h"
#include "gfc_vector.h"
#include "shape.h"
#include "tile_map.h"
#include "hud.h"
#include "path_map.h"
#include "entity.h"
#include "rogue.h"
#include "mage_ent.h"
#include "ranger.h"
#include "warrior.h"
#include "goblin.h"
#include "princess.h"
#include "crystal.h"
#include "building.h"
#include <string.h>
#include <stdlib.h>
static int finished = 0;
static int ent_menu = 0;
static int return_code = 0;
static int entity_type = 0;
static Window *tiler = NULL;
static Window *other = NULL;
static char text[255] = {0};
static int tiler_weight = 1;
static int tile_type = 1;
static int edit_mode = 0;
static int ttit = 1;
static char *composition;
static int cursor;
static int selection_len;
void append(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}
void remove_char(char* s) {
        int len = strlen(s);
        if(len>0)
        s[len-1] = '\0';
}
int text_input_update(Window *win,List *updateList)
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
                tile_type = atoi(text);
                window_free(win);
                other = NULL;
        }
    }
    return 1;
}
int path_input_update(Window *win,List *updateList)
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
                tiler_weight = atoi(text);
                window_free(win);
                other = NULL;
        }
    }
    return 1;
}



int tiler_update(Window *win,List *updateList)
{
    int i,count;
    Element *e;
    ButtonElement *here;
    LabelElement *j;
    Window *window;
    SJson *sj;
    if (!win)return 0;
    if (!updateList)return 0;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList,i);
        switch(e->index)
        {
            case 65:
                win->active = 0;
                window_free(win);
                break;
            case 61:
                sj = sj_load("json/editor/editortext.json");
                if(!sj)return 1;
                window = window_load_from_json(sj);
                window->update = text_input_update;
                window->active = 1;
                other = window;
                win->active = 0;
                window_free(win);
                tiler = NULL;
                return_code = 1;
                break;
            case 62:
                sj = sj_load("json/editor/editortext.json");
                if(!sj)return 1;
                window = window_load_from_json(sj);
                window->update = path_input_update;
                window->active = 1;
                other = window;
                win->active = 0;
                window_free(win);
                tiler = NULL;
                return_code = 1;
                break;
        }
    }
    return 1;
}

int entity_menu_update(Window *win,List *updateList)
{
    int i,count;
    Element *e;
    ButtonElement *here;
    LabelElement *j;
    Window *window;
    SJson *sj;
    if (!win)return 0;
    if (!updateList)return 0;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList,i);
        switch(e->index)
        {
            case 40:
                ent_menu = 0;
                entity_type = 0;
                win->active = 0;
                window_free(win);
                break;
            case 41:
                ent_menu = 0;
                entity_type = 1;
                win->active = 0;
                window_free(win);
                break;
            case 42:
                ent_menu = 0;
                entity_type = 2;
                win->active = 0;
                window_free(win);
                break;
            case 43:
                ent_menu = 0;
                entity_type = 3;
                win->active = 0;
                window_free(win);
                break;
            case 44:
                ent_menu = 0;
                entity_type = 4;
                win->active = 0;
                window_free(win);
                break;
            case 45:
                ent_menu = 0;
                entity_type = 5;
                win->active = 0;
                window_free(win);
                break;
            case 46:
                ent_menu = 0;
                entity_type = 6;
                win->active = 0;
                window_free(win);
                break;
            case 47:
                ent_menu = 0;
                entity_type = 7;
                win->active = 0;
                window_free(win);
                break;
            case 48:
                ent_menu = 0;
                entity_type = 8;
                win->active = 0;
                window_free(win);
                break;
        }
    }
    return 1;
}
void modifyPath(int i, int j, int tile)
{
    SJson *file = sj_load("level/xd.json");
    if(file == NULL) return;
    SJson *new = sj_new();
    SJson *new_arr = sj_array_new();
    SJson *arr = sj_object_get_value(file,"tile_map");
    for(int iter = 0 ; iter < sj_array_get_count(arr); iter++)
    {
        if(iter==i)
        {
            SJson *tmp = sj_array_get_nth(arr,i);
            SJson *tmp_arr = sj_array_new();
            for(int k = 0; k<sj_array_get_count(tmp); k++)
            {
                if(k==j)
                {
                 sj_array_append(tmp_arr,sj_new_int(tile));
                }
                else
                {
                 sj_array_append(tmp_arr,sj_copy(sj_array_get_nth(tmp,k)));
                }
            }
            sj_array_append(new_arr,tmp_arr);
        }
        else
        {
            sj_array_append(new_arr,sj_copy(sj_array_get_nth(arr,iter)));
        }
    }
    sj_object_delete_key(file,"tile_map");
    sj_object_insert(file,"tile_map",new_arr);
    sj_save(file, "level/xd.json");
    return;

}
void modifyTile(SJson *file, int i, int j, int tile)
{
    if(file == NULL) return;
    SJson *new = sj_new();
    SJson *new_arr = sj_array_new();
    SJson *arr = sj_object_get_value(file,"tile_map");
    for(int iter = 0 ; iter < sj_array_get_count(arr); iter++)
    {
        if(iter==j)
        {
            SJson *tmp = sj_array_get_nth(arr,j);
            SJson *tmp_arr = sj_array_new();
            for(int k = 0; k<sj_array_get_count(tmp); k++)
            {
                if(k==i)
                {
                 sj_array_append(tmp_arr,sj_new_int(tile));
                }
                else
                {
                 sj_array_append(tmp_arr,sj_copy(sj_array_get_nth(tmp,k)));
                }
            }
            sj_array_append(new_arr,tmp_arr);
        }
        else
        {
            sj_array_append(new_arr,sj_copy(sj_array_get_nth(arr,iter)));
        }
    }
    sj_object_delete_key(file,"tile_map");
    sj_object_insert(file,"tile_map",new_arr);
    sj_save(file, "level/test.json");
}
void custom_input_handler(TileMap **t,Path_Map **m,SJson *local)
{
    int adjx,adjy;
    if(return_code==1)
    {
        memset(text,0,255);
        int done = 0;
        SDL_StartTextInput();
        while (!done) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type)
                {
                     case SDL_KEYDOWN:
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_RETURN:
                                done = 1;
                                break;
                            case SDLK_BACKSPACE:
                                remove_char(text);
                                break;
                        }
                    case SDL_TEXTINPUT:
                        for(int i=0; i<SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i)
                            {
                                char c = event.text.text[i];
                                // cancel if a non-ascii char is encountered
                                if(c < ' ' || c > '~')
                                    break;

                                append(text,c);
                            }
                        break;
                    case SDL_TEXTEDITING:
                        composition = event.edit.text;
                        cursor = event.edit.start;
                        selection_len = event.edit.length;
                        break;
                }
            }
        SDL_PumpEvents();
        gf2d_graphics_clear_screen();
        tilemap_draw(*t);
        mouse_update();
        window_update(other);
        window_draw(other);
        mouse_draw();
        gf2d_grahics_next_frame();
        Element *j;
        LabelElement *title;
        if(other == NULL)break;
        for (int k = 0; k < gfc_list_get_count(other->elements);k++)
        {
            j = gfc_list_get_nth(other->elements,0);
            List *tmp = ((ListElement *) j->data)->list;
            title = ((Element*)gfc_list_get_nth(tmp,0))->data;
        }
        memset(title->text,0,strlen(title->text));
        strncpy(title->text,text,strlen(text));
        }
        SDL_StopTextInput();
        return_code = 0;
    }
    if(mouse_button_held(0) && (tiler == NULL || tiler->active == 0))
    {
        Vector2D pos = mouse_get_position();
        adjx = ((int)pos.x) / 60;
        adjy = ((int)pos.y) / 60;
        modifyTile(local,adjx,adjy,tile_type);
        modifyPath(adjx,adjy,tiler_weight);
        Path_Map_free(*m);
        *m = Path_Map_load("level/xd.json");
        tilemap_free(*t);
        *t = tilemap_load("level/test.json");
        gf2d_draw_rect(shape_rect_to_sdl_rect(shape_rect_from_vector4d(vector4d(adjx*60,adjy*60,60,60))),vector4d(0,0,255,255));
    }
    if(mouse_button_pressed(2))
    {
        if(tiler)
            window_free(tiler);
        Vector2D pos = mouse_get_position();
        SJson *sj = sj_load("json/editor/tileeditor.json");
        if(!sj)return;
        Window *window = window_load_from_json(sj);
        window->update = tiler_update;
        if(pos.x < 600)
            window->dimensions.x = pos.x + 50;
        else
            window->dimensions.x = pos.x - 550;
        if(pos.y < 360)
            window->dimensions.y = pos.y + 20;
        else
            window->dimensions.y = pos.y - 50 ;
        window->active = 1;
        tiler = window;
    }
    return;
}
void entity_input_handler()
{
    if(mouse_button_pressed(2))
    {
        SJson *sj = sj_load("json/editor/entityeditor.json");
        if(!sj)return;
        ent_menu = 1;
        Window *window = window_load_from_json(sj);
        window->update = entity_menu_update;
        window->active = 1;
    }
    if(mouse_button_pressed(0) && ent_menu == 0)
    {
        Vector2D pos = mouse_get_position();
        switch(entity_type)
        {
            case 0:
                goblin_ent_new(pos,10);
                break;
            case 1:
                ranger_ent_new(pos,50);
                break;
            case 2:
                warrior_ent_new(pos,20);
                break;
            case 3:
                mage_ent_new(pos,40);
                break;
            case 4:
                rogue_ent_new(pos,20);
                break;
            case 5:
                shirt_ent_new(pos);
                break;
            case 6:
                princess_ent_new(pos);
                break;
            case 7:
                crystal_ent_new(pos);
                break;
            case 8:
                building_ent_new(pos);
                break;
        }
    }
}
void re_label_editor(Element *e, char *text)
{
    if(!e) return;
    LabelElement *j = ((LabelElement*)((ButtonElement*)e->data)->label->data);
    strncpy(j->text,text,strlen(j->text));


}
SJson *empty_arr(int x, int y, char *out)
{
    SJson *ptr = sj_object_new();
    sj_object_insert(ptr,"tileset",sj_new_str("images/tileset_3.png"));
    sj_object_insert(ptr,"tile_width",sj_new_int(60));
    sj_object_insert(ptr,"tile_height",sj_new_int(60));
    sj_object_insert(ptr,"tile_count",sj_new_int(6));
    SJson *str = sj_array_new();
    for(int i = 0; i < y; i++)
    {
        SJson *tmp = sj_array_new();
        for(int k = 0; k < y; k++)
            sj_array_append(tmp,sj_new_int(1));
        sj_array_append(str,tmp);
    }
    sj_object_insert(ptr,"tile_map",str);
    sj_save(ptr,out);
    return ptr;
}
int editor_update(Window *win,List *updateList)
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
                edit_mode = 1;
                win->active = 0;
                break;
            case 62:
                edit_mode = 2;
                win->active = 0;
                break;
            case 63:
                save_game_neutral();
                finished = 1;
                win->active = 0;
                window_free(win);
                break;
        }
    }
    return 1;
}
void edit_game()
{
    const Uint8 * keys;
    SJson *sj = sj_load("json/editor/leveleditor.json");
    if(!sj)return;
    Window *window = window_load_from_json(sj);
    window->update = editor_update;
    SJson *local = sj_load("level/test.json");
    TileMap *t = tilemap_load("level/test.json");
    Path_Map *m = Path_Map_load("level/xd.json");
    while(!finished)
    {
        SDL_PumpEvents();
        gf2d_graphics_clear_screen();
        keys = SDL_GetKeyboardState(NULL);
        mouse_update();
        windows_update_all();
        tilemap_draw(t);
        entity_manager_think_all();
        entity_manager_draw_all();
        windows_draw_all();
//         tilemap_free(t);
//         t = tilemap_load("level/test.json");
        if(!window->active && edit_mode == 2)
        {
            entity_input_handler();
        }
        if(!window->active && edit_mode == 1)
            custom_input_handler(&t,&m,local);
        mouse_draw();
        gf2d_grahics_next_frame();
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            window->active = 1;
            apply_block(1);
        }
        if (keys[SDL_SCANCODE_F1])
        {
            window->active = 0;
            apply_block(1);
        }
    }
    if(tiler)
        window_free(tiler);
    finished = 0;
    tilemap_free(t);
}




#endif

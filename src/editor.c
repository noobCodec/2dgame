#ifndef __EDITOR_H__
#define __EDITOR_H__
#include "editor.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "windows.h"
#include "element_button.h"
#include "simple_json.h"
#include "simple_logger.h"
#include "mouse.h"
#include "gfc_vector.h"
#include "shape.h"
#include "tile_map.h"
#include "hud.h"
#include "entity.h"
#include <string.h>
static int finished = 0;
static int return_code = 0;
static Window *tiler = NULL;
static Window *other = NULL;
static char text[255] = {0};
static char *composition;
static int cursor;
static int selection_len;
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
                window_free(win);
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
                Window *window = window_load_from_json(sj);
                window->update = text_input_update;
                window->active = 1;
                other = window;
                win->active = 0;
                window_free(win);
                tiler = NULL;
                return_code = 1;
        }
    }
    return 1;
}
void custom_input_handler()
{
    int adjx,adjy;
    if(mouse_button_pressed(2))
    {
        if(tiler)
            window_free(tiler);
        Vector2D pos = mouse_get_position();
        adjx = (int)pos.x % 60;
        adjy = (int)pos.y % 60;
        slog("%f:%f\n",pos.x,pos.y);
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
    if(return_code==1)
    {
        int done = 0;
        SDL_StartTextInput();
        while (!done) {
            SDL_Event event;
            if (SDL_PollEvent(&event)) {
                switch (event.type)
                {
                     case SDL_KEYDOWN:
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_RETURN:
                                done = 1;
                                break;
                        }
                    case SDL_TEXTINPUT:
                        /* Add new text onto the end of our text */
                        strcat(text, event.text.text);
                        break;
                    case SDL_TEXTEDITING:
                        /*
                        Update the composition text.
                        Update the cursor position.
                        Update the selection length (if any).
                        */
                        composition = event.edit.text;
                        cursor = event.edit.start;
                        selection_len = event.edit.length;
                        break;
                }
            }
        SDL_PumpEvents();
        gf2d_graphics_clear_screen();
        window_update(other);
        window_draw(other);
        gf2d_grahics_next_frame();
        Element *j;
        LabelElement *title;
        for (int k = 0; k < gfc_list_get_count(other->elements);k++)
        {
            j = gfc_list_get_nth(other->elements,0);
            List *tmp = ((ListElement *) j->data)->list;
            title = ((Element*)gfc_list_get_nth(tmp,0))->data;
        }
        strncpy(title->text,text,strlen(text));
        puts(title->text);
        }

        return_code = 0;
    }
    return;
// 	while(SDL_PollEvent(&event))
}
void re_label_editor(Element *e, char *text)
{
    if(!e) return;
    LabelElement *j = ((LabelElement*)((ButtonElement*)e->data)->label->data);
    strncpy(j->text,text,strlen(j->text));


}
void empty_arr(int x, int y, char *out)
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
    sj_save(ptr,"level/test.json");
    return;


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
    empty_arr(12,20,"level/test.json");
    TileMap *t = tilemap_load("level/test.json");
    while(!finished)
    {
        SDL_PumpEvents();
        gf2d_graphics_clear_screen();
        keys = SDL_GetKeyboardState(NULL);
        mouse_update();
        custom_input_handler();
        windows_update_all();
        tilemap_draw(t);
        entity_manager_think_all();
        entity_manager_draw_all();
        windows_draw_all();
//         SDL_Delay(200);

        mouse_draw();
        gf2d_grahics_next_frame();
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            window->active = 1;
            apply_block(1);
        }
    }
    if(tiler)
        window_free(tiler);
    finished = 0;
    tilemap_free(t);
}




#endif

#include <SDL.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include <stdio.h>
#include "game_instance.h"
#include "entity.h"
#include "bug_ent.h"
#include "tile_map.h"
#include "algo.h"
#include "gfc_list.h"
#include "path_map.h"
#include "building.h"
#include "camera.h"
#include "input.h"
#include "font.h"
#include "mage_ent.h"
#include "crystal.h"
#include "actor.h"
#include "warrior.h"
#include "triggerableTile.h"
#include "mouse.h"
#include "opponent.h"
#include "zone.h"
#include "load_obstacles.h"
#include "simple_json.h"
#include "elements.h"
#include "windows.h"
#include "game_ops.h"
#include "gfc_audio.h"
#include "element_button.h"
#include "hud.h"
#include "princess.h"
#include "editor.h"
#include "shirt.h"
static int enabled = 0;
static int load_map = 0;
static int done = 0;
static int dirty = 0;
static Sound *k = NULL;
static Sound *m = NULL;
void restart_game()
{
    game_manager_clear();
	entity_manager_clear();
    game_instance *p1 = game_new(0);
    game_instance *p2 = game_new(1);
    opponent_init(p1,p2);
}
void welcome_menu_update(Window *win,List *updateList)
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
        slog("UPDATE");
        e = gfc_list_get_nth(updateList,i);
        switch(e->index)
        {
            case 61:
            	window_free(win);
                break;
        }
    }



}
Window* welcome_menu()
{
    Window *win;
    SJson *json = NULL;
    json = sj_load("json/menus/welcome_menu.json");
    win = window_load_from_json(json);
    win->active = 1;
    if (win)
    {
        win->update = welcome_menu_update;
    }
    sj_free(json);
    return win;
}
void main_menu_update(Window *win,List *updateList)
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
        slog("UPDATE");
        e = gfc_list_get_nth(updateList,i);
        switch(e->index)
        {
            case 64:
            	done = 1;
                Mix_HaltChannel(1);
                gfc_sound_play(k,-1,50,1,1);
                slog("ok");
                break;
            case 62:
                slog("cancel");
                Mix_HaltChannel(1);
                gfc_sound_play(k,-1,50,1,1);
                save_game();
                break;
            case 61:
                apply_block(0);
                Mix_HaltChannel(1);
                gfc_sound_play(k,-1,50,1,1);
            	win->active = 0;
            	break;
            case 60:
            	restart_game();
                princess_ent_new(vector2d(500,200));
                crystal_ent_new(vector2d(250,300));
                crystal_ent_new(vector2d(50,500));
                crystal_ent_new(vector2d(800,500));
                Entity *local = mage_ent_new(vector2d(150,300),20);
                local->team = 0;
                welcome_menu();
                apply_block(0);
                load_map = 1;
                win->active = 0;
                Mix_HaltChannel(1);
                gfc_sound_play(k,-1,50,1,1);
            	break;
            case 63:
                game_manager_clear();
                apply_block(1);
                entity_manager_clear();
                Mix_HaltChannel(1);
                gfc_sound_play(k,-1,50,1,1);
                load_map = 1;
                dirty = 1;
                load_game("saves/save.json");
                break;
            case 65:
                game_manager_clear();
                entity_manager_clear();
                apply_block(1);
                Mix_HaltChannel(1);
                load_map = 1;
                gfc_sound_play(m,-1,50,1,1);
                win->active = 0;
                edit_game();
                break;


        }
    }
}
Window* main_menu()
{
    Window *win;
    SJson *json = NULL;
    json = sj_load("level/testwindow.cfg");
    win = window_load_from_json(json);
    if (win)
    {
        win->update = main_menu_update;
    }
    win->active = 1;
    sj_free(json);
    return win;
}


int main(int argc, char * argv[])
{
    int lop = 0;
//     update_hud_elements(NULL);
    List *lst;
    gfc_audio_init(32,2,2,1,1,0);
    k = gfc_sound_load("audio/battle.mp3",60.0,1);
    m = gfc_sound_load("audio/editor.mp3",90.0,1);
    Sound *s = gfc_sound_load("audio/main.mp3",60.0,1);
    gfc_sound_play(s,-1,50,1,1);
    //slog("Mix_LoadMUS: %s\n", Mix_GetError());
    init_logger("gf2d.log");
    const Uint8 * keys;
    TileMap *tilemap;

    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        2560,
        1440,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16); // 60 fps = 16 ms delay
    gf2d_sprite_init(1024);
    tile_set_manager_init(16);
    entity_manager_init(2000);
    path_manager_init(1000);
    gem_action_list_init(1000);
    SDL_ShowCursor(SDL_DISABLE);
    mouse_load("level/mouse.json");
    tilemap = tilemap_load("level/test.json"); //new_tile.json
	Path_Map *path = Path_Map_load("level/xd.json"); //final.json
	set_path(path->path,path->pathmap_width,path->pathmap_length);
	font_init(40);
	populate_fonts("level/font.json");
	windows_init(400);
	int resources = 0;
    Font *char_font = font_load("images/text.ttf",12);
	Font *game_font = font_load("images/text.ttf",64);
	load_obstacles(tilemap);
 	game_manager_init(12);
	game_instance *opp = game_new(0);
	game_instance *player = game_new(1);
    player->resources += 1000;
    camera_set_dimensions(vector2d(1200,720));
    camera_set_position(vector2d(0,0));
    char out[255] = "";
    char out2[255] = "";
    init_hud("level/hudcp.json");
	opponent_init(opp,player);
//     trigger_ent_new(vector2d(180,90),"json/editor/entityeditor.json");
	Window *win = main_menu();
    while(!done)
    {
        if(dirty)
        {
         opp = get_game(1);
         player = get_game(0);
         dirty = 0;
        }
        SDL_PumpEvents();
        gf2d_graphics_clear_screen();
        keys = SDL_GetKeyboardState(NULL);
        mouse_update();
        windows_update_all();

        sprintf(out,"%s%d","Resources: ",player->resources);
        sprintf(out2,"%s%d","Opponent Resources: ",opp->resources);

        tilemap_draw(tilemap);
        entity_manager_think_all();
        entity_manager_draw_all();
        check_inputs();
        windows_draw_all();
        if(is_dead())
        opponent_think();
        mouse_draw();
        if(win->active == 0)
        {
        if(game_state() == 3 )
        {
        	font_render(game_font,"YOU WON",vector2d(600,300),gfc_color_from_vector4(vector4d(0,255,0,255)));
        }
        else if(game_state()==4)
        {
        	font_render(game_font,"YOU LOST",vector2d(600,300),gfc_color_from_vector4(vector4d(255,0,0,255)));
        }
            font_render(char_font,out2,vector2d(600,700),gfc_color_from_vector4(vector4d(50,0,0,255)));
            font_render(char_font,out,vector2d(800,700),gfc_color_from_vector4(vector4d(50,0,0,255)));
        }
        gf2d_grahics_next_frame();
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            gfc_sound_play(s,-1,50,1,1);
            win->active = 1;
            apply_block(1);
        }
        if (keys[SDL_SCANCODE_F1])
        {
            apply_block(1);
        }
        if(keys[SDL_SCANCODE_F2])
        {
            apply_block(0);
        }
        if(load_map)
        {
            load_map = 0;
            tilemap_free(tilemap);
            tilemap = tilemap_load("level/test.json");
            load_obstacles(tilemap);
        }
    }
    tilemap_free(tilemap);
    Path_Map_free(path);
//     slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

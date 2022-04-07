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
#include "mouse.h"
#include "opponent.h"
#include "zone.h"
#include "load_obstacles.h"
#include "simple_json.h"
#include "elements.h"
#include "windows.h"
#include "game_ops.h"
#include "gfc_audio.h"
static int enabled = 0;
static int done = 0;
static int dirty = 0;

void restart_game()
{
    game_manager_clear();
	entity_manager_clear();
    game_instance *p1 = game_new(0);
    game_instance *p2 = game_new(1);
    opponent_init(p1,p2);
}


int main_menu_update(Window *win,List *updateList)
{
    int i,count;
    Element *e;
    if (!win)return 0;
    if (!updateList)return 0;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList,i);
        if (!e)continue;
        slog("updated element index %i",e->index);
        switch(e->index)
        {
            case 63:
            	done = 1;
                slog("ok");
                break;
            case 52:
                slog("cancel");
                save_game();
                break;
            case 61:
            	enabled = 0;
            	break;
            case 60:
            	restart_game();
            	break;
            case 62:
                game_manager_clear();
                entity_manager_clear();
                dirty = 1;
                load_game("saves/save.json");
            	
        }
    }
    return 0;
}

void main_menu()
{
    Window *win;
    SJson *json = NULL;
    json = sj_load("level/testwindow.cfg");
    win = window_load_from_json(json);
    if (win)
    {
        win->update = main_menu_update;
    }
    sj_free(json);
}

int main(int argc, char * argv[])
{
    gfc_audio_init(32,2,2,1,1,0);
    Sound *s = gfc_sound_load("audio/main.wav",80.0,1);
    //slog("Mix_LoadMUS: %s\n", Mix_GetError());

    gfc_sound_play(s,100,50,1,1);
    init_logger("gf2d.log");
    const Uint8 * keys;
    TileMap *tilemap;

    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    tile_set_manager_init(16);
    entity_manager_init(2000);
    path_manager_init(200);
    gem_action_list_init(128);
    SDL_ShowCursor(SDL_DISABLE);
    mouse_load("level/mouse.json");
    tilemap = tilemap_load("level/test.json");
	Path_Map *path = Path_Map_load("level/xd.json");
	set_path(path->path,path->pathmap_width,path->pathmap_length);
	font_init(40);
	populate_fonts("level/font.json");
	windows_init(20);
	int resources = 0;
	load_obstacles(tilemap);
	char *out = malloc(14 * sizeof(char));
	char *out2 = malloc(14 * sizeof(char));
    crystal_ent_new(vector2d(450,90));
 	game_manager_init(12);
	game_instance *opp = game_new(0);
	game_instance *player = game_new(1);
	opponent_init(opp,player);
    //load_game("saves/save.json");
	main_menu();
    while(!done)
    {
        if(dirty)
        {
         opp = get_game(1);
         player = get_game(0);
         dirty = 0;
        }
        SDL_PumpEvents();
        	keys = SDL_GetKeyboardState(NULL);
        	mouse_update();
        if(enabled)
        windows_update_all();
        sprintf(out,"%s%d","Resources: ",player->resources);
        sprintf(out2,"%s%d","Opponent Resources: ",opp->resources);
        gf2d_graphics_clear_screen();
        tilemap_draw(tilemap);
        entity_manager_think_all();
        entity_manager_draw_all();
		if(enabled)
        windows_draw_all();
        check_inputs();
        if(game_state() == 3 )
        {
        	font_render(font_get_by_tag(FT_H1),"YOU WON",vector2d(600,300),gfc_color_from_vector4(vector4d(0,255,0,255)));
        }
        else if(game_state()==4)
        {
        	font_render(font_get_by_tag(FT_H1),"YOU LOST",vector2d(600,300),gfc_color_from_vector4(vector4d(255,0,0,255)));
        }
        font_render(font_get_by_tag(FT_Small),out2,vector2d(200,700),gfc_color_from_vector4(vector4d(50,0,0,255)));
        font_render(font_get_by_tag(FT_Small),out,vector2d(400,700),gfc_color_from_vector4(vector4d(50,0,0,255)));
        opponent_think();
        mouse_draw();
        gf2d_grahics_next_frame();
        if (keys[SDL_SCANCODE_ESCAPE])enabled = 1;
    }
    free(out);
    free(out2);
    tilemap_free(tilemap);
    Path_Map_free(path);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

#include <SDL.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include <stdio.h>
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
#include "game_instance.h"
#include "mouse.h"
#include "opponent.h"
#include "zone.h"
#include "load_obstacles.h"
#include "simple_json.h"
#include "elements.h"

int main(int argc, char * argv[])
{
init_logger("gf2d.log");
    int done = 0;
    const Uint8 * keys;
    //Sprite *sprite;
    //Sprite *mouse;
    //Vector4D mouseColor = {255,100,255,200};
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
    entity_manager_init(5000);
    path_manager_init(12);
    gem_action_list_init(128);
    SDL_ShowCursor(SDL_DISABLE);
    //mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    mouse_load("level/mouse.json");
    crystal_ent_new(vector2d(350,200),25);
    crystal_ent_new(vector2d(600,500),25);
    tilemap = tilemap_load("level/test.json");
	Path_Map *path = Path_Map_load("level/xd.json");
	set_path(path->path,path->pathmap_width,path->pathmap_length);
	font_init(20);
	int resources = 0;
	load_obstacles(tilemap);
	char *out = malloc(14 * sizeof(char));
	char *out2 = malloc(14 * sizeof(char));
	SJson *json = sj_load("level/input.json");
	Element *e = NULL;
	if(json)
	{
		e = element_load_from_config(json);
	}
	//new_damage_zone(vector2d(40,40),5);
	Font *char_font = font_load("images/text.ttf",12);
	game_manager_init(12);
	//Path_Map_debug(path);
	game_instance *opp = game_new(0);
	game_instance *player = game_new(1);
	opponent_init(opp,player);
	
    while(!done)
    {
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);
        mouse_update();
        //SDL_GetMouseState(&mx,&my);
        //resources++;
        sprintf(out,"%s%d","Resources: ",player->resources);
        sprintf(out2,"%s%d","Opponent Resources: ",opp->resources);
        gf2d_graphics_clear_screen();
        tilemap_draw(tilemap);
        entity_manager_think_all();
        entity_manager_draw_all();
        check_inputs();
        element_draw(e,vector2d(700,80));
        font_render(char_font,out2,vector2d(200,700),gfc_color_from_vector4(vector4d(255,255,0,255)));
        font_render(char_font,out,vector2d(400,700),gfc_color_from_vector4(vector4d(255,255,0,255)));
     	/*gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        */
        element_update(e,vector2d(700,20));
        opponent_think();
        mouse_draw();
        gf2d_grahics_next_frame();
                
        if (keys[SDL_SCANCODE_ESCAPE])done = 1;
    }
    Path_Map_free(path);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

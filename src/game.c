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
int main(int argc, char * argv[])
{
init_logger("gf2d.log");
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
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
    entity_manager_init(1024);
    path_manager_init(12);
    gem_action_list_init(128);
    SDL_ShowCursor(SDL_DISABLE);
    
    //sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
   	//bug_ent_new(vector2d(90,30),30);
    crystal_ent_new(vector2d(200,60),90);
    //building_ent_new(vector2d(240,90));
    //warrior_ent_new(vector2d(100,150),60);
    //mage_ent_new(vector2d(200,150),10);
    tilemap = tilemap_load("level/test.json");
	Path_Map *path = Path_Map_load("level/test.json");
    game_manager_init(20);
    game_new(1);
    game_new(0);
	font_init(1);
	int resources = 0;
	char *out = malloc(14 * sizeof(char));
	
	Font *char_font = font_load("images/text.ttf",32);
	
	
	//Path_Map_debug(path);
    while(!done)
    {
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL); 
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        resources++;
        sprintf(out,"%s%d","Resources: ",resources);
        gf2d_graphics_clear_screen();
        tilemap_draw(tilemap);
        entity_manager_think_all();
        entity_manager_draw_all();
        check_inputs();
        font_render(char_font,out,vector2d(200,200),gfc_color_from_vector4(vector4d(255,255,0,255)));
     	gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        
        gf2d_grahics_next_frame();
                
        if (keys[SDL_SCANCODE_ESCAPE])done = 1;
    }
    Path_Map_free(path);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

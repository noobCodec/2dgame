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
#include "pathfinder.h"
#include "building.h"
#include "camera.h"
int main(int argc, char * argv[])
{
init_logger("gf2d.log");
slog("hello world -->");

/*int tmp[240] = {*/
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,*/
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};*/
/*int visited[240] = {*/
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,*/
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, */
/*0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};*/

/*List *local = BFS(tmp,visited,11,19,0,1,20,12);*/
/*Point *here;*/
/*for(int i =0; i<gfc_list_get_count(local);i++)*/
/*{*/
/*	here = gfc_list_get_nth(local,i);*/
/*	slog("%d,%d",here->x,here->y);*/
/*	free(here);*/
/*}*/
/*gfc_list_delete(local);*/

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
    SDL_ShowCursor(SDL_DISABLE);
    
    //sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    bug_ent_new(vector2d(90,30));
    building_ent_new(vector2d(240,90));
    tilemap = tilemap_load("level/test.json");
	Path_Map *path = Path_Map_load("level/test.json");
	camera_set_dimensions(vector2d(1200,720));
	camera_set_position(vector2d(20,20));
	path_manager_init(12);
	//Path_Map_debug(path);
    while(!done)
    {
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL); 
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        gf2d_graphics_clear_screen();
        tilemap_draw(tilemap);
        entity_manager_think_all();
        List *tmp_ptr = entity_click(mx,my);
        if(gfc_list_get_count(tmp_ptr)>0)
        {
        	slog("clicked it");
        }
        free(tmp_ptr);
            //gf2d_sprite_draw_image(sprite,vector2d(0,0));
            
            
            entity_manager_draw_all();
     
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        //tilemap_update(tilemap);
        gf2d_grahics_next_frame();
                if (keys[SDL_SCANCODE_RIGHT])
        {
            camera_move(vector2d(10,0));
        }
        if (keys[SDL_SCANCODE_LEFT])
        {
            camera_move(vector2d(-10,0));
        }
        if (keys[SDL_SCANCODE_UP])
        {
            camera_move(vector2d(0,-10));
        }
        if (keys[SDL_SCANCODE_DOWN])
        {
            camera_move(vector2d(0,10));
        }
        if (keys[SDL_SCANCODE_ESCAPE])done = 1;
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    Path_Map_free(path);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

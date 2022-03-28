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
static int enabled = 1;


void resetStuff(game_instance *player, game_instance *opp,TileMap *t)
{
	game_manager_clear();
	entity_manager_clear();
	opp = game_new(0);
	player = game_new(1);
	crystal_ent_new(vector2d(350,200),25);
    crystal_ent_new(vector2d(600,500),25);
    load_obstacles(t);
}
void showStuff(Element *e, Element *e2,game_instance *player, game_instance *opp,TileMap *t)
{
	element_draw(e,vector2d(500,650));
    element_draw(e2,vector2d(650,650));
    List* restart = element_update(e,vector2d(500,650));
    if(gfc_list_get_count(restart))
    {
    	resetStuff(player,opp,t);
    }
    restart = element_update(e2,vector2d(650,650));
    if(gfc_list_get_count(restart))
    {
    	exit(0);
    }
}

int main(int argc, char * argv[])
{
/*	int grid[2073600] = {0};*/
/*	for(int i = 0; i < 2073600;i++) grid[i] = 1;*/
/*	List *res = BFS(grid,20,20,1900,1000,1080,1920);*/
/*	for(int i =0; i<gfc_list_get_count(res);i++)*/
/*			{*/
/*				Point *tmp = gfc_list_get_nth(res,i);*/
/*				slog("%d:%d",tmp->x,tmp->y);*/
/*				free(tmp);*/
/*			}*/
/*	gfc_list_delete(res);*/
init_logger("gf2d.log");
    int done = 0;
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
    entity_manager_init(5000);
    path_manager_init(12);
    gem_action_list_init(128);
    SDL_ShowCursor(SDL_DISABLE);
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
	SJson *json = sj_load("level/restart.json");
	Element *e = NULL;
	if(json)
	{
		e = element_load_from_config(json);
	}
	json = sj_load("level/quit.json");
	Element *e2 = NULL;
	if(json)
	{
		e2 = element_load_from_config(json);
	}
	Font *char_font = font_load("images/text.ttf",12);
	Font *game_font = font_load("images/text.ttf",64);
	game_manager_init(12);
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
        if(game_state() == 3 )
        {
        	font_render(game_font,"YOU WON",vector2d(600,300),gfc_color_from_vector4(vector4d(0,255,0,255)));
        }
        else if(game_state()==4)
        {
        	font_render(game_font,"YOU LOST",vector2d(600,300),gfc_color_from_vector4(vector4d(255,0,0,255)));
        }
        font_render(char_font,out2,vector2d(200,700),gfc_color_from_vector4(vector4d(50,0,0,255)));
        font_render(char_font,out,vector2d(400,700),gfc_color_from_vector4(vector4d(50,0,0,255)));
        if(enabled==-1)
        {
        	showStuff(e,e2,opp,player,tilemap);
        }
        if (keys[SDL_SCANCODE_F1])enabled = -1;
        if (keys[SDL_SCANCODE_F2])enabled = 1;
        if(keys[SDL_SCANCODE_F3])resetStuff(opp,player,tilemap);
        opponent_think();
        mouse_draw();
        gf2d_grahics_next_frame();
        if (keys[SDL_SCANCODE_ESCAPE])done = 1;
    }
    free(out);
    free(out2);
    Path_Map_free(path);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

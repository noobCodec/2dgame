#include "opponent.h"
#include "pathfinder.h"
#include "simple_logger.h"
#include <SDL.h>
typedef enum
{
	ATTACK,
	DEFEND,
	HARVEST,
	WON
} states;
static game_instance *computer = NULL;
static game_instance *player = NULL;
static int state = HARVEST;
static Uint32 time_past = 0;


void opponent_init(game_instance *game_t,game_instance *player_t)
{
	computer = game_t;
	player = player_t;
}


void opponent_think()
{
	if(!computer || !player || state == WON) return;
	Entity *ent = NULL;
	if(computer->resources >= 50)
	{
		ent = gfc_list_get_nth(computer->buildings,0);
		ent->build = 2;
	}
	ent = gfc_list_get_nth(computer->buildings,0);
	if(!ent)
	{
	state = WON;
	return;
	}
	//slog("state:%d",ent->inflict);
	if(!ent->inflict)
	{
		ent->inflict = 1;
		time_past = SDL_GetTicks();
		state = DEFEND;
	}
	else if(gfc_list_get_count(computer->units) > gfc_list_get_count(player->units))
	{
		if(state == DEFEND && (SDL_GetTicks()-time_past > 10000))
			state = ATTACK;
		else if(state !=DEFEND)
			state = ATTACK;
	}
	else
	{
		if(state == DEFEND && (SDL_GetTicks()-time_past > 10000))
			state = HARVEST;
		else if(state !=DEFEND)
			state = HARVEST;
	}
	
	if(state==HARVEST)
	{
		Entity *closest = NULL;
		Entity *building = gfc_list_get_nth(computer->buildings,0);
		Vector2D location = building->position;
		float dist = 9999;
		float calculated = 0;
		List *resource = resources();
		for(int i = 0; i<gfc_list_get_count(resource);i++)
		{
			Entity *node = gfc_list_get_nth(resource,i);
			calculated = vector2d_magnitude_between(location,node->position);
			if(dist > calculated)
			{
			closest = node;
			dist = calculated; 
			}
		}
		if(closest)
		{
			for(int i =0; i<gfc_list_get_count(computer->units);i++)
			{
							ent = gfc_list_get_nth(computer->units,i);
							if(ent->damage)
							{
								if(ent->path)
								{
									path_free(ent->path);
									ent->path = NULL;
								}
								Path *pat = path_new();
								path_find(pat,closest->position.x,closest->position.y,ent->position.x,ent->position.y);
								ent->path = pat;
							}
			}
		}
		gfc_list_delete(resource);
	}
	else if (state == ATTACK)
	{
		Entity *enemy_base = gfc_list_get_nth(player->buildings,0);
		if(!enemy_base)
		{
		state = WON;
		return;
		}
		for(int i =0; i<gfc_list_get_count(computer->units);i++)
			{
							ent = gfc_list_get_nth(computer->units,i);
							if(ent->damage)
							{
								if(ent->path)
								{
									path_free(ent->path);
									ent->path = NULL;
								}
								Path *pat = path_new();
								path_find(pat,enemy_base->position.x,enemy_base->position.y,ent->position.x,ent->position.y);
								ent->path = pat;
							}
			}
	}
	else
	{
		Entity *our_base = gfc_list_get_nth(computer->buildings,0);
		if(!our_base)
		{
		state = WON;
		return;
		}
		for(int i =0; i<gfc_list_get_count(computer->units);i++)
			{
							ent = gfc_list_get_nth(computer->units,i);
							if(ent->damage)
							{
								if(ent->path)
								{
									path_free(ent->path);
									ent->path = NULL;
								}
								Path *pat = path_new();
								path_find(pat,our_base->position.x,our_base->position.y,ent->position.x,ent->position.y);
								ent->path = pat;
							}
			}
	}
}

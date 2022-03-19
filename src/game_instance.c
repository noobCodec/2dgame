
#include "game_instance.h"
#include "simple_logger.h"
#include "building.h"
typedef struct
{
    Uint32 max_games;            /**<how many entities exist*/
    game_instance *players;           /**<a big ole list of entities*/
}Game_Manager;

static int t_curr = 1;
static Game_Manager game_manager = {0};


void game_manager_close()
{
    game_manager_clear(); // clear all entities first
    if(game_manager.players != NULL)
    {
        free(game_manager.players);
    }
    slog("game manager closed");    
}

void game_manager_init(Uint32 max_games)
{
    if (max_games == 0)
    {
        slog("cannot allocate memory for zero entities!");
        return;
    }
    if (game_manager.players != NULL)
    {
        slog("game manager already initialized");
        return;
    }
    game_manager.max_games = max_games;
    game_manager.players = gfc_allocate_array(sizeof(game_instance),max_games);
    atexit(game_manager_close);
    slog("game manager initialized");
}

void game_manager_clear()
{
    int i;
    for (i = 0;i < game_manager.max_games;i++)
    {
        if (!&game_manager.players[i])continue;
        game_free(&game_manager.players[i]);
    }
}
game_instance *get_game(int team)
{
    int i;
    for (i = 0;i < game_manager.max_games;i++)
    {
        if (game_manager.players[i].team == team)
        {
            return &game_manager.players[i];
        }
    }
    return NULL;
}
game_instance *game_new(int isPlayer)
{
    int i;
    Entity *tmp;
    for (i = 0;i < game_manager.max_games;i++)
    {
        if (!game_manager.players[i]._inuse)
        {
            //GOT ONE!
            game_manager.players[i]._inuse = 1;
            game_manager.players[i].team = isPlayer ? 0 : t_curr;
            game_manager.players[i].resources = 100;
           	game_manager.players[i].units=gfc_allocate_array(sizeof(Entity),64);
           	game_manager.players[i].buildings=gfc_allocate_array(sizeof(Entity),16);
            if(isPlayer)
                tmp = building_ent_new(vector2d(100,100));
            else
                tmp = building_ent_new(vector2d(900,500));
            tmp->team = isPlayer ? 0 : t_curr++;
            tmp->inflict = isPlayer ? 0 : 1;
            building_append(&game_manager.players[i],tmp);
            return &game_manager.players[i];
        }
    }
    slog("out of entities");
    return NULL;
}
void unit_append(game_instance *g, Entity *unit)
{
	List *local = g->units;
	local = gfc_list_append(local,unit);
	g->units = local;
	
}
void unit_remove(game_instance *g,Entity *unit)
{
	List *local = g->units;
	gfc_list_delete_data(local,unit);
}
void building_append(game_instance *g,Entity *building)
{
	List *local = g->buildings;
	local = gfc_list_append(local,building);
	g->buildings = local;
}

void building_remove(game_instance *g,Entity *building)
{
	List *local = g->buildings;
	gfc_list_delete_data(local,building);
}

void game_free(game_instance *g)
{
    if (g == NULL)
    {
        slog("null pointer provided, nothing to do!");
        return;
    }
    gfc_list_delete(g->units);
    gfc_list_delete(g->buildings);
    memset(g,0,sizeof(game_instance));
}


// eof

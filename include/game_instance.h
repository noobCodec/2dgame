#include "entity.h"

typedef struct game_t
{
    Uint8 _inuse;
    int team;
    int resources;
    Entity *units;
    Entity *buildings;
} game_instance;

void game_manager_init(Uint32 max_instances);

void game_manager_think_all();


/**
 * @brief free all active game
 * @note for use in level transitions.
 */
void game_manager_clear();

/**
 * @brief get a new empty entity
 * @returns NULL on error, or a pointer to a blank entity
 */
game_instance *game_new(int isPlayer);

/**
 * @brief free the memory of an entity
 * @param entity the entity to free
 */
void game_free(game_instance *entity);

game_instance *get_game(int team);
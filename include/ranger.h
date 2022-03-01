#include "entity.h"

/**
 * @brief creates an instance of a ranger entity
 * @param position the location in the world to spawn the bug at
 * @returns NULL on error, or a pointer to the bug
 */
Entity *ranger_ent_new(Vector2D position, int fire_range);

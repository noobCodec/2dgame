#ifndef __WARRIOR_H__
#define __WARRIOR_H__

#include "entity.h"

/**
 * @brief creates an instance of a warrior entity
 * @param position the location in the world to spawn the bug at
 * @returns NULL on error, or a pointer to the bug
 */
Entity *warrior_ent_new(Vector2D position, int fire_range);

#endif

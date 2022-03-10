#ifndef __ROGUE_H__
#define __ROGUE_H__

#include "entity.h"

/**
 * @brief creates an instance of a minotaur entity
 * @param position the location in the world to spawn the bug at
 * @returns NULL on error, or a pointer to the bug
 */
Entity *rogue_ent_new(Vector2D position, int fire_range);

#endif

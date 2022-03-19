#include "entity.h"
#include "gfc_vector.h"
#include <SDL.h>
void zone_think(Entity *self);

Entity *new_fast_zone(Vector2D position,int size);

Entity *new_slow_zone(Vector2D position,int size);

Entity *new_heal_zone(Vector2D position,int size);

Entity *new_damage_zone(Vector2D position,int size);

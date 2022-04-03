
#include "zone.h"
#include "simple_logger.h"
typedef struct
{
	Entity *ent;
	Uint32 old_effects;
} zoneAct;

Uint32 zone_reset(Uint32 t, void *data)
{
	if(!data) return 0;
    Entity *self = data;
    self->blocked = 0;
    if(!self->enemy) return 0;
    
    return 0;
}
void zone_think(Entity *self)
{
	//gf2d_draw_rect(shape_rect_to_sdl_rect(self->bounding),vector4d(0,255,0,255));
	
	List *located = touch(self);
	if(!located) return;
	for(int i = 0; i<gfc_list_get_count(located);i++)
		{
			Entity *node = gfc_list_get_nth(located,i);
			node->effects |= 1 << self->type; 
    		self->blocked = 1;
		}
    gfc_list_delete(located);
	return;
}

Entity *new_fast_zone(Vector2D position,int size)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for zone");
        return NULL;
    }
    ent->think = zone_think;
    ent->type = 1;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,size,size));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}

Entity *new_slow_zone(Vector2D position,int size)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for zone");
        return NULL;
    }
    ent->type = 2;
    ent->think = zone_think;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,size,size));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}
Entity *new_heal_zone(Vector2D position,int size)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for zone");
        return NULL;
    }
    ent->type = 3;
    ent->think = zone_think;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,size,size));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}
Entity *new_damage_zone(Vector2D position,int size)
{
	Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for zone");
        return NULL;
    }
    ent->type = 4;
    ent->think = zone_think;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,size,size));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}

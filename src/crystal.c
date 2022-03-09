#include "crystal.h"
#include "simple_logger.h"
void crystal_think(Entity *self)
{
	if(self->health <= 0 )
	{
		entity_free(self);
		return;
	}
}
Entity *crystal_ent_new(Vector2D position,int fire_range)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/crystal.png",128,128,16);
    ent->think = crystal_think;
    ent->range = shape_circle(position.x,position.y,fire_range);
    ent->draw_scale = vector2d(0.2,0.2);
    ent->velocity = vector2d(0,0);
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->damage = 10;
    ent->health = 100000000;
    ent->path = NULL;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,32,32));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}

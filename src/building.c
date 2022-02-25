#include "simple_logger.h"
#include "bug_ent.h"
#include "building.h"


void building_think(Entity *self)
{
	if(self->build == 2)
	{
		bug_ent_new(vector2d(self->position.x+self->draw_offset.x,self->position.y+self->draw_offset.y+45),30);
		self->build = 1;
	}

}

Entity *building_ent_new(Vector2D position)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for tents");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_image("images/buildings/tent-tan.png");
    ent->think = building_think;
    ent->draw_scale = vector2d(1,1);
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -48;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,128,112));
    ent->rotation.x = 0;
    ent->rotation.y = 0;
    ent->build = 1;
    vector2d_copy(ent->position,position);
    return ent;
}

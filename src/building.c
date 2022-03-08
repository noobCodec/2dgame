#include "simple_logger.h"
#include "mage_ent.h"
#include "goblin.h"
#include "ranger.h"
#include "warrior.h"
#include "rogue.h"
#include "building.h"

//TODO: transform if to switch
void building_think(Entity *self)
{
    Entity *tmp = NULL;
	if(self->build == 2)
	{
		tmp = mage_ent_new(vector2d(self->position.x+self->draw_offset.x+60,self->position.y+self->draw_offset.y+140),60);
		self->build = 1;
	}
	else if(self->build == 3)
	{
		tmp = goblin_ent_new(vector2d(self->position.x+self->draw_offset.x+60,self->position.y+self->draw_offset.y+140),15);
		self->build = 1;
	}
	else if(self->build == 4)
	{
		tmp = ranger_ent_new(vector2d(self->position.x+self->draw_offset.x+60,self->position.y+self->draw_offset.y+140),70);
		self->build = 1;
	}
	else if(self->build == 5)
	{
		tmp = warrior_ent_new(vector2d(self->position.x+self->draw_offset.x+60,self->position.y+self->draw_offset.y+140),30);
		self->build = 1;
	}
	else if(self->build == 6)
	{
		tmp = rogue_ent_new(vector2d(self->position.x+self->draw_offset.x+60,self->position.y+self->draw_offset.y+140),25);
		self->build = 1;
	}
    if(tmp){
    tmp->team = self->team;
    slog("%d",tmp->team);
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

#include "simple_logger.h"
#include "bug_ent.h"
void bug_think(Entity *self)
{
	self->bounding = shape_rect_from_vector4d(vector4d(self->position.x,self->position.y,32,32));
	Vector2D direction;
	float angle;
	gf2d_draw_rect(shape_rect_to_sdl_rect(self->bounding),vector4d(0,255,0,255));
	if(!self) return;
	Entity *enemy = overlap(self);
	if(enemy)
	{
		direction = vector2d(enemy->position.x-self->position.x,enemy->position.y-self->position.y);
		angle = vector2d_angle(direction) - 90;
		self->rotation.z = angle;
		if(self->path)
		{	
			path_free(self->path);
			self->path = NULL;
		}
		
	}
	if(self->path && gfc_list_get_count(self->path->path))
	{
	//slog("p]");
	Vector2D out = travel_location(self->path,self->position.x,self->position.y,60);
	vector2d_copy(self->velocity,out);
	}
	else if(self->path)
	{
	slog("freed");
	path_free(self->path);
	self->path = NULL;
	}
}
Entity *bug_ent_new(Vector2D position)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/space_bug_top.png",128,128,16);
    ent->think = bug_think;
    ent->draw_scale = vector2d(0.5,0.5);
    ent->velocity = vector2d(0,0);
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->damage = 10;
    ent->path = NULL;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,32,32));
    vector2d_copy(ent->position,position);
    return ent;
}

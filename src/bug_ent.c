#include "simple_logger.h"
#include "bug_ent.h"
void bug_think(Entity *self)
{
	if(self->health <= 0 )
	{
		entity_free(self);
		return;
	}
	self->bounding = shape_rect_from_vector4d(vector4d(self->position.x,self->position.y,32,32));
	self->range = shape_circle(self->position.x-self->draw_offset.x,self->position.y-self->draw_offset.y,self->range.r);
	gf2d_draw_circle(vector2d(self->range.x,self->range.y),self->range.r,vector4d(0,255,0,255));
	Vector2D direction;
	float angle;
	gf2d_draw_rect(shape_rect_to_sdl_rect(self->bounding),vector4d(0,255,0,255));
	if(!self) return;
	self->enemy = overlap(self);
	if(self->enemy)
	{
		direction = vector2d(self->enemy->position.x-self->position.x,self->enemy->position.y-self->position.y);
		angle = vector2d_angle(direction) - 90;
		self->rotation.z = angle;
		self->enemy->health -= self->damage;
		slog("%d", self->enemy->health);
		
	}
	if(self->path && gfc_list_get_count(self->path->path))
	{
	//slog("p]");
	Vector2D out = travel_location(self->path,self->position.x,self->position.y);
	slog("%d",self->effects);
	if(self->effects & 2)
	{
	slog("sp[eed]");
	out.x *=4;
	out.y *=4;
	}
	vector2d_copy(self->velocity,out);
	}
	else if(self->path)
	{
	slog("freed");
	path_free(self->path);
	self->path = NULL;
	}
}
Entity *bug_ent_new(Vector2D position,int fire_range)
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
    ent->range = shape_circle(position.x,position.y,fire_range);
    ent->draw_scale = vector2d(0.5,0.5);
    ent->velocity = vector2d(0,0);
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->damage = 10;
    ent->health = 1000;
    ent->path = NULL;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,32,32));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}

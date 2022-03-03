#include "simple_logger.h"
#include "mage_ent.h"
void mage_think(Entity *self)
{
	    	if(self->health <= 0 )
	{
		slog("%d",gem_actor_get_frames_remaining(self->actor));
		if(!self->dead)
		{
			gem_actor_set_action(self->actor,"die");
			self->dead = 1;
			return;
		}
		if(!gem_actor_get_frames_remaining(self->actor))
		{
			gem_actor_free(self->actor);
			entity_free(self);
			return;
		}
		return;
	}
	self->bounding = shape_rect_from_vector4d(vector4d(self->position.x,self->position.y,32,32));
	self->range = shape_circle(self->position.x+16,self->position.y+16,self->range.r);
	gf2d_draw_circle(vector2d(self->range.x,self->range.y),self->range.r,vector4d(0,255,0,255));
	gf2d_draw_rect(shape_rect_to_sdl_rect(self->bounding),vector4d(0,255,0,255));
	if(!self) return;
	self->enemy = overlap(self);
	if(self->enemy)
	{
	if(strcmp(gem_actor_get_current_action(self->actor)->name,"attack"))
	{
				gem_actor_set_action(self->actor,"attack");
	}
		self->enemy->health -= self->damage;
				
	}
	if(self->path && gfc_list_get_count(self->path->path))
	{
		if(strcmp(gem_actor_get_current_action(self->actor)->name,"move"))
	{
				gem_actor_set_action(self->actor,"move");
	}
	
	Vector2D out = travel_location(self->path,self->position.x,self->position.y,30);
	if(out.x < 0)
		self->flip.x = 1;
	else
		self->flip.x = 0;
	vector2d_copy(self->velocity,out);
	}
	else if(self->path)
	{
	if(strcmp(gem_actor_get_current_action(self->actor)->name,"idle"))
	{
				gem_actor_set_action(self->actor,"idle");
	}
	slog("freed");
	path_free(self->path);
	self->path = NULL;
	}
	else if(!self->enemy)
	{
		if(strcmp(gem_actor_get_current_action(self->actor)->name,"idle"))
	{
				gem_actor_set_action(self->actor,"idle");
	}
	}
}
void mage_draw(Entity *self)
{
	if(!self->actor)return;
	gem_actor_draw(self->actor,self->position,NULL,NULL,&self->rotation,&self->flip);
	gem_actor_next_frame(self->actor);

}
Entity *mage_ent_new(Vector2D position,int fire_range)
{
    Entity *ent;
    Actor* tmp = malloc(sizeof(Actor));
    ent = entity_new();
    if (!ent)
    {
        slog("no space for mage");
        return NULL;
    }
    gem_actor_load(tmp,"level/mage.json");
        ent->think = mage_think;
    ent->actor = tmp;
    ent->range = shape_circle(position.x+16,position.y+16,fire_range);
    ent->draw_scale = vector2d(1,1);
    ent->velocity = vector2d(0,0);
    ent->draw_offset.x = 0;
    ent->draw_offset.y = -16;
    ent->rotation.x = 16;
    ent->rotation.y = 16;
    ent->damage = 10;
    ent->flip.x = 0;
    ent->flip.y = 0;
    ent->health = 2000;
    ent->path = NULL;
    ent->dead = 0;
    ent->draw = mage_draw;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,32,32));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}

#include "simple_logger.h"
#include "ranger.h"

Uint32 ranger_attack(Uint32 interval,void *data)
{

    if(!data) return 0;
    Entity *self = data;
    self->blocked = 0;
    if(!self->enemy) return 0;
    if(shape_rect_circle_collision(self->range,self->enemy->bounding))
    {
        if(strcmp(gem_actor_get_current_action(self->actor)->name,"attack"))
	    {
				gem_actor_set_action(self->actor,"attack");
	    }
		self->enemy->health -= self->damage;
		self->enemy->inflict = self->team;
    }
    return 0;
}
void ranger_think(Entity *self)
{
    if(self->effects & 32)
    {
        self->effects ^= 32;
        self->damage *= 2;
    }
	if(self->effects & 8 && self->health < self->max_health)
	{
		self->health += 1;
	}
	else if(self->effects & 16)
	{
		self->health -= 1;
	}
	//gf2d_draw_pixel(self->position,vector4d(0,255,255,255));
    //slog("%d",self->team);
	    	if(self->health <= 0 )
	{
		//slog("%d",gem_actor_get_frames_remaining(self->actor));
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
    if(!gem_actor_get_frames_remaining(self->actor)){gem_actor_set_action(self->actor,"idle");}
	self->bounding = shape_rect_from_vector4d(vector4d(self->position.x,self->position.y,32,32));
	self->range = shape_circle(self->position.x+16,self->position.y+16,self->range.r);
	//gf2d_draw_circle(vector2d(self->range.x,self->range.y),self->range.r,vector4d(0,255,0,255));
	//gf2d_draw_rect(shape_rect_to_sdl_rect(self->bounding),vector4d(0,255,0,255));
	if(!self) return;
	self->enemy = overlap(self);
	if(self->enemy && !self->blocked)
	{
	    SDL_AddTimer(5000,ranger_attack,self);
        self->blocked = 1;
    }
    Vector2D out = travel_location(self->path,self->position.x,self->position.y);
    if(self->effects & 2)
	{
		out.x *=2;
		out.y *=2;
	}
	else if(self->effects & 4)
	{
		out.x /=2;
		out.y /=2;
	}
    //slog("%f:%f",out.x,out.y);
	if(out.x!=0 || out.y!=0)
	{
		if(strcmp(gem_actor_get_current_action(self->actor)->name,"move"))
		{
			gem_actor_set_action(self->actor,"move");
		}
		if(out.x < 0)
			self->flip.x = 1;
		else
			self->flip.x = 0;
		vector2d_copy(self->velocity,out);
	}
	else if(self->path && out.x ==0 && out.y==0)
	{
		slog("freed");
		if(strcmp(gem_actor_get_current_action(self->actor)->name,"idle"))
		{
					gem_actor_set_action(self->actor,"idle");
		}
		//slog("freed");
		path_free(self->path);
		self->velocity = vector2d(0,0);
		self->path = NULL;
	}
	else if(!self->enemy)
	{
		if(strcmp(gem_actor_get_current_action(self->actor)->name,"idle"))
		{
			gem_actor_set_action(self->actor,"idle");
		}
	}
	self->effects = 0;
}
void ranger_draw(Entity *self)
{
	if(!self->actor)return;
	Entity_draw_hp(self);
	if(self->team != 0)
	{
		self->actor->color = vector4d(255,0,24,200);
	}
    if(self->team == 255)
	{
		self->actor->color = vector4d(255,155,155,255);
	}
	gem_actor_draw(self->actor,self->position,NULL,NULL,&self->rotation,&self->flip);
	gem_actor_next_frame(self->actor);

}
Entity *ranger_ent_new(Vector2D position,int fire_range)
{
    Entity *ent;
    Actor* tmp = malloc(sizeof(Actor));
    ent = entity_new();
    if (!ent)
    {
        slog("no space for mage");
        return NULL;
    }
    gem_actor_load(tmp,"actors/ranger.json");
        ent->think = ranger_think;
    ent->actor = tmp;
    ent->team = 255;
    ent->range = shape_circle(position.x+16,position.y+16,fire_range);
    ent->draw_scale = vector2d(1,1);
    ent->velocity = vector2d(0,0);
    ent->draw_offset.x = 0;
    ent->draw_offset.y = -16;
    ent->rotation.x = 16;
    ent->rotation.y = 16;
    ent->damage = 30;
    ent->flip.x = 0;
    ent->id = 3;
    ent->flip.y = 0;
    ent->health = 100;
    ent->max_health=100;
    ent->path = NULL;
    ent->draw = ranger_draw;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,32,32));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}

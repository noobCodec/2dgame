#include "simple_logger.h"
#include "mage_ent.h"
void mage_think(Entity *self)
{
	self->frame = (self->frame + 0.1);
    if (self->frame >= 10)self->frame = 0;
	if(self->health <= 0 )
	{
		entity_free(self);
		return;
	}
	self->bounding = shape_rect_from_vector4d(vector4d(self->position.x,self->position.y,32,32));
	self->range = shape_circle(self->position.x+16,self->position.y+16,self->range.r);
	gf2d_draw_circle(vector2d(self->range.x,self->range.y),self->range.r,vector4d(0,255,0,255));
	Vector2D direction;
	float angle;
	gf2d_draw_rect(shape_rect_to_sdl_rect(self->bounding),vector4d(0,255,0,255));
	if(!self) return;
	self->enemy = overlap(self);
	if(self->enemy)
	{
	if(strcmp(gem_actor_get_current_action(self->actor)->name,"attack"))
	{
		//slog("poo");
		gem_actor_set_action(self->actor,"attack");
	}
		direction = vector2d(self->enemy->position.x-self->position.x,self->enemy->position.y-self->position.y);
		angle = vector2d_angle(direction) - 90;
		self->rotation.z = angle;
		self->enemy->health -= self->damage;
		slog("%d", self->enemy->health);
		
	}
	if(self->path && gfc_list_get_count(self->path->path))
	{
	//slog("p]");
	if(strcmp(gem_actor_get_current_action(self->actor)->name,"move"))
	{
		//slog("poo");
		gem_actor_set_action(self->actor,"move");
	}
	
	Vector2D out = travel_location(self->path,self->position.x,self->position.y,30);
	if(out.x < 0)
		self->flip.x = 1;
	else
		self->flip.x = 0;
	self->rotation.z = angle;
	vector2d_copy(self->velocity,out);
	}
	else if(self->path)
	{
	if(strcmp(gem_actor_get_current_action(self->actor)->name,"idle"))
	{
		//slog("poo");
		gem_actor_set_action(self->actor,"idle");
	}
	slog("freed");
	path_free(self->path);
	self->path = NULL;
	}
}
void mage_draw(Entity *self)
{
	if(!self->actor)return;
	Action *tmp = gem_actor_get_current_action(self->actor);
	slog("%s,%d,%d,%f",tmp->name,tmp->startFrame,tmp->endFrame,tmp->frameRate);
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
    gem_actor_load(tmp,"level/actor.json");
    //ent->sprite = gf2d_sprite_load_all("images/mage.png",32,32,10);
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
    ent->health = 100000000;
    ent->path = NULL;
    ent->draw = mage_draw;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,32,32));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}

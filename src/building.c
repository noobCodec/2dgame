#include "simple_logger.h"
#include "mage_ent.h"
#include "goblin.h"
#include "ranger.h"
#include "warrior.h"
#include "rogue.h"
#include "building.h"
#include "game_instance.h"
#include "mouse.h"
void other_building(Entity *self)
{
	gf2d_draw_rect(shape_rect_to_sdl_rect(self->bounding),vector4d(0,255,0,255));
	if(self->health<0)
	{
		building_remove(get_game(self->team),self);
		entity_free(self);
	}
    Entity *tmp = NULL;
    game_instance *game = get_game(self->team);
    if(!game) return;
    int resources = game->resources;
	if(self->build == 6&& resources >= 250)
	{
		game->resources -= 250;
		tmp = rogue_ent_new(vector2d(self->position.x+self->draw_offset.x,self->position.y+self->draw_offset.y),25);
		self->build = 1;
	}
	else
		self->build = 1;
    if(tmp){
    tmp->team = self->team;
    unit_append(game,tmp);
    }
    self->old_health = self->health;
}
void building_think(Entity *self)
{
	if(self->health<0)
	{
		building_remove(get_game(self->team),self);
		entity_free(self);
	}
    Entity *tmp = NULL;
    game_instance *game = get_game(self->team);
    if(!game) return;
    int resources = game->resources;
	if(self->build == 2 && resources >= 50)
	{
		game->resources -= 50;
		tmp = mage_ent_new(vector2d(self->position.x+self->draw_offset.x+60,self->position.y+self->draw_offset.y+140),60);
		self->build = 1;
	}
	else if(self->build == 3 && resources >= 100)
	{
		game->resources -= 100;
		tmp = goblin_ent_new(vector2d(self->position.x+self->draw_offset.x+60,self->position.y+self->draw_offset.y+140),15);
		self->build = 1;
	}
	else if(self->build == 4&& resources >= 150)
	{
		game->resources -= 150;
		tmp = ranger_ent_new(vector2d(self->position.x+self->draw_offset.x+60,self->position.y+self->draw_offset.y+140),70);
		self->build = 1;
	}
	else if(self->build == 5&& resources >= 200)
	{
		game->resources -= 200;
		tmp = warrior_ent_new(vector2d(self->position.x+self->draw_offset.x+60,self->position.y+self->draw_offset.y+140),30);
		self->build = 1;
	}
	else if(self->build == 7 && resources >=20)
	{
		game->resources -=20;
		tmp = other_building_ent_new(mouse_get_position());
		self->build = 1;
	}
	else
		self->build = 1;
    if(tmp){
    tmp->team = self->team;
    unit_append(game,tmp);
    }
    self->old_health = self->health;
}
void building_draw(Entity *self)
{
	Vector2D drawPosition;
	vector2d_add(drawPosition,self->position,self->draw_offset);
	Entity_draw_hp(self);
	gf2d_sprite_draw(
        self->sprite,        
        drawPosition,
        &self->draw_scale,
        NULL,
        NULL,
        NULL,
        NULL,
        0);
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
    ent->health = 200;
    ent->max_health = 200;
    ent->draw = building_draw;
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
Entity *other_building_ent_new(Vector2D position)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for other");
        return NULL;
    }
    ent->team = 0;
    ent->health = 200;
    ent->max_health = 200;
    ent->draw = building_draw;
    ent->sprite = gf2d_sprite_load_image("images/building2.png");
    ent->think = other_building;
    ent->draw_scale = vector2d(1,1);
    ent->draw_offset.x = -24;
    ent->draw_offset.y = 0;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,64,64));
    ent->rotation.x = 0;
    ent->rotation.y = 0;
    ent->build = 1;
    vector2d_copy(ent->position,position);
    return ent;
}

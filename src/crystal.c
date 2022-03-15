#include "crystal.h"
#include "simple_logger.h"
#include "game_instance.h"
#include <SDL.h>
void crystal_think(Entity *self)
{
	gf2d_draw_rect(shape_rect_to_sdl_rect(self->bounding),vector4d(0,255,0,255));
	if(self->health <= 0 )
	{
		entity_free(self);
		return;
	}
	if(self->health != self->old_health)
	{
		game_instance *tmp = get_game(self->inflict);
		//slog("%d",tmp->resources);
		tmp->resources += self->old_health-self->health;
	}
	self->old_health = self->health;
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
    //ent->elapsed = SDL_GetTicks();
    ent->harvest = 1;
    ent->old_health = 1000;
    ent->team = 255;
    ent->sprite = gf2d_sprite_load_all("images/crystal.png",128,128,16);
    ent->think = crystal_think;
    //ent->range = shape_circle(position.x,position.y,fire_range);
    ent->draw_scale = vector2d(0.2,0.2);
    ent->velocity = vector2d(0,0);
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->damage = 10;
    ent->health = 1000;
    ent->path = NULL;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x-16,position.y-16,32,32));
    ent->enemy = NULL;
    vector2d_copy(ent->position,position);
    return ent;
}

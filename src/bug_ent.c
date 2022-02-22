#include "simple_logger.h"
#include "bug_ent.h"
#include "pathfinder.h"
void bug_think(Entity *self)
{
	static int path_found = 0;
    Vector2D direction;
    int mx,my;
    SDL_Event test_event;
    float angle;
    const Uint8 *keys;
    if (!self)return;
    self->frame = (self->frame + 0.1);
    if (self->frame >= 16)self->frame = 0;
    //self->position = vector2d(50,50);
    SDL_GetMouseState(&mx,&my);
    direction.x = mx - self->position.x;
    direction.y = my - self->position.y;
    angle = vector2d_angle(direction) - 90;
    self->rotation.z = angle;
    self->bounding =shape_rect_from_vector4d(vector4d(self->position.x,self->position.y,16,16));
    int flag = 0;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    
    
    while(SDL_PollEvent(&test_event))
    {
    	switch(test_event.type)
    	{
    		case SDL_MOUSEBUTTONDOWN:
    			flag = 1;
    			break;	
    			
    		
    	}
    }
   
    if(!path_found && flag)
    {
    	path_new(9);
    	path_find(9,mx,my,self->position.x,self->position.y,60);
    	path_found++;
    }
    else if(path_found && has_path(9))
    {
    Vector2D out = travel_location(9,self->position.x,self->position.y,60);
    vector2d_copy(self->velocity,out);
    //slog("path");
    //if(self->velocity.x || self->velocity.y)
    //slog("x:%f,y:%f",self->velocity.x,self->velocity.y);
    }
    else if(path_found && flag)
   	{
   		path_find(9,mx,my,self->position.x,self->position.y,60);
   	}
/*    if (keys[SDL_SCANCODE_W])*/
/*    {*/
/*        // move forward*/
/*        vector2d_set_magnitude(&direction,3);*/
/*        vector2d_copy(self->velocity,direction);*/
/*    }*/
    /*else
    {
        vector2d_scale(self->velocity,self->velocity,0.5);
        if (vector2d_magnitude(self->velocity)<0.05)
        {
            vector2d_clear(self->velocity);
        }
    }*/

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
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->bounding = shape_rect_from_vector4d(vector4d(position.x,position.y,16,16));
    vector2d_copy(ent->position,position);
    return ent;
}

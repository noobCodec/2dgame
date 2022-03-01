#include <SDL.h>
#include "entity.h"
#include "simple_logger.h"
void check_inputs()
{
	SDL_Event event;
	static List *ents = NULL;
	static int init_x;
	static int init_y;
	static int pressed;
	int mx = 0;
	int my = 0;
	Entity *ent;
	SDL_GetMouseState(&mx,&my);
	while(SDL_PollEvent(&event))
        {
        	if(event.type == SDL_MOUSEBUTTONDOWN)
        	{
        	
        		if(event.button.button == SDL_BUTTON_LEFT)
        		{
        			SDL_GetMouseState(&init_x,&init_y);
        			pressed = 1;
        		}
        		if(event.button.button == SDL_BUTTON_RIGHT)
        		{
        		if(ents)
        		{
        		for(int i =0; i<gfc_list_get_count(ents);i++)
					{
						ent = gfc_list_get_nth(ents,i);
						if(ent->damage)
						{
						if(ent->path)
						{
							path_free(ent->path);
							ent->path = NULL;
						}
						//slog("%d",ent->health);
						Path *pat = path_new();
						path_find(pat,mx,my,ent->position.x,ent->position.y,30);
						ent->path = pat;
						}
					}
        		}
        		}
        	}
        	if(event.type == SDL_MOUSEBUTTONUP)
        	{
        		if(event.button.button == SDL_BUTTON_LEFT)
        		{
        			if(ents)
        			{
        				gfc_list_delete(ents);
        				ents = NULL;
        			}
        			pressed = 0;
        			ents = entity_click(mx,my,init_x,init_y);
        			if(!gfc_list_get_count(ents))
        			{
        				gfc_list_delete(ents);
        				ents = NULL;
        			}
        		}
        	}
        
		    if(event.type == SDL_KEYDOWN)
		    {
				switch(event.key.keysym.sym)
				{
					case SDLK_w:
						for(int i =0; i<gfc_list_get_count(ents);i++)
								{
									ent = gfc_list_get_nth(ents,i);
									if(ent->build==1)
									{
										ent->build = 2;
									}
								}
						break;
					case SDLK_e:
						for(int i =0; i<gfc_list_get_count(ents);i++)
								{
									ent = gfc_list_get_nth(ents,i);
									if(ent->build==1)
									{
										ent->build = 3;
									}
								}
						break;
					case SDLK_r:
						for(int i =0; i<gfc_list_get_count(ents);i++)
								{
									ent = gfc_list_get_nth(ents,i);
									if(ent->build==1)
									{
										ent->build = 4;
									}
								}
						break;
					case SDLK_t:
						for(int i =0; i<gfc_list_get_count(ents);i++)
								{
									ent = gfc_list_get_nth(ents,i);
									if(ent->build==1)
									{
										ent->build = 5;
									}
								}
						break;
					case SDLK_y:
						for(int i =0; i<gfc_list_get_count(ents);i++)
								{
									ent = gfc_list_get_nth(ents,i);
									if(ent->build==1)
									{
										ent->build = 6;
									}
								}
						break;
						
				}
		    }
        }
        if(pressed){
        	ShapeRect tmp = shape_rect_from_vector4d(vector4d(MIN(mx,init_x),MIN(my,init_y),MAX(mx,init_x)-MIN(mx,init_x),MAX(my,init_y)-MIN(my,init_y)));
    gf2d_draw_rect(shape_rect_to_sdl_rect(tmp),vector4d(0,255,0,255));
        }
}

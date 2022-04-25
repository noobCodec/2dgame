#include <SDL.h>
#include <stdio.h>
#include "entity.h"
#include "camera.h"
#include "hud.h"
#include "mouse.h"
#include "simple_logger.h"

static List *ents = NULL;
static int init_x;
static int init_y;
static int pressed;
static int external_block = 0;
List* check_inputs()
{
    int mx = 0;
    int my = 0;
	SDL_Event event;
	Entity *ent;
    Vector2D local_vec = mouse_get_position();
	mx = local_vec.x;
    my = local_vec.y;
    if(mouse_button_pressed(0))
    {

     init_x = mx;
     init_y = my;
     pressed = 1;
    }
    if(mouse_button_pressed(2))
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
                    Path *pat = path_new();
                    path_find(pat,mx,my,ent->position.x,ent->position.y);
                    ent->path = pat;
                    }
                }
        }
    }
    if(mouse_button_released(0) && !external_block)
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
        if(gfc_list_get_count(ents)==1)
        {
            Entity *candidate = gfc_list_get_nth(ents,0);
            update_hud_elements(ents);
            if(candidate->build)
                open_building(candidate);
        }
        else
            update_hud_elements(ents);
    }
    else if(mouse_button_released(0))
    {
        external_block = 0;
        pressed = 0;
        update_hud_elements(ents);
    }
	while(SDL_PollEvent(&event))
        {
//         	if(event.type == SDL_MOUSEBUTTONDOWN)
//         	{
//
//         		if(event.button.button == SDL_BUTTON_LEFT)
//         		{
//         			SDL_GetMouseState(&init_x,&init_y);
//         			pressed = 1;
//         		}
//         		if(event.button.button == SDL_BUTTON_RIGHT)
//         		{
//                     if(ents)
//                     {
//                         for(int i =0; i<gfc_list_get_count(ents);i++)
//                             {
//                                 ent = gfc_list_get_nth(ents,i);
//                                 if(ent->damage)
//                                 {
//                                 if(ent->path)
//                                 {
//                                     path_free(ent->path);
//                                     ent->path = NULL;
//                                 }
//                                 Path *pat = path_new();
//                                 path_find(pat,mx,my,ent->position.x,ent->position.y);
//                                 ent->path = pat;
//                                 }
//                             }
//                     }
//         		}
//         	}
//         	if(event.type == SDL_MOUSEBUTTONUP)
//         	{
//         		if(event.button.button == SDL_BUTTON_LEFT && !external_block)
//         		{
// //                     slog("MODIFIED ENTITIES");
//         			if(ents)
//         			{
//         				gfc_list_delete(ents);
//         				ents = NULL;
//         			}
//         			pressed = 0;
//         			ents = entity_click(mx,my,init_x,init_y);
//         			if(!gfc_list_get_count(ents))
//         			{
//         				gfc_list_delete(ents);
//         				ents = NULL;
//         			}
//         		}
//         		else if(event.button.button == SDL_BUTTON_LEFT)
//                 {
// //                     slog("UNBLOCKED");
//                     external_block = 0;
//                     pressed = 0;
//                 }
//                 slog("ENTITY COUNT: %d",gfc_list_get_count(ents));
//                 update_hud_elements(ents);
//         	}

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
					case SDLK_h:
						for(int i =0; i<gfc_list_get_count(ents);i++)
								{
									ent = gfc_list_get_nth(ents,i);
									if(ent->build==1)
									{
										ent->build = 7;
									}
								}
                        break;
                    case SDLK_l:
                        camera_move(vector2d(60,0));
                        break;
                    case SDLK_u:
                        camera_move(vector2d(-60,0));
                        break;
                    case SDLK_j:
                        camera_move(vector2d(0,60));
                        break;
                    case SDLK_k:
                        camera_move(vector2d(0,-60));
                        break;
				}
		    }
//             if(event.type==SDL_MOUSEWHEEL)
//             {
//                 if(event.wheel.y > 0)
//                 {
//                     Vector2D dim = camera_get_dimensions();
//                     dim.x /= 1.02;
//                     dim.y /= 1.02;
//                     camera_set_dimensions(dim);
//
//
//                 }
//
//                 else if(event.wheel.y < 0)
//                 {
//                     Vector2D dim = camera_get_dimensions();
//                     dim.x *= 1.02;
//                     dim.y *= 1.02;
//                     camera_set_dimensions(dim);
//
//                 }
            //}
        }
        if(pressed){
        	ShapeRect tmp = shape_rect_from_vector4d(vector4d(MIN(mx,init_x),MIN(my,init_y),MAX(mx,init_x)-MIN(mx,init_x),MAX(my,init_y)-MIN(my,init_y)));
    gf2d_draw_rect(shape_rect_to_sdl_rect(tmp),vector4d(0,255,0,255));
        }
        if(ents && gfc_list_get_count(ents))
        {
         for(int i = 0; i < gfc_list_get_count(ents);i++)
         {
          Entity *local = gfc_list_get_nth(ents,i);
          if(local->damage)
          gf2d_draw_rect(shape_rect_to_sdl_rect(local->bounding),vector4d(0,255,0,255));
         }

        }
    return NULL;
}
void set_inputs(List *entities)
{
    gfc_list_delete(ents);
    ents = entities;
}
List *get_inputs()
{
    return ents;
}
void block(int value)
{
    external_block = value;
}

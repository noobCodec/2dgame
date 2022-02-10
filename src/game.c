#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_vector.h"
#include "entity.h"
#include "gf2d_draw.h"
#include "gf2d_space.h"
#include "gf2d_collision.h"
int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    //Collision Vibe Check
    
    Collision collision;
    CollisionFilter filter = {0};
    static Body body[100];
    Shape shape[4];
    Space *space = NULL;
    int i;
    //End Collision Vibe Check
    
    //Sprite *sprite;
    int blocked = 1;
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_manager_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    // Entity For Testing
    
    Entity *random = entity_new();
    random->position = vector2d(180,180);
    random->frame = 0;
    random->frameCount = 16;
    random->frameRate = 0.1;
    
    // End Entity
    
    
    /*demo setup*/
    Sprite *bug_sprite = gf2d_sprite_load_all("images/space_bug.png",128,128,16);
    random->sprite = bug_sprite;
    random->update = bug_update;
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    space = gf2d_space_new_full(
        3,
        gf2d_rect(0,0,1200,700),
        0.1,
        vector2d(0,0.1),
        1,
        20);

    shape[1] = gf2d_shape_circle(0,0, 10);
    shape[2] = gf2d_shape_circle(10,0, 15);
    shape[3] = gf2d_shape_rect(-32,-32,64,64);
    shape[0] = gf2d_shape_rect(-16,-16, 32,32);

    /* Stress test*/
        gf2d_body_set(
            &body[0],
            "body",
            1,
            0,
            0,
            0,
            vector2d(256,256),
            vector2d(2.3,4.4),
            10,
            1,
            1,  //elasticity
            &shape[0],
            NULL,
            NULL);
        gf2d_space_add_body(space,&body[0]);
    for (i = 1; i < 100;i++)
    {
        gf2d_body_set(
            &body[i],
            "body",
            1,
            0,
            0,
            0,
            vector2d(256 + 128,256 + 128),
            vector2d(2.5 * gfc_crandom(),3 * gfc_crandom()),
            10,
            1,
            1,  //elasticity
            &shape[i%2],
            NULL,
            NULL);
        gf2d_space_add_body(space,&body[i]);
    }
    filter.worldclip = 1;
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        if(keys[SDL_SCANCODE_C]&& blocked){
        	//Pressed Button Lets Spawn another
			random = entity_new();
	  		random->position = vector2d(180,180);
	  		random->sprite = bug_sprite;
	  		random->update = bug_update;
	  		random->velocity = vector2d(0,0);
	  		random->frame=0;
	  		random->frameCount = 16;
	  		random->frameRate = 0.1;
	  		blocked = 0;
		}
		else if(keys[SDL_SCANCODE_C]==0){
	  		blocked = 1;
		}
		// Start Blastin
        if(keys[SDL_SCANCODE_F] && random){
	  		kill_time();
		}
	
	
		//WASD controls ???
		if(keys[SDL_SCANCODE_W] && random){
		  vector2d_add(random->velocity,vector2d(0,0),vector2d(0,-2));
		}
		if(keys[SDL_SCANCODE_A] && random){
		  vector2d_add(random->velocity,vector2d(0,0),vector2d(-2,0));
		}
		if(keys[SDL_SCANCODE_D] && random){
		  vector2d_add(random->velocity,vector2d(0,0),vector2d(2,0));
		}
		if(keys[SDL_SCANCODE_S] && random){
		  vector2d_add(random->velocity,vector2d(0,0),vector2d(0,2));
		}
	
		// go towards mouse cursor
		if(keys[SDL_SCANCODE_G] && random){
		  vector2d_sub(random->velocity,vector2d(mx,my),random->position);
		  vector2d_scale(random->velocity,random->velocity,0.05);
		}
		//dont go out of bounds
		else if(random && (random->position.x >= 1150 || random->position.x <= 0 || random->position.y >= 650 || random->position.y <= 0 )){
		  random->velocity = vector2d(0,0);
		}
	
		
        gf2d_graphics_clear_screen();// clears drawing buffers
        gf2d_space_update(space);
		collision = gf2d_collision_trace_space(space, vector2d(mx,my), vector2d(600,360) ,filter);
        if (collision.collided)
        {
            gf2d_draw_line(vector2d(mx,my),collision.pointOfContact, vector4d(255,0,0,255));            
        }
        else
        {
            gf2d_draw_line(vector2d(mx,my),vector2d(600,360), vector4d(255,255,0,255));
        }
        gf2d_space_draw(space,vector2d(0,0));
        // all drawing should happen betweem clear_ssrcreen and next_frame
            //backgrounds drawn first
           // gf2d_sprite_draw_image(sprite,vector2d(0,0));
            entity_manager_update_entities();
            entity_manager_draw_entities();
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    gf2d_space_free(space);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

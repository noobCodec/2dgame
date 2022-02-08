#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_vector.h"
#include "entity.h"
int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
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
    Entity *random = entity_new();
    random->position = vector2d(180,180);
    random->frame = 0;
    random->frameCount = 16;
    random->frameRate = 0.1;
    /*demo setup*/
    Sprite *bug_sprite = gf2d_sprite_load_all("images/space_bug.png",128,128,16);
    random->sprite = bug_sprite;
    random->update = bug_update;
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
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
        if(keys[SDL_SCANCODE_F] && random){
	  kill_time();
	}
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
	if(keys[SDL_SCANCODE_G] && random){
	  vector2d_sub(random->velocity,vector2d(mx,my),random->position);
	  vector2d_scale(random->velocity,random->velocity,0.05);
	}
	else if(random && (random->position.x >= 1150 || random->position.x <= 0 || random->position.y >= 650 || random->position.y <= 0 )){
	  random->velocity = vector2d(0,0);
	}
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_ssrcreen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
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
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

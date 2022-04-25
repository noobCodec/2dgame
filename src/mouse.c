#include "mouse.h"
#include "actor.h"
#include "camera.h"
#include <SDL.h>

typedef struct
{
    Uint32 buttons;     /**<buttons mask*/
    Vector2D position;  /**<position of mouse*/
}MouseState;

typedef struct
{
    MouseState mouse[2];        /**<mouse state for the current and last frames*/
    Actor actor;
}Mouse;

static Mouse mouse = {0};

void mouse_set_action(char *action)
{
    gem_actor_set_action(&mouse.actor,action);
}

void mouse_load(char *actorFile)
{
    gem_actor_free(&mouse.actor);
    gem_actor_load(&mouse.actor,actorFile);
}

void mouse_update()
{
    int x,y;
    gem_actor_next_frame(&mouse.actor);

    memcpy(&mouse.mouse[1],&mouse.mouse[0],sizeof(MouseState));
    mouse.mouse[0].buttons = SDL_GetMouseState(&x,&y);
//     camera_set_position(vector2d(x,y));
    vector2d_set(mouse.mouse[0].position,x,y);
}

void mouse_draw()
{
    gem_actor_draw(
    &mouse.actor,
    mouse.mouse[0].position,
    NULL,
    NULL,
    NULL,
    NULL);
}

int mouse_moved()
{
    if ((mouse.mouse[0].position.x != mouse.mouse[1].position.x) ||
        (mouse.mouse[0].position.y != mouse.mouse[1].position.y) ||
        (mouse.mouse[0].buttons != mouse.mouse[1].buttons))
    {
        return 1;
    }
    return 0;
}

int mouse_button_pressed(int button)
{
    int mask;
    mask = 1 << button;
    if ((mouse.mouse[0].buttons & mask) &&
        !(mouse.mouse[1].buttons & mask))
    {
        return 1;
    }
    return 0;
}

int mouse_button_held(int button)
{
    int mask;
    mask = 1 << button;
    if ((mouse.mouse[0].buttons & mask) &&
        (mouse.mouse[1].buttons & mask))
    {
        return 1;
    }
    return 0;
}

int mouse_button_released(int button)
{
    int mask;
    mask = 1 << button;
    if (!(mouse.mouse[0].buttons & mask) &&
        (mouse.mouse[1].buttons & mask))
    {
        return 1;
    }
    return 0;
}

int mouse_button_state(int button)
{
    int mask;
    mask = 1 << button;
    return (mouse.mouse[0].buttons & mask);
}

Vector2D mouse_get_position()
{
    return mouse.mouse[0].position;
}

Vector2D mouse_get_movement()
{
    Vector2D dif;
    vector2d_sub(dif,mouse.mouse[0].position,mouse.mouse[1].position);
    return dif;
}

int mouse_in_rect(ShapeRect r)
{
    return shape_point_in_rect(mouse.mouse[0].position,r);
}

/*eol@eof*/

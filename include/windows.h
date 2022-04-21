#ifndef __MENUS_H__
#define __MENUS_H__
#include "simple_json.h"
#include "gf2d_sprite.h"
#include "gfc_list.h"
#include "elements.h"
#include "gf2d_draw.h"
#include "element_label.h"
#include "element_actor.h"
#include "element_list.h"


typedef enum
{
    BE_TL,
    BE_Top,
    BE_TR,
    BE_Left,
    BE_Right,
    BE_BL,
    BE_Bottom,
    BE_BR
}BorderElements;

typedef struct Window_S
{
    int _inuse;             /**<do not touch*/
    int no_draw_generic;    /**<if true, do not use the generic window draw style*/
    int active;
    int dead;
    List *elements;         /**<all the components of the window*/
    Sprite *background;     /**<background image*/
    Sprite *border;         /**<border sprites*/
    ShapeRect dimensions;        /**<where on the screen*/
    ShapeRect canvas;            /**<Where within the window we draw things*/
    Vector4D color;         /**<color to draw the window with*/
    int (*update)(struct Window_S *win,List *updateList);
    int (*draw)(struct Window_S *win);
    int (*free_data)(struct Window_S *win);
    void *data;             /**<custom data*/
}Window;

/**
 * @brief initialize the window system
 * @param max_windows the limit of active windows
 */
void windows_init(int max_windows);

/**
 * @brief draw all active windows
 */
void windows_draw_all();

/**
 * @brief update all active windows
 */
void windows_update_all();

/**
 * @brief get a new initialized window
 * @return NULL on error or a window pointer
 */
Window *window_new();

/**
 * @brief load a window config from file and return a handle to it
 * @param the filename of the config file
 * @return NULL on error or a window pointer on success
 */
Window *window_load(char *filename);

/**
 * @brief free a window no longer in use
 * @param win the window to free
 */
void window_free(Window *win);

/**
 * @brief add a gui element to the window
 * @param win the window to add an element to
 * @param w the element to add
 */
void window_add_element(Window *win,Element *e);

/**
 * @brief update a window and all of its elements
 * @param win the window to update
 */
void window_update(Window *win);

/**
 * @brief draw a window to the screen
 * @param win the window to draw
 */
void window_draw(Window *win);

/**
 * @brief draw a window given the border sprites and background image
 * @param border the sprite containing the border elements
 * @param bg the sprite to use for the background image (it will be stretch to match the rect
 * @param rect the dimensions of the window to draw
 * @param color the color to draw the window with
 */
void draw_window_border(Sprite *border,Sprite *bg,ShapeRect rect,Vector4D color);

/**
 * @brief draw a generic window using the common border assets
 * @param rect the dimensions of the window to draw
 * @param color the color to draw the window with
 */
void draw_window_border_generic(ShapeRect rect,Vector4D color);

/**
 * @brief make a new window based on the contents of a json config file
 * @param json the json file containing a window config
 * @return NULL on error or the new window otherwise
 */
Window *window_load_from_json(SJson *json);

#endif


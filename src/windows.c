#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "windows.h"
#include "simple_logger.h"

typedef struct
{
    Sprite *generic_border;
    Sprite *generic_background;
    Window *window_list;    /**<*/
    int window_max;         /**<how many windows can exist at once*/
    List *window_deque;     /**<draw order is back to front, update order is front to back*/
    int drawbounds;         /**<if true draw rects around window bounds*/
}WindowManager;

static WindowManager window_manager = {0};

void draw_window_border_generic(ShapeRect rect,Vector4D color)
{
    draw_window_border(window_manager.generic_border,window_manager.generic_background,rect,color);
}

void draw_window_border(Sprite *border,Sprite *bg,ShapeRect rect,Vector4D color)
{
    Vector2D scale = {0};
    if (bg)
    {
        scale.x = rect.w/bg->frame_w;
        scale.y = rect.h/bg->frame_h;
        gf2d_sprite_draw(
            bg,
            vector2d(rect.x,rect.y),
            &scale,
            NULL,
            NULL,
            NULL,
            &color,
            0);
    }
    if (!border)return;
    scale.x = (rect.w - border->frame_w)/(float)border->frame_w;
    scale.y = 1;
    gf2d_sprite_draw(
        border,
        vector2d(rect.x + border->frame_w/2,rect.y - border->frame_w/2),
        &scale,
        NULL,
        NULL,
        NULL,
        &color,
        BE_Top);
    gf2d_sprite_draw(
        border,
        vector2d(rect.x + border->frame_w/2,rect.y + rect.h- border->frame_w/2),
        &scale,
        NULL,
        NULL,
        NULL,
        &color,
        BE_Bottom);

    scale.y = (rect.h - border->frame_h)/(float)border->frame_h;
    scale.x = 1;

    gf2d_sprite_draw(
        border,
        vector2d(rect.x - border->frame_w/2,rect.y + border->frame_w/2),
        &scale,
        NULL,
        NULL,
        NULL,
        &color,
        BE_Left);
    gf2d_sprite_draw(
        border,
        vector2d(rect.x + rect.w - border->frame_w/2,rect.y + border->frame_w/2),
        &scale,
        NULL,
        NULL,
        NULL,
        &color,
        BE_Right);
    
    gf2d_sprite_draw(
        border,
        vector2d(rect.x + rect.w - border->frame_w/2,rect.y - border->frame_w/2),
        NULL,
        NULL,
        NULL,
        NULL,
        &color,
        BE_TR);
    gf2d_sprite_draw(
        border,
        vector2d(rect.x - border->frame_w/2,rect.y + rect.h - border->frame_w/2),
        NULL,
        NULL,
        NULL,
        NULL,
        &color,
        BE_BL);
    gf2d_sprite_draw(
        border,
        vector2d(rect.x - border->frame_w/2,rect.y - border->frame_w/2),
        NULL,
        NULL,
        NULL,
        NULL,
        &color,
        BE_TL);
    gf2d_sprite_draw(
        border,
        vector2d(rect.x + rect.w - border->frame_w/2,rect.y + rect.h - border->frame_w/2),
        NULL,
        NULL,
        NULL,
        NULL,
        &color,
        BE_BR);
    if (window_manager.drawbounds)
    {
        gf2d_draw_rect(shape_rect_to_sdl_rect(rect),vector4d(255,100,100,255));
    }
}

void windows_close()
{
    int i;
    for (i = 0;i < window_manager.window_max;i++)
    {
        if (window_manager.window_list[i]._inuse)
        {
            window_free(&window_manager.window_list[i]);
        }
    }
    gfc_list_delete(window_manager.window_deque);
    slog("window system closed");
}

void windows_init(int max_windows)
{
    if (max_windows <= 0)
    {
        slog("cannot initilize window system for 0 windows");
        return;
    }
    window_manager.window_list = (Window*)malloc(sizeof(Window)*max_windows);
    if(window_manager.window_list == NULL)
    {
        slog("failed to allocate memory for window system");
        return;
    }
    memset(window_manager.window_list,0,sizeof(Window)*max_windows);
    window_manager.window_max = max_windows;
    window_manager.window_deque = gfc_list_new();
    window_manager.generic_background = gf2d_sprite_load_image("images/window_background.png");
    window_manager.generic_border = gf2d_sprite_load_all("images/window_border.png",64,64,8);
    window_manager.drawbounds = 1;
    slog("window system initilized");
    atexit(windows_close);
}

void window_free(Window *win)
{
    int i,count;
    if (!win)return;
    if (win->free_data != NULL)
    {
        win->free_data(win);
    }
    gfc_list_delete_data(window_manager.window_deque,win);
    count = gfc_list_get_count(win->elements);
    for (i = 0;i < count;i++)
    {
        element_free((Element*)gfc_list_get_nth(win->elements,i));
    }
    gfc_list_delete(win->elements);
    gf2d_sprite_free(win->background);
    gf2d_sprite_free(win->border);
    memset(win,0,sizeof(Window));
}

void window_draw(Window *win)
{
    int count,i;
    Vector2D offset;
    if (!win)return;
    if (!win->no_draw_generic)
    {
        draw_window_border_generic(win->dimensions,win->color);
    }
    offset.x = win->dimensions.x + win->canvas.x;
    offset.y = win->dimensions.y + win->canvas.y;
    count = gfc_list_get_count(win->elements);
    for (i = 0;i < count;i++)
    {
        element_draw((Element *)gfc_list_get_nth(win->elements,i), offset);
    }
}

void window_update(Window *win)
{
    int count,i;
    Vector2D offset;
    List *updateList = NULL;
    List *updated = NULL;
    Element *e;
    if (!win)return;
    updateList = gfc_list_new();
    offset.x = win->dimensions.x + win->canvas.x;
    offset.y = win->dimensions.y + win->canvas.y;
    count = gfc_list_get_count(win->elements);
    for (i = 0;i < count;i++)
    {
        e = (Element *)gfc_list_get_nth(win->elements,i);
        if (!e)continue;
        updated = element_update(e, offset);
        if (updated)
        {
            if (!updateList)
            {
                updateList = gfc_list_new();
            }
            gfc_list_concat_free(updateList,updated);
        }
    }
    if (win->update)
    {
        win->update(win,updateList);
    }
    gfc_list_delete(updateList);
}

Window *window_new()
{
    int i;
    for (i = 0;i < window_manager.window_max;i++)
    {
        
        if (!window_manager.window_list[i]._inuse)
        {
           window_manager.window_list[i]._inuse = 1;
           gfc_list_append(window_manager.window_deque,&window_manager.window_list[i]);
           window_manager.window_list[i].elements = gfc_list_new();
           return &window_manager.window_list[i];
        }
    }
    return NULL;
}

void window_add_element(Window *win,Element *e)
{
    if (!win)return;
    if (!e)return;
    gfc_list_append(win->elements,e);
}

void windows_draw_all()
{
    int i,count;
    count = gfc_list_get_count(window_manager.window_deque);
    for (i = 0; i < count; i++)
    {
        window_draw((Window*)gfc_list_get_nth(window_manager.window_deque,i));
    }
}

void windows_update_all()
{
    int i,count;
    count = gfc_list_get_count(window_manager.window_deque);
    for (i = count - 1; i >= 0; i--)
    {
        window_update((Window*)gfc_list_get_nth(window_manager.window_deque,i));
    }
}

Window *window_load_from_json(SJson *json)
{
	int x,y,z,k;
    Window *win = NULL;
    int i,count;
    Vector4D vector = {255,255,255,255};
    SJson *elements,*value;
    if (!json)
    {
        slog("json not provided");
        return NULL;
    }
    json = sj_object_get_value(json,"window");
    if (!json)
    {
        slog("json does not contain window definition");
        return NULL;
    }
    win = window_new();
    if (!win)
    {
        slog("failed to create new window");
        return NULL;
    }
    value = sj_object_get_value(json,"color");
    sj_get_integer_value(sj_array_get_nth(value,0),&x);
    sj_get_integer_value(sj_array_get_nth(value,1),&y);
    sj_get_integer_value(sj_array_get_nth(value,2),&z);
    sj_get_integer_value(sj_array_get_nth(value,3),&k);
    win->color = vector4d(x,y,z,k);
    
    vector4d_clear(vector);
    value = sj_object_get_value(json,"dimensions");
    sj_get_integer_value(sj_array_get_nth(value,0),&x);
    sj_get_integer_value(sj_array_get_nth(value,1),&y);
    sj_get_integer_value(sj_array_get_nth(value,2),&z);
    sj_get_integer_value(sj_array_get_nth(value,3),&k);
    vector = vector4d(x,y,z,k);
    win->dimensions = shape_rect_from_vector4d(vector4d(vector.x,vector.y,vector.z,vector.w));

    
    elements = sj_object_get_value(json,"elements");
    count = sj_array_get_count(elements);
    for (i = 0; i< count; i++)
    {
        value = sj_array_get_nth(elements,i);
        if (!value)continue;
        window_add_element(win,element_load_from_config(value));
    }
    return win;
}

/*eol@eof*/


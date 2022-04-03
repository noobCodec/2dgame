#ifndef __FONT_H__
#define __FONT_H__

#include <SDL_ttf.h>

#include "gfc_vector.h"
#include "gfc_text.h"
#include "gfc_types.h"
#include "gfc_color.h"
typedef struct
{
    Uint32      _refCount;  /**<how many times this specific resource has been used*/
    TTF_Font   *font;       /**<the handle to the TTF_Font we will use*/
    TextLine    filename;   /**<filename of the font we loaded*/
    int         ptsize;     /**<the font size loaded.*/
}Font;


typedef enum
{
    FT_Normal,
    FT_Small,
    FT_H1,
    FT_H2,
    FT_H3,
    FT_H4,
    FT_H5,
    FT_H6,
    FT_MAX
}FontTypes;
/**
 * @brief initialize font resource manager
 * @param maxFonts how many fonts to support
 */
void font_init(Uint32 maxFonts);

Font *font_load(const char *filename, int ptsize);
void font_free(Font * font);
void font_render(Font *font,char *text,Vector2D position,Color color);
Font *font_get_by_tag(FontTypes tag);
Vector2D text_get_bounds(char *text,FontTypes tag);
Vector2D font_get_bounds(char *text,Font *font);
void populate_fonts(char *file);
void text_draw_line(char *text,FontTypes tag, Color color, Vector2D position);
void font_manager_clear();
#endif

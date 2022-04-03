#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "font.h"
#include "simple_json.h"

typedef struct
{
    Font *fontList;
    Font *font_tags[FT_MAX];
    Uint32 maxFonts;
}FontManager;

static FontManager font_manager = {0};

void font_delete(Font *font)
{
    if (!font)return;
    if (font->font != NULL)TTF_CloseFont(font->font);
    memset(font,0,sizeof(Font));
}

void font_manager_close()
{
    int i;
    if (font_manager.fontList != NULL)
    {
        for (i = 0 ; i < font_manager.maxFonts; i++)
        {
            font_delete(&font_manager.fontList[i]);
        }
    }
    free(font_manager.fontList);
    memset(&font_manager,0,sizeof(FontManager));
}
void font_manager_clear()
{
	int i;
    if (font_manager.fontList != NULL)
    {
        for (i = 0 ; i < font_manager.maxFonts; i++)
        {
            font_delete(&font_manager.fontList[i]);
        }
    }
}
void font_init(Uint32 maxFonts)
{
    if (!maxFonts)
    {
        slog("cannot allocate for 0 fonts");
        return;
    }
    if((!TTF_WasInit()) && (TTF_Init()==-1))
    {
        slog("TTF_Init: %s\n", TTF_GetError());
        return;
    }
    font_manager.maxFonts = maxFonts;
    font_manager.fontList = (Font *)gfc_allocate_array(sizeof(Font),maxFonts);
    atexit(font_manager_close);
}

Font *font_new()
{
    int i;
    for (i = 0; i < font_manager.maxFonts; i++)
    {
        if (font_manager.fontList[i]._refCount == 0)
        {
            font_manager.fontList[i]._refCount = 1;
            return &font_manager.fontList[i];
        }
    }
    return NULL;
}

Font *font_get_by_file_point(const char *filename, int ptsize)
{
    int i;
    for (i = 0; i < font_manager.maxFonts; i++)
    {
        if (font_manager.fontList[i]._refCount == 0)continue;
        if (gfc_line_cmp(font_manager.fontList[i].filename,filename) != 0)continue;
        if (font_manager.fontList[i].ptsize != ptsize)continue;
        return &font_manager.fontList[i];
    }
    return NULL;
}

Font *font_load(const char *filename, int ptsize)
{
    Font *font;
    
    font = font_get_by_file_point(filename,ptsize);
    if (font != NULL)
    {// we have this in memory already, increase the refCount and return it
        font->_refCount++;
        return font;
    }
    
    font = font_new();
    
    if (!font)
    {
        slog("we failed to get a new font slot!");
        return NULL;
    }
    
    font->ptsize = ptsize;
    gfc_line_cpy(font->filename,filename);
    font->font = TTF_OpenFont(filename, ptsize);
    
    if (font->font == NULL)
    {
        slog("failed to open font %s!, re: %s",filename,TTF_GetError());
        font_delete(font);
        return NULL;
    }
    
    switch(ptsize)
    {
    	case 14:
    		font_manager.font_tags[FT_Normal] = font;
    		break;
    	case 10:
    		font_manager.font_tags[FT_Small] = font;
    		break;
    	case 28:
    		font_manager.font_tags[FT_H1] = font;
    		break;
    	case 24:
    		font_manager.font_tags[FT_H2] = font;
    		break;
    	case 22:
    		font_manager.font_tags[FT_H3] = font;
    		break;
    	case 20:
    		font_manager.font_tags[FT_H4] = font;
    		break;
    	case 16:
    		font_manager.font_tags[FT_H5] = font;
    		break;
    	case 15:
    		font_manager.font_tags[FT_H6] = font;
    		break;
    }
    return font;
}

void font_free(Font * font)
{
    if (!font)return;
    font->_refCount--;
    if (font->_refCount == 0)font_delete(font);
}

void font_render(Font *font,char *text,Vector2D position,Color color)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
    if ((!font)||(!font->font))
    {
        slog("no font provided to render!");
        return;
    }
    if (!text)
    {
        slog("no text provided to render!");
    }
    surface = TTF_RenderText_Blended(font->font, (const char *)text, gfc_color_to_sdl(color));
    
    if (!surface)
    {
        slog("failed to render font text %s", text);
        return;
    }
    
    surface = gf2d_graphics_screen_convert(&surface);
    if (!surface)
    {
        slog("failed to convert rendered text to graphics format: %s",text);
        return;
    }
    
    texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(),surface);
    if (!texture)
    {
        slog("failed to convert surface text to texture %s",text);
        SDL_FreeSurface(surface);
        return;
    }
    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
    
    
    rect.x = (Sint32)position.x;
    rect.y = (Sint32)position.y;
    rect.w = surface->w;
    rect.h = surface->h;
    
    SDL_UpdateTexture(texture,
                    NULL,
                    surface->pixels,
                    surface->pitch);
    SDL_RenderCopyEx(gf2d_graphics_get_renderer(),
                     texture,
                     NULL,
                     &rect,
                     0,
                     NULL,
                     0);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

Font *font_get_by_tag(FontTypes tag)
{
    if ((tag < 0) ||(tag >= FT_MAX))
    {
        slog("bad font tag: %i",tag);
        return NULL;
    }
    return &font_manager.fontList[tag];
}

Vector2D font_get_bounds(char *text,Font *font)
{
    int x = -1,y = -1;
    if (!text)
    {
        slog("cannot size text, none provided");
        return vector2d(-1,-1);
    }
    if (!font)
    {
        slog("cannot size text, no font provided");
        return vector2d(-1,-1);
    }
    
    TTF_SizeUTF8(font->font, text, &x,&y);
    return vector2d(x,y);
}


Vector2D text_get_bounds(char *text,FontTypes tag)
{
	return font_get_bounds(text,font_get_by_tag(tag));
}

void text_draw_line(char *text,FontTypes tag, Color color, Vector2D position)
{
	font_render(font_get_by_tag(tag),text,position,color);
}
void populate_fonts(char *file)
{
	SJson *json = NULL;
	SJson *arr = NULL;
	SJson *element = NULL;
	int e;
	const char *fontFile = NULL;
	json = sj_load("level/font.json");
	if(!json)
	{
		slog("no json loaded in populate_fonts");
		return;
	}
	fontFile = sj_get_string_value(sj_object_get_value(json,"file"));
	arr = sj_object_get_value(json,"sizes");
	
	for(int i = 0; i < sj_array_get_count(arr);i++)
	{
		element = sj_array_get_nth(arr,i);
		sj_get_integer_value(element, &e);
		font_load(fontFile,e);
	}
	sj_free(json);
}


#ifndef __TILE_SET_H__
#define __TILE_SET_H__

#include "gfc_text.h"

#include "gf2d_sprite.h"

typedef struct
{
    Uint8       _refcount;
    TextLine    filename;
    Sprite     *tile_image; //image loaded for a tile set
    Uint32      tile_width;
    Uint32      tile_height;
    Uint32      tile_count; //how many tiles are in the set
}TileSet;

void tile_set_manager_init(Uint32 tileset_count);

TileSet *tile_set_load(
    char       *filename,
    Uint32      tile_width,
    Uint32      tile_height,
    Uint32      tile_count);

void tile_set_free(TileSet *tileset);

/**
 * @brief draw a tile from a tile set to the screen
 * @param tileset the set to draw a tile from
 * @param tile which tile to draw
 * @param position the screen position to draw the tile at
 */
void tile_set_draw(TileSet *tileset,Uint32 tile,Vector2D position);

#endif		

#ifndef __PATH_MAP_H__
#define __PATH_MAP_H__

#include "gfc_text.h"

typedef struct
{
    TextLine filename;      //map file
    Uint32	 *path;
    Uint32   pathmap_length;  // how many tiles we have
    Uint32   pathmap_width;  // how many tiles across we have
}Path_Map;

Path_Map *Path_Map_load(char *filename);
void Path_Map_free(Path_Map *map);
void Path_Map_debug(Path_Map *map);

#endif

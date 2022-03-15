#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <SDL.h>
#include "gfc_list.h"
#include "gfc_vector.h"
typedef struct
{
	Uint8 _inuse;
	List *path;
} Path;

void path_manager_init(Uint32 max_paths);

void path_manager_close();

void path_manager_clear();

void path_free(Path *path);

Path *path_new();

void path_find(Path *path,int srcx,int srcy,int dstx,int dsty);

Vector2D travel_location(Path *path,float x, float y);

void set_path(int *arr,Uint32 rows,Uint32 cols);

#endif

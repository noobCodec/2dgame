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

void path_find(Path *path,int srcx,int srcy,int dstx,int dsty,int scale);

Vector2D travel_location(Path *path,float x, float y, int scale);


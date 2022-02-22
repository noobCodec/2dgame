#include <SDL.h>
#include "gfc_list.h"
#include "gfc_vector.h"
typedef struct {
	Uint8 ID;
	List* path;
} PathFind;

void path_manager_init(Uint32 max_paths);

void path_manager_close();

void path_manager_clear();

void path_free(PathFind *path);

void path_new(Uint8 id);

void path_find(Uint8 id,int srcx,int srcy,int dstx,int dsty,int scale);

Vector2D travel_location(Uint8 id,float x, float y, int scale);

int has_path(Uint8 id);

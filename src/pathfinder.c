#include "pathfinder.h"
#include "simple_logger.h"
#include "algo.h"
#include <string.h>
#include <stdlib.h>
typedef struct
{
    Uint32 max_paths; 
    Path  *path_list;       
}PathManager;

typedef struct
{
Uint32 rows;
Uint32 cols;
int *path_map;
}path_set;
static PathManager path_manager = {0};

static path_set tmp = {0};


int get_rows()
{
return tmp.rows;
}
int get_cols()
{
return tmp.cols;
}

void set_path(int *arr,Uint32 rows,Uint32 cols)
{
	if(arr == NULL) return;
	tmp.rows = rows;
	tmp.cols = cols;
	tmp.path_map = malloc(sizeof(int) * rows * cols);
	memcpy(tmp.path_map,arr,sizeof(int)*rows*cols);
	
/*	for(int i = 0 ; i < rows*cols;i++)*/
/*	{*/
/*		slog("%d ",tmp.path_map[i]);*/
/*	}*/
}
void path_manager_init(Uint32 max_paths)
{
	if(max_paths == 0)
	{
		return;
	}
	if(path_manager.path_list != NULL)
	{
		return;
	}
	path_manager.max_paths = max_paths;
	path_manager.path_list = gfc_allocate_array(sizeof(Path),max_paths);
	atexit(path_manager_close);
}
void path_manager_close()
{
	path_manager_clear();
	free(tmp.path_map);
	free(path_manager.path_list);
}
void path_manager_clear()
{
	int i;
	for(i = 0; i < path_manager.max_paths;i++)
	{
        if(path_manager.path_list[i]._inuse)
		path_free(&path_manager.path_list[i]);
	}
}
void path_free(Path *path)
{

    if (path == NULL)
    {
        slog("null pointer provided, nothing to do!");
        return;
    }
	Point *here;
	List *my_path = path->path;
    for(int i =0; i<gfc_list_get_count(my_path);i++)
	{
		here = gfc_list_get_nth(my_path,i);
		free(here);
	}
	gfc_list_delete(my_path);
    memset(path,0,sizeof(Path));
    
}
Path *path_new()
{
	int i;
    for (i = 0;i < path_manager.max_paths;i++)
    {
        if (!path_manager.path_list[i]._inuse)
        {
            path_manager.path_list[i]._inuse = 1;
            return &path_manager.path_list[i];
        }
    }
	return NULL;
}
void path_find(Path* path,int srcx,int srcy,int dstx,int dsty)
{
	//slog("%d:%d,%d:%d",srcx/60,srcy/60,dstx/60,dsty/60);
	int scale = 1200 / tmp.cols;
    if(path)
    {
		path->path = BFS(tmp.path_map,srcx/scale,srcy/scale,dstx/scale,dsty/scale,tmp.rows,tmp.cols);
    }
    return;
}
Vector2D travel_location(Path* path,float x, float y)
{
	int scale = 1200 / tmp.cols;
	//slog("%d",scale);
	int realx = x / scale;
	int realy = y / scale;
	float calcx;
	float calcy;
    if(!path)
    {
    	//slog("dead");
    	return vector2d(0,0);
    }
    List *next = path->path;
    if(!next)
    {
    	//slog("dead");
    	return vector2d(0,0);
    } 
    Point *local = gfc_list_get_nth(next,0);
    if(gfc_list_get_count(next) && local->x==realx && local->y==realy)
    {
		gfc_list_delete_nth(next,0);
        free(local);
		local = gfc_list_get_nth(next,0);
    }
    if(!gfc_list_get_count(next)) 
    {
    	calcx = realx * scale + scale * 0.5;
    	calcy = realy * scale + scale * 0.5;
    	if(abs(calcx-x) < (scale*.01) && abs(calcy-y) < (scale*.01))
    	{
    	//slog("kek");
    	return vector2d(0,0);
    	}
    	//slog("cringe");
    	Vector2D res = vector2d(calcx-x,calcy-y);
    	vector2d_set_magnitude(&res,1);
    	return res;
    }
    calcx = local->x * scale + scale * 0.5;
    calcy = local->y * scale + scale * 0.5;
    realx = realx * scale + scale * 0.5;
    realy = realy * scale + scale * 0.5;
    Vector2D res = vector2d(calcx-realx,calcy-realy);
    vector2d_set_magnitude(&res,1);
    return res;
}




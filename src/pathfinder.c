#include "pathfinder.h"
#include "simple_logger.h"
#include "algo.h"
typedef struct
{
    Uint32 max_paths; 
    Path  *path_list;       
}PathManager;

static PathManager path_manager = {0};

	int tmp[960] = {0};
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
	free(path_manager.path_list);
}
void path_manager_clear()
{
	int i;
	for(i = 0; i < path_manager.max_paths;i++)
	{
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
void path_find(Path* path,int srcx,int srcy,int dstx,int dsty,int scale)
{
	//slog("%d:%d,%d:%d",srcx/60,srcy/60,dstx/60,dsty/60);
	int visited[960];
	memset(visited,0,sizeof(visited));
	
    if(path)
    {
		path->path = BFS(tmp,visited,srcx/scale,srcy/scale,dstx/scale,dsty/scale,24,40);
		slog("path found");
    }
    return;
}

Vector2D travel_location(Path* path,float x, float y,int scale)
{
	int realx = x / scale;
	int realy = y / scale;
	float calcx;
	float calcy;
    if(!path->path)
    {
    	return vector2d(0,0);
    }
    List *next = path->path;
    if(!next)
    {
    	return vector2d(0,0);
    } 
    Point *local = gfc_list_get_nth(next,0);
    if(gfc_list_get_count(next) && local->x==realx && local->y==realy)
    {
		gfc_list_delete_nth(next,0);
		local = gfc_list_get_nth(next,0);
    }
    if(!gfc_list_get_count(next)) 
    {
    	return vector2d(0,0);
    }
    //slog("pathing");
/*    slog("x:%f,y:%f",local->x,local->y);*/
/*    slog("LOCALx:%f,y:%f",realx,realy);*/
    calcx = local->x * scale + scale * 0.5;
    calcy = local->y * scale + scale * 0.5;
    realx = realx * scale + scale * 0.5;
    realy = realy * scale + scale * 0.5;
/*    slog("x:%f,y:%f",calcx,calcy);*/
    Vector2D res = vector2d(calcx-realx,calcy-realy);
/*    slog("RESx:%f,y:%f",res.x,res.y);*/
    vector2d_set_magnitude(&res,1);
    return res;
}




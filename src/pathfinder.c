#include "pathfinder.h"
#include "simple_logger.h"
#include "algo.h"
typedef struct
{
    Uint32 max_paths; 
    PathFind  *path_list;       
}PathManager;

static PathManager path_manager = {0};

	int tmp[240] = {0};
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
	path_manager.path_list = gfc_allocate_array(sizeof(PathFind),max_paths);
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
void path_free(PathFind *path)
{

    if (path == NULL)
    {
        slog("null pointer provided, nothing to do!");
        return;
    }
    if (path->path != NULL)
    {
    	Point *here;
        for(int i =0; i<gfc_list_get_count(path->path);i++)
		{
			here = gfc_list_get_nth(path->path,i);
			free(here);
		}
		gfc_list_delete(path->path);
    }
    memset(path,0,sizeof(PathFind));
    
}
void path_new(Uint8 id)
{
	int i;
    for (i = 0;i < path_manager.max_paths;i++)
    {
        if (!path_manager.path_list[i].ID)
        {
            path_manager.path_list[i].ID = id;
            return;
        }
    }

}
void path_find(Uint8 id,int srcx,int srcy,int dstx,int dsty,int scale)
{
	slog("%d:%d,%d:%d",srcx,srcy,dstx,dsty);
	int visited[240];
	memset(visited,0,sizeof(visited));
	PathFind *ptr = NULL;
	int i;
    for (i = 0;i < path_manager.max_paths;i++)
    {
        if (path_manager.path_list[i].ID == id)
        {
            ptr = &path_manager.path_list[i];
            break;
        }
    }
    if(ptr)
    {
    List *myq = BFS(tmp,visited,srcx/scale,srcy/scale,dstx/scale,dsty/scale,12,20);
    ptr->path=myq;
    }
    return;
}
int has_path(Uint8 id)
{

	PathFind *ptr = NULL;
	int i;
    for (i = 0;i < path_manager.max_paths;i++)
    {
        if (path_manager.path_list[i].ID == id)
        {
            ptr = &path_manager.path_list[i];
            break;
        }
    }
    if(ptr && ptr->path == NULL)
    {
    	return 0;
    }
    	return 1;
}
Vector2D travel_location(Uint8 id,float x, float y,int scale)
{
	PathFind *ptr = NULL;
	int realx = x / scale;
	int realy = y / scale;
	float calcx;
	float calcy;
	int i;
    for (i = 0;i < path_manager.max_paths;i++)
    {
        if (path_manager.path_list[i].ID == id)
        {
            ptr = &path_manager.path_list[i];
            break;
        }
    }
    if(!ptr)
    {
    return vector2d(0,0);
    }
    List *next = ptr->path;
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
    	Point *here;
        for(int i =0; i<gfc_list_get_count(next);i++)
		{
			here = gfc_list_get_nth(next,i);
			free(here);
		}
		gfc_list_delete(next);
    	ptr->path = NULL;
    	return vector2d(0,0);
    }
/*    slog("x:%f,y:%f",local->x,local->y);*/
/*    slog("LOCALx:%f,y:%f",realx,realy);*/
    calcx = local->x * scale + scale * 0.5;
    calcy = local->y * scale + scale * 0.5;
    realx = realx * scale + scale * 0.5;
    realy = realy * scale + scale * 0.5;
/*    slog("x:%f,y:%f",calcx,calcy);*/
    Vector2D res = vector2d(calcx-realx,calcy-realy);
/*    slog("RESx:%f,y:%f",res.x,res.y);*/
    vector2d_set_magnitude(&res,5);
    return res;
}




#include "pathfinder.h"
#include "simple_logger.h"
typedef struct
{
    Uint32 max_paths; 
    PathFind  *path_list;       
}PathManager;

static PathManager path_manager = {0};

	int tmp[240] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int visited[240] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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
        killqueue(path->path);
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
void path_find(Uint8 id,int srcx,int srcy,int dstx,int dsty)
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
    if(ptr)
    {
    Queue *myq = BFS(tmp,visited,srcy,srcx,dsty,dstx);
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
Vector2D travel_location(Uint8 id,float x, float y)
{
	PathFind *ptr = NULL;
	int realx = x / 60;
	int realy = y / 60;
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
    Queue *next = ptr->path;
    if(!next)
    {
    return vector2d(0,0);
    } 
    Vector2D *local = front(next);
    if(!isEmpty(next) && local->x==realx && local->y==realy)
    {
    pop(next);
    local = front(next);
    }
    if(isEmpty(next)) 
    {
    	killqueue(ptr->path);
    	ptr->path = NULL;
    	return vector2d(0,0);
    }
    slog("x:%f,y:%f",local->x,local->y);
    slog("LOCALx:%f,y:%f",realx,realy);
    calcx = local->x * 60 + 30;
    calcy = local->y * 60 + 30;
    realx = realx * 60 + 30;
    realy = realy * 60 + 30;
    slog("x:%f,y:%f",calcx,calcy);
    Vector2D res = vector2d(calcx-realx,calcy-realy);
    slog("RESx:%f,y:%f",res.x,res.y);
    vector2d_set_magnitude(&res,5);
    return res;
}




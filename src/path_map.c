#include "simple_logger.h"
#include "simple_json.h"
#include "path_map.h"
Path_Map *Path_Map_new()
{
	Path_Map *map;
	map = (Path_Map *)malloc(sizeof(Path_Map));
	if(!map)
	{
		slog("failed to allocate memory for path map");
		return NULL;
	}
	memset(map,0,sizeof(Path_Map));
	return map;
}
Path_Map *Path_Map_load(char *filename)
{
	int i,r,c,e;
	SJson *json,*rows,*columns,*element;
	int Path_Map_Height,Path_Map_Width,granularity;
	Path_Map *map;
	
	
	if(!filename) return NULL;
	json = sj_load(filename);
	if(!json) return NULL;
	map = Path_Map_new();
	element = sj_object_get_value(json,"granularity");
	if(element)
	{
		sj_get_integer_value(element,&granularity);
	}
	else
		granularity = 1;
	rows = sj_object_get_value(json,"tile_map");
	Path_Map_Height = sj_array_get_count(rows);
	if(!Path_Map_Height)
	{
		slog("Empty Data");
		sj_free(json);
		return NULL;
	}
	columns = sj_array_get_nth(rows,0);
	Path_Map_Width = sj_array_get_count(columns);
	map->pathmap_length = Path_Map_Height;
	map->pathmap_width = Path_Map_Width;
	map->path = (int*)gfc_allocate_array(sizeof(int),Path_Map_Width * Path_Map_Height*granularity * granularity);
	if(!map->path)
	{
		sj_free(json);
		return map;
	}
	for (i = 0, r = 0; r < Path_Map_Height;r++)
    {
        columns = sj_array_get_nth(rows,r);
        for(int m = 0; m < granularity;m++)
        {
		    if (!columns)continue;
		    for (c = 0; c < Path_Map_Width;c++)
		    {
		    	for(int k = 0; k < granularity;k++)
		    	{
		        element = sj_array_get_nth(columns,c);
		        if (!element)continue;
		        sj_get_integer_value(element,&e);
		        map->path[i++] = e;
		        }

		    }
        }
    }
    map->pathmap_length = Path_Map_Height*granularity;
	map->pathmap_width = Path_Map_Width*granularity;
    sj_free(json);
    return map;
}
void Path_Map_free(Path_Map *map)
{
	if(!map) return;
	free(map->path);
	free(map);
}
void Path_Map_debug(Path_Map *map)
{
	int i;
	if(!map) return;
	if(!map->path) return;
	int iter = (map->pathmap_length) * map->pathmap_width;
	printf("%d:%d\n\n",map->pathmap_length,map->pathmap_width);
	for (i = 0; i < iter;i++)
	{
	if(i!=0 && i%(map->pathmap_width)==0)
	{
		printf("\n");
	}
	printf("%2d ",map->path[i]);
	
	}
	printf("\n");
	return;
}

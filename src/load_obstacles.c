#include "load_obstacles.h"
#include "simple_logger.h"
void load_obstacles(TileMap *t)
{
	if(!t) return;
		Uint32 cols;
		Vector2D res;
		int scale;
		float x,y;
		Uint32 *path;
		cols = t->tilemap_width;
		path = t->tilemap;
		scale = 1200/cols;
		for(int i = 0; i < t->tilemap_count;i++)
		{
			x = i % cols *scale;
			y = i / cols *scale;
			res = vector2d(x,y);
			switch(path[i])
			{
				case 2:
					new_fast_zone(res,scale);
					break;
				case 3:
					new_slow_zone(res,scale);
					break;
				case 4:
					new_heal_zone(res,scale);
					break;
				case 5:
					new_damage_zone(res,scale);
					break;
			}
		}
}

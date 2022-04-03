
#include "simple_logger.h"
#include "entity.h"
#include "game_instance.h"
typedef struct
{
    Uint32 max_entities;            /**<how many entities exist*/
    Entity  *entity_list;           /**<a big ole list of entities*/
}EntityManager;

static EntityManager entity_manager = {0};


void entity_manager_close()
{
    entity_manager_clear(); // clear all entities first
    if(entity_manager.entity_list != NULL)
    {
        free(entity_manager.entity_list);
    }
    slog("entity manager closed");    
}

void entity_manager_init(Uint32 max_entities)
{
	gf2d_sprite_load_image("images/blood.png");
	gf2d_sprite_load_image("images/white.png");
    if (max_entities == 0)
    {
        slog("cannot allocate memory for zero entities!");
        return;
    }
    if (entity_manager.entity_list != NULL)
    {
        slog("entity manager already initialized");
        return;
    }
    entity_manager.max_entities = max_entities;
    entity_manager.entity_list = gfc_allocate_array(sizeof(Entity),max_entities);
    atexit(entity_manager_close);
    slog("entity manager initialized");
}

void entity_manager_clear()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_free(&entity_manager.entity_list[i]);
    }
}

Entity *entity_new()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)
        {
            //GOT ONE!
            entity_manager.entity_list[i]._inuse = 1;
            entity_manager.entity_list[i].draw_scale.x = 1;
            entity_manager.entity_list[i].draw_scale.y = 1;
            return &entity_manager.entity_list[i];
        }
    }
    slog("out of entities");
    return NULL;
}

void entity_think(Entity *ent)
{
    if (!ent)return;
    //generic upkeep
    
    if (ent->think)
    {
        ent->think(ent);
    }
    
    vector2d_add(ent->position,ent->position,ent->velocity);
}

void entity_manager_think_all()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_think(&entity_manager.entity_list[i]);
    }
}

void entity_manager_draw_all()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_draw(&entity_manager.entity_list[i]);
    }
}

void entity_draw(Entity *entity)
{
    Vector2D drawPosition;
    if (entity == NULL)
    {
        slog("null pointer provided, nothing to do!");
        return;
    }
    if(entity->draw)
    {
		entity->draw(entity);
		return;
    }
    if (entity->sprite == NULL)return;// nothing to draw
    vector2d_add(drawPosition,entity->position,entity->draw_offset);
    gf2d_sprite_draw(
        entity->sprite,        
        drawPosition,
        &entity->draw_scale,
        NULL,
        &entity->rotation,
        NULL,
        NULL,
        (Uint32)entity->frame);
}

void entity_free(Entity *entity)
{
    if (entity == NULL)
    {
        slog("null pointer provided, nothing to do!");
        return;
    }
    if (entity->sprite != NULL)
    {
        gf2d_sprite_free(entity->sprite);
    }
    if(!entity->build)
    {
    	game_instance *ptr = get_game(entity->team);
    	if(ptr && ptr->units)
    		unit_remove(ptr,entity);
    }
    if(entity->path)
    {
        path_free(entity->path);
    }
    if(entity->actor)
    {
//         if(entity->actor->al)
//         gem_actor_free(entity->actor);
        free(entity->actor);
    }
    memset(entity,0,sizeof(Entity));
}

List* entity_click(int mx, int my,int init_x,int init_y)
{
    List* touched = gfc_list_new();
    ShapeRect tmp = shape_rect_from_vector4d(vector4d(MIN(mx,init_x),MIN(my,init_y),MAX(mx,init_x)-MIN(mx,init_x),MAX(my,init_y)-MIN(my,init_y)));
    gf2d_draw_rect(shape_rect_to_sdl_rect(tmp),vector4d(0,255,0,255));
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        if ((&entity_manager.entity_list[i].bounding)==NULL)continue;
        if(shape_rect_collision(tmp,entity_manager.entity_list[i].bounding))
        {
        	touched = gfc_list_append(touched,&entity_manager.entity_list[i]);
        }
    }
    return touched;
}
List* resources()
{
	List *nodes = gfc_list_new(4);
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        Entity* other = &entity_manager.entity_list[i];
        if(other->harvest == 1)
        {
        	gfc_list_append(nodes,other);
        }
    }
    return nodes;
}
Entity* heal(Entity *self)
{
	float dist = 9999;
	float calculated = 0;
	Entity* heal = NULL;
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        Entity *node = &entity_manager.entity_list[i];
        if(node->type == 3)
        {
			calculated = vector2d_magnitude_between(self->position,node->position);
			if(dist > calculated)
			{
				heal = node;
				dist = calculated; 
			}
		}
    }
    return heal;
}
List* touch(Entity *self)
{
	List *nodes = gfc_list_new(4);
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        Entity* other = &entity_manager.entity_list[i];
        if(shape_rect_collision(self->bounding,other->bounding) && other!=self)
        {
        	gfc_list_append(nodes,other);
        }
    }
    return nodes;
}
Entity* overlap(Entity *ent)
{
    int i;
    Entity *ret = NULL;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
    	
        if (!entity_manager.entity_list[i]._inuse)continue;
        if ((&entity_manager.entity_list[i].range)==NULL)continue;
        Entity* other = &entity_manager.entity_list[i];
        if(ent->team != other->team && ent != other && shape_rect_circle_collision(ent->range,other->bounding))
        {
			ret = &entity_manager.entity_list[i]; 
        	if(other->harvest==0)
        	{
        		return &entity_manager.entity_list[i];
        	}
        }
    }
    return ret;
}
void Entity_draw_hp(Entity *self)
{
	Vector2D drawPosition;
	Vector4D opacity = vector4d(255,255,255,255);
	if(self->actor)
    vector2d_add(drawPosition,self->position,self->actor->al->drawOffset);
    else
    vector2d_add(drawPosition,self->position,self->draw_offset);
    Vector2D scaler = vector2d(1,1);
    if(self->build)
    {
    	scaler.x = 4;
    	scaler.y = 3;
    	drawPosition.x+=20;
    	drawPosition.y-=20;
    }
    drawPosition.y -= 5;
	Sprite *sprite1 = NULL;
    Sprite *sprite2 = NULL;
    float hp_off = self->health/(float)self->max_health;
    Vector2D vec = vector2d(scaler.x*hp_off,scaler.y);
    sprite1 = gf2d_sprite_load_image("images/white.png");
	sprite2 = gf2d_sprite_load_image("images/blood.png");
	gf2d_sprite_draw(
        sprite1,        
        drawPosition,
        &scaler,
        NULL,
        NULL,
        NULL,
        &opacity,
        0);
	gf2d_sprite_draw(
        sprite2,        
        drawPosition,
        &vec,
        NULL,
        NULL,
        NULL,
        &opacity,
        0);
}
// eof

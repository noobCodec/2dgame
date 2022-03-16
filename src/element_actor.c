#include <stdlib.h>
#include <stdio.h>
#include "element_actor.h"
#include "simple_logger.h"
#include "gf2d_draw.h"
void element_actor_draw(Element *element, Vector2D offset)
{
	ActorElement* actor;
	Vector2D position;
	if(!element) return;
	actor = (ActorElement*)element->data;
	if(!actor) return;
	
	vector2d_add(position,offset,vector2d(element->bounds.x,element->bounds.y));
	gem_actor_draw(
		&actor->actor,
		position,
		NULL,
		NULL,
		NULL,
		NULL
	);
}
List *element_actor_update(Element *element,Vector2D offset)
{
	ActorElement *actor;
	if(!element) return NULL;
	actor = (ActorElement *)element->data;
	if(!actor) return NULL;
	gem_actor_next_frame(&actor->actor);
	return NULL;
}
void element_actor_free(Element *element)
{
    ActorElement *actor;
    if (!element)return;
    actor = (ActorElement*)element->data;
    if (actor != NULL)
    {
        free(actor);
    }
}

ActorElement *element_actor_new()
{
    ActorElement *actor;
    actor = (ActorElement *)malloc(sizeof(ActorElement));
    if (!actor)
    {
        slog("failed to allocate memory for actor");
        return NULL;
    }
    memset(actor,0,sizeof(ActorElement));
    return actor;
}


ActorElement *element_actor_new_full(char *actorFile)
{
    ActorElement *ae;
    ae = element_actor_new();
    if (!ae)
    {
        return NULL;
    }
    gem_actor_load(&ae->actor,actorFile);
    if(!ae->actor.al)
    	return NULL;
    vector2d_copy(ae->scale,ae->actor.al->scale);
    return ae;
}

Actor *element_actor_get_actor(Element *e)
{
    ActorElement *ae;
    if (!e)return NULL;
    ae = (ActorElement *)e->data;
    if (!ae)return NULL;
    return &ae->actor;
}

void element_make_actor(Element *e,ActorElement *actor)
{
    if (!e)return;
    e->data = (void*)actor;
    e->type = ET_ACTOR;
    e->draw = element_actor_draw;
    e->update = element_actor_update;
    e->free_data = element_actor_free;
}

void element_load_actor_from_config(Element *e,SJson *json)
{
    SJson *value;
    const char *buffer;
    if ((!e) || (!json))
    {
        slog("call missing parameters");
        return;
    }
    value = sj_object_get_value(json,"actor");
    buffer = sj_get_string_value(value);
    element_make_actor(e,element_actor_new_full((char *)buffer));
}

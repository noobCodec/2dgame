#ifndef __ELEMENT_ACTOR_H__
#define __ELEMENT_ACTOR_H__

#include "elements.h"
#include "actor.h"

typedef struct
{
	Actor actor;
	Vector2D scale;
}ActorElement;

ActorElement *element_actor_new();

ActorElement *element_actor_new_full(char *actorFile);

Actor *element_actor_get_actor(Element *e);

void element_make_actor(Element *e, ActorElement *actor);

void element_load_actor_from_config(Element *e, SJson *json);

#endif

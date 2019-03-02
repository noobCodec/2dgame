#include "player.h"
#include "simple_logger.h"
#include "camera.h"
#include "level.h"
#include "gf2d_input.h"
#include "entity_common.h"
#include "particle_effects.h"
#include "gui.h"

static Entity *_player = NULL;

void player_draw(Entity *self);
void player_activate(Entity *self,Entity *activator);
void player_think(Entity *self);
void player_update(Entity *self);
int  player_touch(Entity *self,Entity *other);
int  player_damage(Entity *self,int amount, Entity *source);
void player_die(Entity *self);

typedef struct
{
    int baseSpeed;
    int maxSpeed;
    float baseAcceleration;
    float shields,shieldMax;
    float charge,chargeMax;
    float energy,energyMax;
    int capacitors,capacitorsMax;
    int attackType;
}PlayerData;


static PlayerData playerData = {
    3,50,3.6,
    0,100,
    0,100,
    0,100,
    0,0};

Entity *player_get()
{
    return _player;
}

void player_set_position(Vector2D position)
{
    if (!_player)
    {
        slog("no player loaded");
        return;
    }
    vector2d_copy(_player->position,position);
    vector2d_copy(_player->body.position,position);
}

Entity *player_spawn(Vector2D position,SJson *args)
{
    if (_player != NULL)
    {
        vector2d_copy(_player->position,position);
        level_add_entity(_player);
        return NULL;
    }
    return player_new(position);
}

Entity *player_new(Vector2D position)
{
    Entity *self;
    self = gf2d_entity_new();
    if (!self)return NULL;
    
    gf2d_line_cpy(self->name,"player");
    self->parent = NULL;
    
    self->shape = gf2d_shape_rect(-16, -16, 30, 60);
    gf2d_body_set(
        &self->body,
        "player",
        1,
        WORLD_LAYER,
        0,
        1,
        position,
        vector2d(0,0),
        10,
        1,
        0,
        &self->shape,
        self,
        NULL);

    gf2d_actor_load(&self->actor,"actors/player.actor");
    gf2d_actor_set_action(&self->actor,"idle");

    self->sound[0] = gf2d_sound_load("sounds/jump_10.wav",1,-1);

    
    vector2d_copy(self->position,position);
    
    vector2d_copy(self->scale,self->actor.al->scale);
    vector2d_set(self->scaleCenter,64,64);
    vector3d_set(self->rotation,64,64,0);
    vector2d_set(self->flip,0,0);
    vector2d_set(self->facing,1,0);
    
    self->think = player_think;
    self->draw = player_draw;
    self->update = player_update;
    self->touch = player_touch;
    self->damage = player_damage;
    self->die = player_die;
    self->free = level_remove_entity;
    self->activate = player_activate;

    self->data = (void*)&playerData;
    
    self->health = self->maxHealth = 100;
    _player = self;
    level_add_entity(self);
    return self;
}

void player_draw(Entity *self)
{
    //additional player drawings can go here
}

void player_activation_check(Entity *self)
{
    entity_activate(self);
}

void player_think(Entity *self)
{
    switch (self->state)
    {
        case ES_Idle:
            if (gf2d_input_command_down("walkleft"))
            {
                self->flip.x = 1;
                self->facing.x = -1;
                if (!entity_wall_check(self,vector2d(-2,0)))
                {   
                    self->velocity.x -= 1.25;
                }
            }
            if (gf2d_input_command_down("walkright"))
            {
                self->flip.x = 0;
                self->facing.x = 1;
                if (!entity_wall_check(self,vector2d(2,0)))
                {
                    self->velocity.x += 1.25;
                }
            }
            if (((gf2d_input_command_pressed("jump"))&&(self->grounded))&&(self->jumpcool <= 0))
            {
                self->velocity.y -= 10;
                self->jumpcool = gf2d_actor_get_frames_remaining(&self->actor);
                gf2d_sound_play(self->sound[0],0,1,-1,-1);
                gf2d_actor_set_action(&self->actor,"jump");
            }
            if (gf2d_input_command_down("activate"))
            {
                slog("activating");
                self->cooldown  = 16;
                self->state = ES_Cooldown;
                player_activation_check(self);
            }
            if (gf2d_input_command_released("melee"))
            {
                gf2d_actor_set_action(&self->actor,"hack");
                self->cooldown = gf2d_actor_get_frames_remaining(&self->actor);
                self->state = ES_Attacking;
                self->attack = 1;
            }
            if (gf2d_input_command_released("shoot"))
            {
                gf2d_actor_set_action(&self->actor,"shoot");
                self->cooldown = gf2d_actor_get_frames_remaining(&self->actor);
                self->state = ES_Attacking;
                self->attack = 2;
            }
            break;
        default:
            break;
    }
}

void player_melee(Entity *self)
{
    Shape s;
    int i,count;
    Entity *other;
    Collision *c;
    List *collisionList = NULL;
    s = gf2d_shape_rect(self->position.x + (self->flip.x * -48) + 16, self->position.y, 16, 32);
    collisionList = entity_get_clipped_entities(self,s, MONSTER_LAYER, 0);
    count = gf2d_list_get_count(collisionList);
    slog("hit %i targets",count);
    for (i = 0; i < count;i++)
    {
        c = (Collision*)gf2d_list_get_nth(collisionList,i);
        if (!c)continue;
        if (!c->body)continue;
        if (!c->body->data)continue;
        other = c->body->data;
        if (other->damage)other->damage(other,1,self);//TODO: make this based on weapon / player stats
    }
    gf2d_collision_list_free(collisionList);
}

void player_update(Entity *self)
{
    Vector2D camPosition = {0,0};
    if (!self)return;
    
    if (self->maxHealth)gui_set_health(self->health/self->maxHealth);
    camera_set_position(camPosition);
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    //world clipping

    
    // walk dampening
    if (self->velocity.x)
    {
        self->velocity.x *= 0.8;
        if (fabs(self->velocity.x) < 1)self->velocity.x = 0;
    }
    entity_world_snap(self);    // error correction for collision system
    entity_apply_gravity(self);
    switch (self->state)
    {
        case ES_Idle:
            break;
        case ES_Seeking:
            break;
        case ES_Attacking:
            if (gf2d_actor_get_frames_remaining(&self->actor) == 2)
            {
                if (self->attack == 1)
                {
                    player_melee(self);
                }
                else
                {
                    particle_spray(self->position, vector2d(gf2d_crandom(),gf2d_crandom()),gf2d_color8(255,255,255,255), 100);
                }
            }
        case ES_Jumping:
        case ES_Leaving:
        case ES_Charging:
        case ES_Pain:
        case ES_Cooldown:
            self->cooldown--;
            if (self->cooldown <= 0)
            {
                self->state = ES_Idle;
                gf2d_actor_set_action(&self->actor,"idle");
            }
            break;
        case ES_Walking:
        case ES_Dying:
            return;
        case ES_Dead:
            return;
    }
    
}

int player_touch(Entity *self,Entity *other)
{
    return 0;// player does not touch
}

int player_damage(Entity *self,int amount, Entity *source)
{
    switch(self->state)
    {
        case ES_Dying:
        case ES_Dead:
        case ES_Leaving:
        case ES_Pain:
            slog("player cannot take damage now");
            return 0;
        default:
            break;
    }
    slog("player taking %i damage!",amount);
    self->health -= amount;
    //play pain sound, set pain state
    if (self->health <= 0)
    {
        self->health = 0;
        self->die(self);
    }
    return amount;//todo factor in shields
}

void player_die(Entity *self)
{
    
}

void player_activate(Entity *self,Entity *activator)
{
    
}

/*eol@eof*/
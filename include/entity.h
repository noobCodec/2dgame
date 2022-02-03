#include "gf2d_sprite.h"

typedef struct Entity_s
{
    Bool       _inuse;
    Vector2D    position;
    Vector2D    velocity;
    Vector3D    rotation; //(x,y) = rotation center, z = degrees of rotation
    Sprite     *sprite; 
    float       frame;
    float       frameRate;
    int         frameCount;
    void      (*update)(struct Entity_s *self);
    void      (*think)(struct Entity_s *self);
    void      (*draw)(struct Entity_s *self);
    void      (*free)(struct Entity_s *self);
    void       *data;
}Entity;

/**
 * @brief get a pointer to a new entity
 * @return NULL on out of memory or error, a pointer to a blank entity otherwise
 */
Entity *entity_new();

/**
 * @brief initialize the entity resource manager
 * @param maxEnts upper bound of maximum concurrent entities to be supported
 * @note must be called before creating a new entity
 */
void entity_manager_init(Uint32 maxEnts);


/**
 * @brief free all entities in the system and destroy entity manager
 */
void entity_manager_free();

/**
 * @brief free a previously allocated entity
 * @param self a pointer to the entity to free
 */
void entity_free(Entity *self);

/**
 * @brief update every active entity
 */
void entity_update_all();
/**
 * @brief draww every active entity
 */
void entity_draw_all();

void entity_draw(Entity *ent);


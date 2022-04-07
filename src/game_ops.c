#include "game_ops.h"
#include "simple_json.h"
#include "simple_logger.h"
#include "mage_ent.h"
#include "rogue.h"
#include "ranger.h"
#include "goblin.h"
#include "warrior.h"
#include "game_instance.h"
#include "building.h"
#include "crystal.h"
void save_game()
{
    SJson *ptr = sj_object_new();
    SJson *str = NULL;
    SJson *tmp = NULL;
    SJson *arr = NULL;
    game_instance *g1 = get_game(1);
    game_instance *g2 = get_game(0);
    str = sj_array_new();
    for(int i = 0; i<gfc_list_get_count(g1->units);i++)
    {
        tmp = sj_object_new();
        Entity *ent = gfc_list_get_nth(g1->units,i);
        slog(ent->actor->al->filename);
        if(!gfc_line_cmp(ent->actor->al->filename,"level/mage.json"))
        {
            sj_object_insert(tmp,"type",sj_new_str("mage"));
        }
        else if(!gfc_line_cmp(ent->actor->al->filename,"level/warrior.json"))
        {
            sj_object_insert(tmp,"type",sj_new_str("warrior"));
        }
        else if(!gfc_line_cmp(ent->actor->al->filename,"level/goblin.json"))
        {
            sj_object_insert(tmp,"type",sj_new_str("goblin"));
        }
        else if(!gfc_line_cmp(ent->actor->al->filename,"level/ranger.json"))
        {
            sj_object_insert(tmp,"type",sj_new_str("ranger"));
        }
        else if(!gfc_line_cmp(ent->actor->al->filename,"level/rogue.json"))
        {
            sj_object_insert(tmp,"type",sj_new_str("rogue"));
        }
        sj_object_insert(tmp,"team",sj_new_int(ent->team));
        sj_object_insert(tmp,"health",sj_new_int(ent->health));
        arr = sj_array_new();
        sj_array_append(arr,sj_new_int(ent->position.x));
        sj_array_append(arr,sj_new_int(ent->position.y));
        sj_object_insert(tmp,"position",arr);
        sj_object_insert(tmp,"range",sj_new_int(ent->range.r));
        sj_array_append(str,tmp);
    }
    for(int i = 0; i<gfc_list_get_count(g2->units);i++)
    {
        tmp = sj_object_new();
        Entity *ent = gfc_list_get_nth(g2->units,i);
        //slog(ent->actor->al->filename);
        if(!gfc_line_cmp(ent->actor->al->filename,"level/mage.json"))
        {
            sj_object_insert(tmp,"type",sj_new_str("mage"));
        }
        else if(!gfc_line_cmp(ent->actor->al->filename,"level/warrior.json"))
        {
            sj_object_insert(tmp,"type",sj_new_str("warrior"));
        }
        else if(!gfc_line_cmp(ent->actor->al->filename,"level/goblin.json"))
        {
            sj_object_insert(tmp,"type",sj_new_str("goblin"));
        }
        else if(!gfc_line_cmp(ent->actor->al->filename,"level/ranger.json"))
        {
            sj_object_insert(tmp,"type",sj_new_str("ranger"));
        }
        else if(!gfc_line_cmp(ent->actor->al->filename,"level/rogue.json"))
        {
            sj_object_insert(tmp,"type",sj_new_str("rogue"));
        }
        sj_object_insert(tmp,"team",sj_new_int(ent->team));
        sj_object_insert(tmp,"health",sj_new_int(ent->health));
        arr = sj_array_new();
        sj_array_append(arr,sj_new_int(ent->position.x));
        sj_array_append(arr,sj_new_int(ent->position.y));
        sj_object_insert(tmp,"position",arr);
        sj_object_insert(tmp,"range",sj_new_int(ent->range.r));
        sj_array_append(str,tmp);
    }
    sj_object_insert(ptr,"entities",str);
    str = sj_array_new();
    List *res = resources();
    for(int i = 0; i<gfc_list_get_count(res);i++)
    {
        tmp = sj_object_new();
        Entity *ent = gfc_list_get_nth(res,i);
        sj_object_insert(tmp,"health",sj_new_int(ent->health));
        arr = sj_array_new();
        sj_array_append(arr,sj_new_int(ent->position.x));
        sj_array_append(arr,sj_new_int(ent->position.y));
        sj_object_insert(tmp,"position",arr);
        sj_array_append(str,tmp);
    }
    sj_object_insert(ptr,"crystals",str);
    str = sj_array_new();
    for(int i = 0; i<gfc_list_get_count(g2->buildings);i++)
    {
        tmp = sj_object_new();
        Entity *ent = gfc_list_get_nth(g2->buildings,i);
        sj_object_insert(tmp,"team",sj_new_int(ent->team));
        sj_object_insert(tmp,"health",sj_new_int(ent->health));
        arr = sj_array_new();
        sj_array_append(arr,sj_new_int(ent->position.x));
        sj_array_append(arr,sj_new_int(ent->position.y));
        sj_object_insert(tmp,"position",arr);
        sj_array_append(str,tmp);
    }
    for(int i = 0; i<gfc_list_get_count(g1->buildings);i++)
    {
        tmp = sj_object_new();
        Entity *ent = gfc_list_get_nth(g1->buildings,i);
        sj_object_insert(tmp,"team",sj_new_int(ent->team));
        sj_object_insert(tmp,"health",sj_new_int(ent->health));
        arr = sj_array_new();
        sj_array_append(arr,sj_new_int(ent->position.x));
        sj_array_append(arr,sj_new_int(ent->position.y));
        sj_object_insert(tmp,"position",arr);
        sj_array_append(str,tmp);
    }
    sj_object_insert(ptr,"player_resources",sj_new_int(g2->resources));
    sj_object_insert(ptr,"enemy_resources",sj_new_int(g1->resources));
    sj_object_insert(ptr,"buildings",str);

    //sj_echo(ptr);
    sj_save(ptr,"saves/save.json");
    sj_free(ptr);


}

void load_game(const char *filename)
{
    int l,j,k;
    SJson *file = sj_load(filename);
    SJson *tmp = NULL;
    if(file == NULL){
        slog("no file provided");
        return;
    }
    SJson *ents = sj_object_get_value(file,"entities");
    game_instance *g1 = game_new_no_default(1); // Player - 0
    game_instance *g2 = game_new_no_default(0); // OPPONENT - 1
    game_iterate();
    for(int i =  0; i < sj_array_get_count(ents); i++)
    {
        SJson *entity = sj_array_get_nth(ents,i);
        Entity *actual_ent = NULL;
        //sj_echo(entity);
        tmp = sj_object_get_value(entity,"position");
        sj_get_integer_value(sj_array_get_nth(tmp,0),&l);
        sj_get_integer_value(sj_array_get_nth(tmp,1),&j);
        sj_get_integer_value(sj_object_get_value(entity,"range"),&k);
        if(!gfc_line_cmp(sj_get_string_value(sj_object_get_value(entity,"type")),"mage"))
        {
            actual_ent = mage_ent_new(vector2d(l,j),k);
        }
        else if(!gfc_line_cmp(sj_get_string_value(sj_object_get_value(entity,"type")),"warrior"))
        {
            actual_ent = warrior_ent_new(vector2d(l,j),k);
        }
        else if(!gfc_line_cmp(sj_get_string_value(sj_object_get_value(entity,"type")),"goblin"))
        {
            actual_ent = goblin_ent_new(vector2d(l,j),k);
        }
        else if(!gfc_line_cmp(sj_get_string_value(sj_object_get_value(entity,"type")),"ranger"))
        {
            actual_ent = ranger_ent_new(vector2d(l,j),k);
        }
        else if(!gfc_line_cmp(sj_get_string_value(sj_object_get_value(entity,"type")),"rogue"))
        {
            actual_ent = rogue_ent_new(vector2d(l,j),k);
        }
        sj_get_integer_value(sj_object_get_value(entity,"health"),&k);
        actual_ent->health = k;
        sj_get_integer_value(sj_object_get_value(entity,"team"),&k);
        actual_ent->team = k;
        if(k == 0)
            unit_append(g1,actual_ent);
        else
            unit_append(g2,actual_ent);

    }
    ents = sj_object_get_value(file,"buildings");
    for(int i =  0; i < sj_array_get_count(ents); i++)
    {
        SJson *entity = sj_array_get_nth(ents,i);
        Entity *actual_ent = NULL;
        //sj_echo(entity);
        tmp = sj_object_get_value(entity,"position");
        sj_get_integer_value(sj_array_get_nth(tmp,0),&l);
        sj_get_integer_value(sj_array_get_nth(tmp,1),&j);
        actual_ent = building_ent_new(vector2d(l,j));
        sj_get_integer_value(sj_object_get_value(entity,"health"),&k);
        actual_ent->health = k;
        sj_get_integer_value(sj_object_get_value(entity,"team"),&k);
        actual_ent->team = k;
        actual_ent->inflict = 1;
        if(k == 0)
        {
            building_append(g1,actual_ent);
        }
        else
        {
            building_append(g2,actual_ent);
        }

    }
    ents = sj_object_get_value(file,"crystals");
    for(int i =  0; i < sj_array_get_count(ents); i++)
    {
        SJson *entity = sj_array_get_nth(ents,i);
        Entity *actual_ent = NULL;
        //sj_echo(entity);
        tmp = sj_object_get_value(entity,"position");
        sj_get_integer_value(sj_array_get_nth(tmp,0),&l);
        sj_get_integer_value(sj_array_get_nth(tmp,1),&j);
        actual_ent = crystal_ent_new(vector2d(l,j));
        sj_get_integer_value(sj_object_get_value(entity,"health"),&k);
        actual_ent->health = k;
    }
    sj_get_integer_value(sj_object_get_value(file,"player_resources"),&l);
    g1->resources = l;
    sj_get_integer_value(sj_object_get_value(file,"enemy_resources"),&l);
    g2->resources = l;
    opponent_init(g2,g1);
    sj_free(file);


}

void quit_game();

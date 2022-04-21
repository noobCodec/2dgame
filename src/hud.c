#include "hud.h"
#include "element_button.h"
#include "simple_logger.h"
static Window *my_hud = NULL;
void update_hud(Window *win,List *updateList)
{
    int i,count;
    Element *e;
    ButtonElement *here;
    LabelElement *j;
    if (!win)return;
    if (!updateList)return;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList,i);
//         if (!e)continue;
//         if(e->type == ET_BUTTON)
//         {
//             here = (ButtonElement *)e;
//             j = (LabelElement *)here->label;
//             here->label = NULL;
//             here->style = ES_HIDDEN;
//             slog("updated");
//         }
        List *stuff = get_inputs();
        if(stuff)
        {
            Entity *ent = gfc_list_get_nth(stuff,e->index);
            slog("%d",ent==NULL);
            stuff = gfc_list_new();
            gfc_list_append(stuff,ent);
            set_inputs(stuff);
        }
        block(1);
    }
    return;
}
SJson *make_vec4d(Vector4D x)
{
    SJson *arr = sj_array_new();
    sj_array_append(arr,sj_new_int(x.x));
    sj_array_append(arr,sj_new_int(x.y));
    sj_array_append(arr,sj_new_int(x.z));
    sj_array_append(arr,sj_new_int(x.w));
    return arr;
}
SJson *make_vec3d(Vector3D x)
{
    SJson *arr = sj_array_new();
    sj_array_append(arr,sj_new_int(x.x));
    sj_array_append(arr,sj_new_int(x.y));
    sj_array_append(arr,sj_new_int(x.z));
    return arr;
}
SJson *make_vec2d(Vector2D x)
{
    SJson *arr = sj_array_new();
    sj_array_append(arr,sj_new_int(x.x));
    sj_array_append(arr,sj_new_int(x.y));
    return arr;
}
void update_hud_elements(List *ents)
{
    return;
    SJson *json = NULL;
    char s[25];
    int i = 0;
    SJson *window,*elements,*list_of_ents,*newarr1,*newarr2,*value,*label,*actor;
    json = sj_load("level/control.json");
    window = sj_object_get_value(json,"window");
    elements = sj_object_get_value(window,"elements");
    list_of_ents = sj_object_get_value(sj_array_get_nth(elements,0),"elements");
    sj_object_delete_key(sj_array_get_nth(list_of_ents,1),"elements");
    sj_object_delete_key(sj_array_get_nth(list_of_ents,2),"elements");
    newarr1 = sj_array_new();
    newarr2 = sj_array_new();
    for(int j = 0; j < gfc_list_get_count(ents);j++)
    {
        Entity *ent = gfc_list_get_nth(ents,j);
        if(ent->build != 0 || !ent->damage) continue;
        value = sj_object_new();
        sj_object_insert(value,"type",sj_new_str("button"));
        sj_object_insert(value,"name",sj_new_str("unit"));
        sj_object_insert(value,"id",sj_new_int(i++));
        sj_object_insert(value,"bounds",make_vec4d(vector4d(0,0,55,45)));
        sj_object_insert(value,"color",make_vec4d(vector4d(255,255,255,255)));
        sj_object_insert(value,"highColor",make_vec4d(vector4d(255,255,255,255)));
        sj_object_insert(value,"pressColor",make_vec4d(vector4d(255,255,255,255)));
        label = sj_object_new();
        sj_object_insert(label,"type",sj_new_str("label"));
        sj_object_insert(label,"name",sj_new_str("unit"));
        sj_object_insert(label,"id",sj_new_int(900));
        sj_object_insert(label,"bounds",make_vec4d(vector4d(-30,20,105,45)));
        sj_object_insert(label,"color",make_vec4d(vector4d(255,255,255,255)));
        switch(ent->id)
        {
            case 1:
                sj_object_insert(label,"text",sj_new_str("Mage"));
                break;
            case 2:
                sj_object_insert(label,"text",sj_new_str("Rogue"));
                break;
            case 3:
                sj_object_insert(label,"text",sj_new_str("Ranger"));
                break;
            case 4:
                sj_object_insert(label,"text",sj_new_str("Goblin"));
                break;
            case 5:
                sj_object_insert(label,"text",sj_new_str("Warrior"));
                break;
        }
        sj_object_insert(label,"text",sj_new_str("Mage"));
        sj_object_insert(label,"align",sj_new_str("middle"));
        sj_object_insert(label,"justify",sj_new_str("center"));
        sj_object_insert(label,"style",sj_new_str("H2"));
        actor = sj_object_new();
        sj_object_insert(actor,"type",sj_new_str("actor"));
        sj_object_insert(actor,"id",sj_new_int(900));
        sj_object_insert(actor,"name",sj_new_str("joe_mama"));
        sj_object_insert(actor,"bounds",make_vec4d(vector4d(0,0,168,64)));
        sj_object_insert(actor,"color",make_vec4d(vector4d(255,255,255,255)));
        switch(ent->id)
        {
            case 1:
                sj_object_insert(actor,"actor",sj_new_str("json/hud/mage_hud.json"));
                break;
            case 2:
                sj_object_insert(actor,"actor",sj_new_str("json/hud/rogue_hud.json"));
                break;
            case 3:
                sj_object_insert(actor,"actor",sj_new_str("json/hud/ranger_hud.json"));
                break;
            case 4:
                sj_object_insert(actor,"actor",sj_new_str("json/hud/goblin_hud.json"));
                break;
            case 5:
                sj_object_insert(actor,"actor",sj_new_str("json/hud/warrior_hud.json"));
                break;
        }
        sj_object_insert(value,"label",label);
        sj_object_insert(value,"actor",actor);
        sj_array_append(newarr1,value);

        label = sj_object_new();
        sj_object_insert(label,"type",sj_new_str("label"));
        sj_object_insert(label,"name",sj_new_str("generic"));
        sj_object_insert(label,"id",sj_new_int(900));
        sj_object_insert(label,"bounds",make_vec4d(vector4d(0,0,350,32)));
        sj_object_insert(label,"color",make_vec4d(vector4d(255,255,255,255)));
        sprintf(s, "HP: %d/%d", ent->health, ent->max_health);
        sj_object_insert(label,"text",sj_new_str(s));
        sj_object_insert(label,"justify",sj_new_str("left"));
        sj_object_insert(label,"style",sj_new_str("Normal"));

        sj_array_append(newarr2,label);

    }
    sj_object_insert(sj_array_get_nth(list_of_ents,1),"elements",newarr1);
    sj_object_insert(sj_array_get_nth(list_of_ents,2),"elements",newarr2);
    sj_save(json,"level/control.json");
    sj_free(json);
    window_free(my_hud);
    init_hud("level/control.json");
    return;



}

void open_building(Entity *bding)
{




}
void init_hud(const char *file)
{
    Window *win;
    SJson *json = NULL;
    json = sj_load(file);
    win = window_load_from_json(json);
    if (win)
    {
        win->update = update_hud;
    }
    win->active = 1;
    sj_free(json);
    my_hud = win;
}



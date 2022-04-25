#ifndef __HUD_H__
#define __HUD_H__

#include "entity.h"
#include "input.h"
#include "windows.h"

void init_hud(const char *file);

void update_hud(Window *win,List *updateList);

void update_hud_elements(List *ents);


void open_building(Entity *bding);

void draw_hud();

void apply_block(int i);

void close_building_hud();
#endif


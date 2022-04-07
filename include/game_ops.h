#ifndef __GAME_OPS_H__
#define __GAME_OPS_H__

#include "entity.h"
#include "opponent.h"
#include "game_instance.h"
#include "gfc_list.h"

void save_game();

void load_game(const char *filename);

void quit_game();


#endif

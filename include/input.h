#ifndef __INPUT_H__
#define __INPUT_H__

#include <SDL.h>
#include "entity.h"

List* check_inputs();

void set_inputs(List *entities);

List *get_inputs();

void block(int value);
#endif

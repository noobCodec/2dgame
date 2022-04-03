#include "game_ops.h"
#include "simple_json.h"
#include "simple_logger.h"

void save_game()
{
    SJson *ptr = sj_object_new();
    SJson *str = sj_new_int(22);
    sj_object_insert(ptr,"kek",str);
    sj_echo(ptr);
    sj_save(ptr,"saves/save.json");
//     sj_free(str);
    sj_free(ptr);


}


void quit_game();

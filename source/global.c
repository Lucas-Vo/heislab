#include <stdbool.h>

#include "global.h"
#include "fsm.h"

bool g_floor_panel[6]; /// [4 down, 3 down, 2 down, 3 up, 2 up, 1 up]
bool g_elevator_panel[4]; /// [1, 2, 3, 4]
fsm_state_t g_current_state;
query_t g_query[M_QUERY_LEN];
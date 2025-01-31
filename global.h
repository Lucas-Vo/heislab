#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdbool.h>
#include <stdint.h>

extern bool g_floor_panel[6]; /// [4 down, 3 down, 2 down, 3 up, 2 up, 1 up]
extern bool g_elevator_panel[4]; /// [1, 2, 3, 4]
extern fsm_state_t g_current_state;

#endif
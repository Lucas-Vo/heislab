#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdbool.h>
#include <stdint.h>
#include "fsm.h"

#define M_QUERY_LEN  10

#define M_FLOOR_BIT_MASK  0x07
#define M_DIRECTION_BIT_MASK  0x38
#define M_PRIORITY_BIT_MASK  0x40
#define M_ACTIVE_BIT_MASK 0x80

#define M_BUTTON_COUNT 4
#define M_FLOOR_COUNT 4

typedef uint8_t query_t; // 1 bit for active, 1 bit for priority, 1 bit for button type, bits for direction, 3 bits for floor

extern bool g_floor_panel[6]; /// [4 down, 3 down, 2 down, 3 up, 2 up, 1 up]
extern bool g_elevator_panel[4]; /// [1, 2, 3, 4]
extern fsm_state_t g_current_state;

#endif
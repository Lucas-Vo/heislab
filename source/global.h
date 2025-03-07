#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdbool.h>
#include <stdint.h>
#include "fsm.h"

#define M_QUERY_LEN  10

//Masks for getting correct information from the query info package
#define M_FLOOR_BIT_MASK  0x07
#define M_DIRECTION_BIT_MASK  0x18
#define M_BUTTON_TYPE_BIT_MASK 0x20
#define M_PRIORITY_BIT_MASK  0x40
#define M_ACTIVE_BIT_MASK 0x80

#define M_BUTTON_COUNT 4
#define M_FLOOR_COUNT 4

#define CLOCK_CYCLES_PER_SECOND 10000

typedef uint8_t query_t; // 1 bit for active, 1 bit for priority, 1 bit for button type, 2 bits for direction, 3 bits for floor

extern bool g_floor_panel[M_FLOOR_COUNT*2-2]; /// [1 up, 2 up, 3 up, 2 down, 3 down, 4 down]
extern bool g_cab_panel[M_BUTTON_COUNT]; /// [1, 2, 3, 4]
extern fsm_state_t g_current_state;
extern query_t g_query[M_QUERY_LEN];
#endif
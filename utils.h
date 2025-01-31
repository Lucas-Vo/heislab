#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include "fsm.h"

#define M_QUERY_LEN  10-1

//Masks for getting correct information from the query info package
#define M_PRIORITY_BIT_MASK  0x40
#define M_FLOOR_BIT_MASK  0x07
#define M_DIRECTION_BIT_MASK  0x38

uint8_t sort_query (uint8_t *query, fsm_state_t *elevator_state);
uint8_t prioritize_inputs (uint8_t *query, fsm_state_t *elevator_state);












#endif
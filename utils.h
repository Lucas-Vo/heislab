#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include "fsm.h"
#include "global.h"


//Masks for getting correct information from the query info package
uint8_t sort_query ( query_t *query, fsm_state_t *elevator_state);
uint8_t prioritize_inputs ( query_t *query, fsm_state_t *elevator_state);

#endif
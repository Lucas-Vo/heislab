#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include "fsm.h"
#include "global.h"
#include "driver/elevio.h"


//Masks for getting correct information from the query info package
uint8_t sort_query ( query_t *query, fsm_state_t *elevator_state);
uint8_t populate_query ( query_t *query, fsm_state_t *elevator_state, uint8_t floor, ButtonType button);
uint8_t iterate_query ( query_t *query);
uint8_t clear_query ( query_t *query);

uint8_t prioritize_inputs ( query_t *query, fsm_state_t *elevator_state);

uint8_t update_elevator_floor(fsm_state_t *p_elevator_state);
uint8_t update_elevator_state(fsm_state_t *p_elevator_state);

uint8_t poll_floor_panel (bool *arr_floor_panel);
uint8_t poll_elevator_panel (bool *arr_elevator_panel);



#endif
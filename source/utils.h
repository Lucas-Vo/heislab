#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include "fsm.h"
#include "global.h"
#include "driver/elevio.h"





// functions for handling query 
uint8_t sort_query ( query_t *arr_query, fsm_state_t *p_elevator_state);
uint8_t populate_query ( query_t *arr_query, fsm_state_t *p_elevator_state, uint8_t floor, ButtonType button);
uint8_t iterate_query ( query_t *arr_query);
uint8_t clear_query ( query_t *arr_query);
uint8_t prioritize_query_elements ( query_t *arr_query, fsm_state_t *p_elevator_state);

// functions for updating 
uint8_t update_elevator_floor(fsm_state_t *p_elevator_state);
uint8_t update_elevator_state(fsm_state_t *p_elevator_state);

uint8_t poll_floor_panel (bool *arr_floor_panel);
uint8_t poll_cab_panel (bool *arr_elevator_panel);
void poll (query_t *arr_query, fsm_state_t* p_elevator_state);


#endif
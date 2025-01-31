#include "utils.h"



/*
@brief Sorts the query array based on the priority bit, direction moving, and the floor bit
@param query: pointer to the query array
*/
uint8_t sort_query (uint8_t *arr_query, fsm_state_t *p_elevator_state) {
    int i, j;
    uint8_t temp;
    for (j = 0; j < M_QUERY_LEN-1; j++) {
        for (i = 0; i < M_QUERY_LEN-j; i++) {
            if (arr_query[i]&0x40 > arr_query[i+1]&0x40) {
                 //checks if the 7th bit (Prioritized in stage bit) is bigger than no 2
                continue;
            }
            else if (arr_query[i]&0x40 == 0) {
                //checks if relevant bit is 0, which means it is not prioritized this round
                continue;
            }
            else if (arr_query[j+1]&0x40 == 1){
                //If the next on the queue is prioritized, then swap if the 

            }
        }
    }
    return 0;
}

/*
@brief Prioritizes the inputs based on the priority bit, direction moving, and the floor bit
@param query: pointer to the query array
*/
uint8_t prioritize_inputs (uint8_t *arr_query, fsm_state_t *p_elevator_state){
    for (uint8_t i = 0; i <M_QUERY_LEN; i++){
        if (p_elevator_state->direction == arr_query[i]&M_DIRECTION_BIT_MASK){
            //checks if the moving direction of the query is the same as the elevator
            if ( arr_query[i] & M_FLOOR_BIT_MASK >= p_elevator_state->floor && p_elevator_state->direction == FSM_DIRECTION_UP)
            { // if moving up and the floor is higher than the current floor
            
                //If the movie is up and the floor is higher than the current floor
        }
    }

    
    return 0;
}
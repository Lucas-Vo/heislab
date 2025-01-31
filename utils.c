#include "utils.h"



/*
@brief Sorts the query array based on the priority bit, direction moving, and the floor bit
@param query: pointer to the query array
*/
uint8_t sort_query (uint8_t *query,) {
    int i, j;

    uint8_t temp;
    for (j = 0; j < M_QUERY_LEN; j++) {
        for (i = 0; i < M_QUERY_LEN-j; i++) {
            if (query[i]&0x40 > query[i+1]&0x40) {
                 //checks if the 7th bit (Prioritized in stage bit) is bigger than no 2
                continue;
            }
            else if (query[i]&0x40 == 0) {
                //checks if relevant bit is 0, which means it is not prioritized this round
                continue;
            }
            else if (query[j+1]&0x40 == 1){
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
uint8_t prioritize_inputs (uint8_t *query, fsm_state_s &elevator_state){

    return 0;
}
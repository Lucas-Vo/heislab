#include "utils.h"

/*
@brief yuuhh
@param 
*/
uint8_t poll (){
    return 0;
}


/*
@brief Sorts the query array based on the priority bit, direction moving, and the floor bit
@param query: pointer to the query array
*/
uint8_t sort_query ( query_t *arr_query, fsm_state_t *p_elevator_state)
{
    if (p_elevator_state->direction == FSM_DIRECTION_IDLE)
    {
        //checks if the elevator is idle
        return 0;
    }
    query_t temp;

    for (uint8_t j = 0; j < M_QUERY_LEN-1; j++)
    {
        for (uint8_t i = 0; i < M_QUERY_LEN-j; i++)
        {
            if (arr_query[i+1]&M_PRIORITY_BIT_MASK == 0)
            {
                //checks if relevant bit is 0, which means it is not prioritized this round
                continue;
            }
            else if (arr_query[j]&M_PRIORITY_BIT_MASK == 0)
            {
                //Swaps the two queries if arr_query[j] is not prioritized, but arr_query[j+1] is
                continue;
            }
            else 
            {
                //If both queries are prioritized, it checks the floor and movement direction of current elevator
                if (p_elevator_state->direction ==  FSM_DIRECTION_UP)
                {
                    if (arr_query[j]&M_FLOOR_BIT_MASK > arr_query[j+1]&M_FLOOR_BIT_MASK)
                    {
                        //Swaps the two queries if the floor of arr_query[j] is higher than arr_query[j+1]
                        temp = arr_query[j];
                        arr_query[j] = arr_query[j+1];
                        arr_query[j+1] = temp;
                    }
                }
                else if (p_elevator_state->direction ==  FSM_DIRECTION_DOWN)
                {
                    if (arr_query[j]&M_FLOOR_BIT_MASK < arr_query[j+1]&M_FLOOR_BIT_MASK)
                    {
                        //Swaps the two queries if the floor of arr_query[j] is higher than arr_query[j+1]
                        temp = arr_query[j];
                        arr_query[j] = arr_query[j+1];
                        arr_query[j+1] = temp;
                    }
                }
            }
        }
    }
    return 0;
}

/*
@brief Prioritizes the inputs based on the priority bit, direction moving, and the floor bit
@param query: pointer to the query array
*/
uint8_t prioritize_inputs ( query_t *arr_query, fsm_state_t *p_elevator_state)
{
    if (p_elevator_state->direction == FSM_DIRECTION_IDLE)
    {
            //checks if the elevator is idle
            arr_query[0] |= M_PRIORITY_BIT_MASK;
            return 0;
        }
    for (uint8_t i = 0; i <M_QUERY_LEN; i++)
    {
        if (arr_query[i]&M_ACTIVE_BIT_MASK == 0)
        {
            //checks if the query is active
            continue;
        }
        else if (p_elevator_state->direction == arr_query[i]&M_DIRECTION_BIT_MASK)
        {
            //checks if the moving direction of the query is the same as the elevator
            if (p_elevator_state->direction == FSM_DIRECTION_UP && arr_query[i] & M_FLOOR_BIT_MASK >= p_elevator_state->floor)
            { // if moving up and the floor is higher than the current floor
                arr_query[i] |= M_PRIORITY_BIT_MASK;
                continue;
            }
            else if (p_elevator_state->direction == FSM_DIRECTION_DOWN && arr_query[i] & M_FLOOR_BIT_MASK <= p_elevator_state->floor)
            { // if moving up and the floor is higher than the current floor
                arr_query[i] |= M_PRIORITY_BIT_MASK;
                continue;
            }
        }
    }
    return 0;
}
#include "utils.h"

/*
@brief yuuhh
@param 
*/
uint8_t poll ()
{
    return 0;
}


/*
@brief Sorts the query array based on the priority bit, direction moving, and the floor bit
@param query: pointer to the query array
*/
uint8_t sort_query ( query_t *arr_query, fsm_state_t *p_elevator_state)
{
    if (p_elevator_state->event == FSM_EVENT_IDLE)
    {
        //checks if the elevator is idle
        return 0;
    }
    query_t temp;

    for (uint8_t j = 0; j < M_QUERY_LEN-1; j++)
    {
        for (uint8_t i = 0; i < M_QUERY_LEN-j; i++)
        {
            if ( (arr_query[i+1]&M_PRIORITY_BIT_MASK) == 0)
            {
                //checks if relevant bit is 0, which means it is not prioritized this round
                continue;
            }
            else if ( (arr_query[j]&M_PRIORITY_BIT_MASK) == 0)
            {
                //Swaps the two queries if arr_query[j] is not prioritized, but arr_query[j+1] is
                continue;
            }
            else 
            {
                //If both queries are prioritized, it checks the floor and movement direction of current elevator
                if (p_elevator_state->event ==  FSM_EVENT_UP)
                {
                    if ( (arr_query[j]&M_FLOOR_BIT_MASK) > (arr_query[j+1]&M_FLOOR_BIT_MASK) )
                    {
                        //Swaps the two queries if the floor of arr_query[j] is higher than arr_query[j+1]
                        temp = arr_query[j];
                        arr_query[j] = arr_query[j+1];
                        arr_query[j+1] = temp;
                    }
                }
                else if (p_elevator_state->event ==  FSM_EVENT_DOWN)
                {
                    if ( (arr_query[j]&M_FLOOR_BIT_MASK) < (arr_query[j+1]&M_FLOOR_BIT_MASK) )
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
@brief Populates the query with a new query element if it not already exists
@param query: pointer to the query array
@param elevator_state: pointer to the elevator state struct
@param floor: the floor of the new query
@param button: the button of the new query
*/
uint8_t populate_query ( query_t *query, fsm_state_t *elevator_state, uint8_t floor, ButtonType button){
    query_t new_query_element=0;
    
    uint8_t direction = 0; // FIX THIS DIRECTION SO IT CAN DO THIS WELL THANK YOU

    new_query_element |= M_ACTIVE_BIT_MASK;
    new_query_element |= floor;
    new_query_element |= direction<<3;
    for (uint8_t i = 0; i < M_QUERY_LEN; i++)
    {
        if (query[i]==new_query_element)
        {
            return 0;
        }
        else if ((query[i]&M_ACTIVE_BIT_MASK) == 0)
        {
            query[i] = new_query_element;
            return 0;
        }
    }
    return 0;
}

/*
@brief Throws the first element of the query array, and moves the others one step down
@param query: pointer to the query array
*/
uint8_t iterate_query (query_t *query)
{
    for (uint8_t i = 0; i < M_QUERY_LEN-1; i++)
    {
        query[i] = query[i+1];
    }
    return 0;
}

/*
@brief Clears the query array
@param query: pointer to the query array
*/
uint8_t clear_query ( query_t *query){
    for (uint8_t i = 0; i < M_QUERY_LEN; i++)
    {
        query[i] = 0;
    }
    return 0;
}

/*
@brief Prioritizes the inputs based on the priority bit, direction moving, and the floor bit
@param query: pointer to the query array
*/
uint8_t prioritize_inputs ( query_t *arr_query, fsm_state_t *p_elevator_state)
{
    if (p_elevator_state->event == FSM_EVENT_IDLE)
    {
            //checks if the elevator is idle
            arr_query[0] |= M_PRIORITY_BIT_MASK;
            return 0;
        }
    for (uint8_t i = 0; i <M_QUERY_LEN; i++)
    {
        if ((arr_query[i]&M_ACTIVE_BIT_MASK) == 0)
        {
            //checks if the query is active
            continue;
        }
        else if (p_elevator_state->event == (arr_query[i]&M_DIRECTION_BIT_MASK))
        {
            //checks if the moving direction of the query is the same as the elevator
            if ((p_elevator_state->event == FSM_EVENT_UP) && ((arr_query[i] & M_FLOOR_BIT_MASK) >= p_elevator_state->floor))
            { // if moving up and the floor is higher than the current floor
                arr_query[i] |= M_PRIORITY_BIT_MASK;
                continue;
            }
            else if ((p_elevator_state->event == FSM_EVENT_DOWN) && ((arr_query[i] & M_FLOOR_BIT_MASK) <= p_elevator_state->floor))
            { // if moving up and the floor is higher than the current floor
                arr_query[i] |= M_PRIORITY_BIT_MASK;
                continue;
            }
        }
    }
    return 0;
}

/*
@brief Prioritizes the inputs based on the priority bit, direction moving, and the floor bit
@param query: pointer to the query array
*/
uint8_t update_elevator_floor(fsm_state_t *p_elevator_state)
{
    int sensedfloor = elevio_floorSensor();
    if (elevio_floorSensor() != -1)
    {
        p_elevator_state->floor = elevio_floorSensor()<<1;
    }
    else
    { // means elevator is stuck between two worlds
        int prev_floor = p_elevator_state->floor;
        if (p_elevator_state->event == FSM_EVENT_UP)
        {
            p_elevator_state->floor = prev_floor + 1;
        }
        else if (p_elevator_state->event == FSM_EVENT_DOWN)
        {
            p_elevator_state->floor = prev_floor - 1;
        }
    }
    return 0;
}

/* ATTENZIONE */
uint8_t poll_floor_panel (bool *arr_floor_panel)
{
    for (int i=0; i<3; i++)
    {
        arr_floor_panel[i] = (bool) elevio_callButton(i, BUTTON_HALL_DOWN);
        arr_floor_panel[i+3] = (bool) elevio_callButton(i, BUTTON_HALL_UP);
    }
    return 0;
}

uint8_t poll_cab_panel(bool *arr_elevator_panel)
{
    for(int i = 0; i < M_FLOOR_COUNT; i++)
    {
        bool btnPressed = (bool) elevio_callButton(i, BUTTON_CAB);
        arr_elevator_panel[i] = btnPressed;
    }
    return 0;
}
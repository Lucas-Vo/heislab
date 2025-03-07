#include "utils.h"
#include "global.h"


/*
@brief Sorts the query array based on the priority bit, direction moving, and the floor bit
@param *arr_query: pointer to the query array
@param *p_elevator_state: pointer to the elevator state struct
*/
uint8_t sort_query ( query_t *arr_query, fsm_state_t *p_elevator_state)
{
    prioritize_query_elements (arr_query, p_elevator_state);
    if (p_elevator_state->event == FSM_EVENT_IDLE)
    {
        //checks if the elevator is idle
        return 0;
    }

    query_t temp;
    for (uint8_t j = 0; j < M_QUERY_LEN-1; j++)
    {
        for (uint8_t i = 0; i < M_QUERY_LEN-j-1; i++)
        {
            if ( (arr_query[i+1]&M_PRIORITY_BIT_MASK) == 0)
            {
                //checks if relevant bit is 0, which means it is not prioritized this round
                continue;
            }
            else if ( (arr_query[i]&M_PRIORITY_BIT_MASK) == 0)
            {
                //Swaps the two queries if arr_query[j] is not prioritized, but arr_query[j+1] is
                temp = arr_query[i];
                arr_query[i] = arr_query[i+1];
                arr_query[i+1] = temp;
                continue;
            }
            else if (1)
            {
                //If both queries are prioritized, it checks the floor and movement direction of current elevator
                if (p_elevator_state->event ==  FSM_EVENT_UP)
                {
                    if ( (arr_query[i]&M_FLOOR_BIT_MASK) > (arr_query[i+1]&M_FLOOR_BIT_MASK) )
                    {
                        //Swaps the two queries if the floor of arr_query[j] is higher than arr_query[j+1]
                        temp = arr_query[i];
                        arr_query[i] = arr_query[i+1];
                        arr_query[i+1] = temp;
                    }
                }
                else if (p_elevator_state->event ==  FSM_EVENT_DOWN)
                {
                    if ( (arr_query[i]&M_FLOOR_BIT_MASK) < (arr_query[i+1]&M_FLOOR_BIT_MASK) )
                    {
                        //Swaps the two queries if the floor of arr_query[j] is higher than arr_query[j+1]
                        temp = arr_query[i];
                        arr_query[i] = arr_query[i+1];
                        arr_query[i+1] = temp;
                    }
                }
            }
        }
    }
    return 0;
}

/*
@brief Populates the query with a new query element if it not already exists
@param *arr_query: pointer to the query array
@param *elevator_state: pointer to the elevator state struct
@param floor: the floor of the new query
@param button: the button of the new query
*/

uint8_t populate_query(query_t *arr_query, fsm_state_t *elevator_state, uint8_t floor, ButtonType button)
{
    query_t new_query_element = 0;
    uint8_t direction = 0;

    // if the button is on the top panel, the value of the direction bits in the queue value will be 01 for up and 10 for down
    if (button != BUTTON_CAB)
    {
        direction = (int) button + 1;
    }
    // If the button is on the cab panel, the direction of the call will depend on the current floor of the elevator
    else
    {
        if (elevator_state->floor < (floor<<1)) {
            // If the elevator is below the requested floor, move up
            direction = 1; // Move up
        }
        else if (elevator_state->floor > (floor<<1)) {
            // If the elevator is above the requested floor, move down
            direction = 2; // Move down
        }
        else {
            // If the elevator is on the requested floor, use the button's requested direction
            direction = 0;
        }
    }
    
    // Set the new query element with the direction, floor, and active bit
    new_query_element |= M_ACTIVE_BIT_MASK;
    new_query_element |= (floor << 1);
    new_query_element |= (direction << 3);
    //Sets the button type bit on the elevator code call
    if (button == BUTTON_CAB)
    {
        new_query_element |= 0b00100000;
    }
   

    // Check if query already contains the same element, if so return
    for (uint8_t i = 0; i < M_QUERY_LEN; i++)
    {
        if ((arr_query[i]&0b10111111) == (new_query_element&0b10111111))
        {
            return 0;
        }
        else if ((arr_query[i] & M_ACTIVE_BIT_MASK) == 0)
        {
            arr_query[i] = new_query_element;
            return 0;
        }
    }
    return 0;
}


/*
@brief Throws the first element of the query array, and moves the others one step down
@param arr_query: pointer to the query array
*/
uint8_t iterate_query (query_t *arr_query)
{   
    uint8_t current_floor = (g_query[0] & M_FLOOR_BIT_MASK)>>1;
    uint8_t button = (g_query[0] & M_BUTTON_TYPE_BIT_MASK)>>5;
    ButtonType buttontype;
    uint8_t direction = (g_query[0]&M_DIRECTION_BIT_MASK)>>3;
    if ((button == 0) && (direction == FSM_EVENT_UP))
    {//up
        buttontype = BUTTON_HALL_UP;
    }
    else if ((button == 0) && (direction == FSM_EVENT_DOWN))
    {//down
        buttontype = BUTTON_HALL_DOWN;
    }
    else
    {//cab panel
        buttontype = BUTTON_CAB;
    }
    elevio_buttonLamp(current_floor, buttontype, 0);
    for (uint8_t i = 0; i < M_QUERY_LEN-1; i++)
    {
        arr_query[i] = arr_query[i+1];
    }
    return 0;
}

/*
@brief Clears the query array
@param arr_query: pointer to the query array
*/
uint8_t clear_query ( query_t *arr_query)
{
    for (uint8_t i = 0; i < M_QUERY_LEN; i++)
    {
        arr_query[i] = 0;
    }
    return 0;
}

/*
@brief Prioritizes the inputs based on the priority bit, direction moving, and the floor bit
@param arr_query: pointer to the query array
@param *p_elevator_state: pointer to the elevator state struct
*/
uint8_t prioritize_query_elements ( query_t *arr_query, fsm_state_t *p_elevator_state)
{
    //checks if the elevator is idle
    if (p_elevator_state->event == FSM_EVENT_IDLE && ((arr_query[0] & M_ACTIVE_BIT_MASK) != 0))
    {
            arr_query[0] |= M_PRIORITY_BIT_MASK;
            return 0;
        }
    for (uint8_t i = 0; i <M_QUERY_LEN; i++)
    {
        //checks if the query is active
        if ((arr_query[i]&M_ACTIVE_BIT_MASK) == 0)
        {
            continue;
        }
        //checks if the moving direction of the query is the same as the elevator
        else if ((p_elevator_state->event == ((arr_query[i]&M_DIRECTION_BIT_MASK))>>3) || ((arr_query[i]&M_BUTTON_TYPE_BIT_MASK)>>5 == 1))
        {
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
@brief Polls info from the panel and populates the arr_floor_panel with current status of panel buttons pressed
@param arr_floor_panel: pointer to array of floor panel inputs
*/
uint8_t poll_floor_panel (bool *arr_floor_panel) //[1 up, 2 up, 3 up, 2 down, 3 down, 4 down]
{
    for (int i=0; i<3; i++)
    {
        arr_floor_panel[i] = (bool) elevio_callButton(i, BUTTON_HALL_UP);
        arr_floor_panel[i+3] = (bool) elevio_callButton(i+1, BUTTON_HALL_DOWN);
    }


    return 0;
}

/*
@brief Polls info from the front panel and populates the arr_elevator_panel with current status of panel buttons pressed
@param arr_elevator_panel: pointer to array of cab panel inputs
*/
uint8_t poll_cab_panel(bool *arr_elevator_panel)
{
    for(int i = 0; i < M_FLOOR_COUNT; i++)
    {
        arr_elevator_panel[i] = (bool) elevio_callButton(i, BUTTON_CAB);;
    }
    return 0;
}

/*
@brief polls both panels for inputs, and populates the query with the given inputs.
@param arr_query: pointer to the query array
@param elevator_state: pointer to the elevator state struct
*/
void poll (query_t *arr_query, fsm_state_t* elevator_state)
{
    poll_cab_panel(g_cab_panel);
    poll_floor_panel(g_floor_panel);
    
    for (int i = 0; i<M_FLOOR_COUNT;i++)
    {
        if (g_cab_panel[i])
        {
            populate_query (arr_query, elevator_state, i, BUTTON_CAB);
        }
    }
    for (int i= 0; i<6;i++){ /// g_floor_panel[6]; [1 up, 2 up, 3 up, 2 down, 3 down, 4 down]
        if (i<3 && g_floor_panel[i]){
            populate_query (arr_query, elevator_state, i, BUTTON_HALL_UP);
        }
        else if(g_floor_panel[i]){                  // i - 2 ?
            populate_query(arr_query,elevator_state,i-2,BUTTON_HALL_DOWN);
        }
    }
}


#include "fsm.h"
#include "driver/elevio.h"
#include <time.h>
#include "utils.h"

void flop(bool* flip){
    *flip = !(*flip);
}

void exit_functions_fsm(fsm_state_t *p_state)
{
    if (p_state->transition != true) return;
    switch (p_state->previous_event)
    {
    case FSM_EVENT_UP:
        UpExit(p_state);
        break;
    case FSM_EVENT_DOWN:
        DownExit(p_state);
        break;
    case FSM_EVENT_IDLE:
        IdleExit(p_state);
        break;
    case FSM_EVENT_HALT:
        HaltExit(p_state);
        break;
    case FSM_EVENT_ATFLOOR:
        AtFloorExit(p_state);
        break;
    default:
        break;
    }
}

void enter_functions_fsm(fsm_state_t *p_state)
{
    if(p_state->transition != true) return;
    p_state->transition = false;
    p_state->previous_event = p_state->event;
    switch (p_state->event)
    {
    case FSM_EVENT_UP:
        UpEnter(p_state);
        break;
    case FSM_EVENT_DOWN:
        DownEnter(p_state);
        break;
    case FSM_EVENT_IDLE:
        IdleEnter(p_state);
        break;
    case FSM_EVENT_HALT:
        HaltEnter(p_state);
        break;
    case FSM_EVENT_ATFLOOR:
        AtFloorEnter(p_state);
        break;
    default:
        break;
    }
}

void update_functions_fsm(fsm_state_t *p_state)
{
    // clean up
    if (g_query[M_QUERY_LEN-1] != 0){
        clear_query(g_query);
    }
    // remove duplicates
    

    switch (p_state->event)
    {
    case FSM_EVENT_IDLE:
        IdleUpdate(p_state);
        break;
    
    case FSM_EVENT_UP:
        UpUpdate(p_state);
        break;
    
    case FSM_EVENT_DOWN:
        DownUpdate(p_state);
        break;
    case FSM_EVENT_HALT:
        HaltUpdate(p_state);
        break;
    
    case FSM_EVENT_ATFLOOR:
        AtFloorUpdate(p_state);
        break;
    default:
        break;
    }
}

void init_fsm(fsm_state_t *p_state)
{

    // turns off 4th floor down light
    elevio_buttonLamp(3, BUTTON_HALL_DOWN, 0);

    clear_query(g_query);
    elevio_floorIndicator(0);
    for(uint8_t i = 0; i < 3; i++){
        elevio_buttonLamp(i, BUTTON_HALL_UP,0);
        elevio_buttonLamp(i, BUTTON_HALL_DOWN,0);
        elevio_buttonLamp(i, BUTTON_CAB,0);
    }
    elevio_buttonLamp(3, BUTTON_CAB,0);

    // goes down to first floor
    while(1){
        nanosleep(&(struct timespec){0, 100*1000*1000}, NULL);
        elevio_motorDirection(DIRN_DOWN);
        if (elevio_floorSensor() == 0){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
    }


    p_state->event = FSM_EVENT_IDLE;
    p_state->previous_event = FSM_EVENT_IDLE;
    p_state->floor = FSM_FLOOR_1;
    p_state->timer = 0;
    
}

void run_fsm(fsm_state_t *p_state)
{
    while (1) {
        poll(g_query,p_state); // polls all buttons
        service_all_lights(p_state); 
        exit_functions_fsm(p_state);
        enter_functions_fsm(p_state);
        update_functions_fsm(p_state);
        // int a = elevio_callButton(3,BUTTON_HALL_DOWN);
        // printf("%d", a);
    }
}

// press 3 heispanel og 2-up
void service_all_lights(fsm_state_t *p_state)
{
    // service the 4 floor lights to the right
    if ((int)(p_state->floor)%2 == 0 && (p_state->floor >=(0)))
    {
        elevio_floorIndicator((int)((p_state->floor))/2);
    }

    // service the stop button
    if (p_state->event == FSM_EVENT_HALT)
    {
        elevio_stopLamp(1);
    }
    else
    {
        elevio_stopLamp(0);
    }

    // handles the 6 lights on the top panel based on g_floor_panel
    elevio_buttonLamp(0, BUTTON_HALL_UP, (int)g_floor_panel[0]);
    elevio_buttonLamp(1, BUTTON_HALL_UP, (int)g_floor_panel[1]);
    elevio_buttonLamp(2, BUTTON_HALL_UP, (int)g_floor_panel[2]);
    elevio_buttonLamp(1, BUTTON_HALL_DOWN, (int)g_floor_panel[3]);
    elevio_buttonLamp(2, BUTTON_HALL_DOWN, (int)g_floor_panel[4]);
    elevio_buttonLamp(3, BUTTON_HALL_DOWN, (int)g_floor_panel[5]);

    // handles the 4 lights on the cab panel based on g_cab_panel
    elevio_buttonLamp(0, BUTTON_CAB, (int)g_cab_panel[0]);
    elevio_buttonLamp(1, BUTTON_CAB, (int)g_cab_panel[1]);
    elevio_buttonLamp(2, BUTTON_CAB, (int)g_cab_panel[2]);
    elevio_buttonLamp(3, BUTTON_CAB, (int)g_cab_panel[3]);
    
    // handles the stop lamp if button is being pressed
    elevio_stopLamp(elevio_stopButton());
    
    // handles the 6 on top and the 4 in front
    for (uint8_t i = 0; i < M_QUERY_LEN; i++)
    {
        // check if active
        if (!(g_query[i]&M_ACTIVE_BIT_MASK)) return; 

        uint8_t current_floor = (g_query[i] & M_FLOOR_BIT_MASK)>>1;
        uint8_t button = (g_query[i] & M_BUTTON_TYPE_BIT_MASK)>>5;
        ButtonType buttontype;
        uint8_t direction = (g_query[i]&M_DIRECTION_BIT_MASK)>>3;
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
        elevio_buttonLamp(current_floor, buttontype, 1);
    }

}


// Enter functions

void IdleEnter(fsm_state_t *p_state)
{
    elevio_motorDirection(DIRN_STOP);
}
void AtFloorEnter(fsm_state_t *p_state)
{
    elevio_motorDirection(DIRN_STOP);
    if (elevio_floorSensor() != -1) {
        p_state->floor = (fsm_floor_t) elevio_floorSensor() * 2;
        p_state->timer = clock();
    } else if (elevio_floorSensor() == -1)
    {
        // wiggle until floor detected
        bool flip = false;
        bool* p_flip = &flip;
        uint32_t counter = 1;
        while(elevio_floorSensor() == -1) 
        {
            if (flip){
                elevio_motorDirection(DIRN_UP);

            } else {
                elevio_motorDirection(DIRN_DOWN);
            }
            nanosleep(&(struct timespec){0, counter*10*1000*1000}, NULL);
            
            counter ++;
            flop(p_flip);
        }
        elevio_motorDirection(DIRN_STOP);
        p_state->floor = (fsm_floor_t) elevio_floorSensor() * 2;
        p_state->timer = clock();
    }
}
void UpEnter(fsm_state_t *p_state)
{
    p_state->floor += 1; // + 0.5
    elevio_motorDirection(DIRN_UP);
}

void DownEnter(fsm_state_t *p_state)
{
    p_state->floor -= 1; // - 0.5
    elevio_motorDirection(DIRN_DOWN);
}
void HaltEnter(fsm_state_t *p_state)
{
    //turn on light
    elevio_stopLamp(1);
    //clear query
    clear_query (g_query);
    // stop moving
    elevio_motorDirection(DIRN_STOP);

    p_state ->timer = clock();

}

// Update functions
void IdleUpdate(fsm_state_t *p_state)
{
    //logic
    //transition conditions
    sort_query (g_query, p_state);
    // if query[0].active = not empty, transition to up or down
    if((int)(g_query[0] & M_ACTIVE_BIT_MASK) != 0){
        p_state->transition = true;
        if(p_state->floor < (g_query[0] & M_FLOOR_BIT_MASK)){
            p_state->event = FSM_EVENT_UP;
        } else if (p_state->floor > (g_query[0] & M_FLOOR_BIT_MASK)){
            p_state->event = FSM_EVENT_DOWN;
        }
        else if (p_state->floor == (g_query[0] & M_FLOOR_BIT_MASK)){
            iterate_query(g_query);
        }
    }

    // halt



}

void UpUpdate(fsm_state_t *p_state)
{
    //logic
    //transition conditions
    sort_query (g_query, p_state);
    if (elevio_floorSensor() != -1){
        p_state -> floor = elevio_floorSensor()<<1;
    }
    else if ( (p_state->floor %2 == 0) && (elevio_floorSensor()==-1)){
        p_state->floor++;
    }
    // if query[0].floor = floor, go to atfloor
    if((int)( (g_query[0] & M_FLOOR_BIT_MASK)>>1 )== elevio_floorSensor())
    {
        iterate_query(g_query);
        elevio_motorDirection(DIRN_STOP);
        p_state->transition = true;
        p_state->event = FSM_EVENT_ATFLOOR;
        

    } 
    if(elevio_stopButton()){
        iterate_query(g_query);
        elevio_motorDirection(DIRN_STOP);
        p_state->transition = true;
        p_state->event = FSM_EVENT_HALT;
    }
}

void DownUpdate(fsm_state_t *p_state)
{
    //logic
    //transition conditions
    sort_query (g_query, p_state);
    if (elevio_floorSensor() != -1){
        p_state -> floor = elevio_floorSensor()<<1;
    }
    else if ( (p_state->floor %2 ==0) && (elevio_floorSensor()==-1)){
        p_state->floor--;
    }
    // if query[0].floor = floor, go to atfloor
    if((int)( (g_query[0] & M_FLOOR_BIT_MASK)>>1 )== elevio_floorSensor())
    {
        iterate_query(g_query);
        elevio_motorDirection(DIRN_STOP);
        p_state->transition = true;
        p_state->event = FSM_EVENT_ATFLOOR;
    }

    if(elevio_stopButton()){
        iterate_query(g_query);
        elevio_motorDirection(DIRN_STOP);
        p_state->transition = true;
        p_state->event = FSM_EVENT_HALT;
    }
}
void HaltUpdate(fsm_state_t *p_state)
{
    sort_query (g_query, p_state);

    //if on floor, open door in 3 sec
    if((int)(p_state->floor)%2 == 0)
    {
        p_state->transition = true;
        p_state->event = FSM_EVENT_ATFLOOR;
        ; /* ATTENZIONE FIX THIS LATER */
    }
    // transition to idle after 3 seconds
    if ((double)(p_state->timer-clock())/ CLOCKS_PER_SEC * 1.0>3.0){
        p_state->transition = true;
        p_state->event = FSM_EVENT_IDLE;
    }

}

void AtFloorUpdate(fsm_state_t *p_state)
{
    //logic
    //transition conditions
    sort_query (g_query, p_state);

    // check if query is empty, if so, transition to idle
    if((int)(g_query[0] & M_ACTIVE_BIT_MASK) == 0){
        p_state->transition = true;
        p_state->event = FSM_EVENT_IDLE;
    }
    if ((double)(p_state->timer-clock())/ CLOCKS_PER_SEC * 1.0>3.0){
        p_state->transition = true;
        p_state->event = FSM_EVENT_IDLE;
    }
}

void IdleExit(fsm_state_t *p_state)
{

}
void AtFloorExit(fsm_state_t *p_state)
{
    p_state ->timer = 0;
    // turn off floor panel light and cab panel
}
void UpExit(fsm_state_t *p_state)
{
    //elevio_motorDirection(DIRN_STOP);
}
void DownExit(fsm_state_t *p_state)
{
    //elevio_motorDirection(DIRN_STOP);
}
void HaltExit(fsm_state_t *p_state)
{
    p_state ->timer = 0;
    elevio_stopLamp(0);
}
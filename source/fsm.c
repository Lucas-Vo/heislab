#include "fsm.h"
#include "driver/elevio.h"
#include <time.h>
#include "utils.h"


void exit_functions_fsm(fsm_state_t *p_state)
{
    if (p_state->transition != true) return;
    switch (p_state->event)
    {
    case FSM_EVENT_UP:
        p_state->floor += 1; // + 0.5
        UpExit(p_state);
        break;
    case FSM_EVENT_DOWN:
        p_state->floor -= 1; // - 0.5
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
    p_state->floor = FSM_FLOOR_1;
}

void run_fsm(fsm_state_t *p_state)
{
    while (1) {
        service_all_lights(p_state);
        exit_functions_fsm(p_state);
        enter_functions_fsm(p_state);
        update_functions_fsm(p_state);
    }
}


void service_all_lights(fsm_state_t *p_state)
{
    if ((int)(p_state->floor)%2 == 0)
    {
        elevio_floorIndicator((int)((p_state->floor))/2);
    }
    if (p_state->event == FSM_EVENT_HALT)
    {
        elevio_stopLamp(1);
    }
    else
    {
        elevio_stopLamp(0);
    }
    for (uint8_t i = 0; i < M_QUERY_LEN; i++)
    {
        if (!(g_query[i]&M_ACTIVE_BIT_MASK)) return; 
        uint8_t current_floor = g_query[i] & M_FLOOR_BIT_MASK>>1;
        uint8_t button = g_query[i] & M_BUTTON_TYPE_BIT_MASK;
        uint8_t direction = g_query[i]&M_DIRECTION_BIT_MASK>>3;
        if ((button == 0) && (direction == FSM_EVENT_UP))
        {
            button = 0;
        }
        else if ((button == 0) && (direction == FSM_EVENT_DOWN))
        {
            button = 1;
        }
        else
        {
            button = 2;
        }

        elevio_buttonLamp(current_floor, (ButtonType) button, 1);
    }
}


// Enter functions
void BootEnter(fsm_state_t *p_state)
{

}
void IdleEnter(fsm_state_t *p_state)
{

}
void AtFloorEnter(fsm_state_t *p_state)
{
    
}
void UpEnter(fsm_state_t *p_state)
{

}

void DownEnter(fsm_state_t *p_state)
{

}
void HaltEnter(fsm_state_t *p_state)
{

}

// Update functions
void IdleUpdate(fsm_state_t *p_state)
{
    //logic
    //transition conditions
}
void UpUpdate(fsm_state_t *p_state)
{
    //logic
    //transition conditions
}
void DownUpdate(fsm_state_t *p_state)
{
    //logic
    //transition conditions
}
void HaltUpdate(fsm_state_t *p_state)
{
    //logic
    //transition conditions
}

void AtFloorUpdate(fsm_state_t *p_state)
{
    //obstruction type 
}

void IdleExit(fsm_state_t *p_state)
{

}
void AtFloorExit(fsm_state_t *p_state)
{
    // turn off floor panel light and cab panel
}
void UpExit(fsm_state_t *p_state)
{

}
void DownExit(fsm_state_t *p_state)
{

}
void HaltExit(fsm_state_t *p_state)
{

}
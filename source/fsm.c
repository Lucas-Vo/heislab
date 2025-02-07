#include "fsm.h"
#include "driver/elevio.h"
#include <time.h>


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
        if (elevio_floorSensor() == 1){
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
        exit_functions_fsm(p_state);
        enter_functions_fsm(p_state);
        update_functions_fsm(p_state);
    }
}


// Enter functions
void BootEnter(fsm_state_t *p_state)
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
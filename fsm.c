#include "fsm.h"

/// @warning    This has to stay in sync with the fsm_state_t enum!
static stateFunctionRow_t stateFunction[] = {
  {"Idle",      &IdleEnter,      &IdleUpdate,    &IdleExit   },
  {"Up",        &UpEnter,        &UpUpdate,      &UpExit     },
  {"Down",      &DownEnter,      &DownUpdate,    &DownExit   },
  {"Halt",     &HaltEnter,      &HaltUpdate,    &HaltExit    },

};

void exit_functions_fsm(fsm_state_t *p_state)
{
    switch (p_state->event)
    {
    case FSM_EVENT_BOOT:
        // do nothing
        break;
    case FSM_EVENT_NONE:
        // do nothing
        break;
    case FSM_EVENT_UP:
        p_state->direction = FSM_DIRECTION_UP;
        p_state->floor += 1; // + 0.5
        UpEnter();
        break;
    case FSM_EVENT_DOWN:
        DownEnter();
        break;
    case FSM_EVENT_IDLE:
        IdleEnter();
        break;
    case FSM_EVENT_HALT:
        HaltEnter();
        break;
    
    default:
        break;
    }
}

void enter_functions_fsm(fsm_state_t *p_state)
{
    switch (p_state->event)
    {
    case FSM_EVENT_BOOT:
        BootEnter();
        p_state->event = FSM_EVENT_NONE;
        break;
    case FSM_EVENT_NONE:
        p_state->event = FSM_EVENT_NONE;
        break;
    case FSM_EVENT_UP:
        UpEnter();
        p_state->event = FSM_EVENT_NONE;
        break;
    case FSM_EVENT_DOWN:
        DownEnter();
        p_state->event = FSM_EVENT_NONE;
        break;
    case FSM_EVENT_IDLE:
        IdleEnter();
        p_state->event = FSM_EVENT_NONE;
        break;
    case FSM_EVENT_HALT:
        HaltEnter();
        p_state->event = FSM_EVENT_NONE;
        break;
    
    default:
        break;
    }
}

void update_functions_fsm(fsm_state_t *p_state)
{
    switch (p_state->direction)
    {
    case FSM_DIRECTION_IDLE:
        IdleUpdate();
        break;
    
    case FSM_DIRECTION_UP:
        UpUpdate();
        break;
    
    case FSM_DIRECTION_DOWN:
        DownUpdate();
        break;
    
    case FSM_DIRECTION_ERROR:
        HaltUpdate();
        break;
    
    default:
        break;
    }

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
void BootEnter()
{

}

// Update functions
void IdleUpdate()
{
    //logic
    //transition conditions
}
void UpUpdate()
{
    //logic
    //transition conditions
}
void DownUpdate()
{
    //logic
    //transition conditions
}
void HaltUpdate()
{
    //logic
    //transition conditions
}
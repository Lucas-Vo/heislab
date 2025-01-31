#include "fsm.h"

/// @warning    This has to stay in sync with the fsm_state_t enum!
static stateFunctionRow_t stateFunction[] = {
  {"Idle",      &IdleEnter,      &IdleUpdate,    &IdleExit   },
  {"Up",        &UpEnter,        &UpUpdate,      &UpExit     },
  {"Down",      &DownEnter,      &DownUpdate,    &DownExit   },
  {"Error",     &ErrorEnter,     &ErrorUpdate,   &ErrorExit  },

};


void run_fsm(fsm_state_t *p_state){
    while (1) {
        switch (p_state->direction)
        {
        case FSM_DIRECTION_IDLE:
            /* code */
            break;
        
        case FSM_DIRECTION_UP:
            /* code */
            break;
        
        case FSM_DIRECTION_DOWN:
            /* code */
            break;
        
        case FSM_DIRECTION_ERROR:
            /* code */
            break;
        
        default:
            break;
        }

    }
}


// Update functions
void IdleUpdate(){
    //logic
    //transition conditions
}
void UpUpdate(){
    //logic
    //transition conditions
}
void DownUpdate(){
    //logic
    //transition conditions
}
void ErrorUpdate(){
    //logic
    //transition conditions
}
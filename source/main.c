#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "fsm.h"

// TODO

/*
4th flor down button not working, might be hardware?
pressing 3-up then 4, elevator goes down, it should go up
pressing 3-down, elevator goes down, it should go up
*/

int main(){
    elevio_init();
    
    fsm_state_t *state;


    init_fsm(state);

    run_fsm(state);

    return 0;
}

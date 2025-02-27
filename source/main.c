#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "fsm.h"

// TODO

/*
Missing obstruction function and also stop button hasnt been tested. 

Lights flashing due to turning constantly on and off
*/

int main(){
    //initializes the elevator system
    elevio_init();
    
    //defines the state of elevator
    fsm_state_t *state;

    //sets elevator to known state
    init_fsm(state);

    //runs the elevator
    run_fsm(state);

    return 0;
}

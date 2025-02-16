#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "fsm.h"

// TODO

/*
Missing obstruction function and also stop button hasnt been tested. 
*/

int main(){
    elevio_init();
    
    fsm_state_t *state;

    init_fsm(state);

    run_fsm(state);

    return 0;
}

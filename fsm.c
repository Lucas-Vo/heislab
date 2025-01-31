#include "fsm.h"

/// @warning    This has to stay in sync with the fsm_state_t enum!
static stateFunctionRow_t stateFunction[] = {
  {"Idle",      &IdleEnter,      &IdleUpdate,    &IdleExit   },
  {"Up",        &UpEnter,        &UpUpdate,      &UpExit     },
  {"Down",      &DownEnter,      &DownUpdate,    &DownExit   },
  {"Error",     &ErrorEnter,     &ErrorUpdate,   &ErrorExit  },

};


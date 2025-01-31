#ifndef __FSM_H__
#define __FSM_H__

#include <stdbool.h> // booleans
#include <stdint.h>  // xintx_t types
#include <stdio.h>   // sprintf


// Different state of ATM machine
/// @warning This has to reflect the order of the stateFunctionRow_t array
typedef enum {
  FSM_DIRECTION_IDLE,
  FSM_DIRECTION_UP,
  FSM_DIRECTION_DOWN,
  FSM_DIRECTION_ERROR
} fsm_direction_t;

typedef enum {
  FSM_FLOOR_1,
  FSM_FLOOR_1_5,
  FSM_FLOOR_2,
  FSM_FLOOR_2_5,
  FSM_FLOOR_3,
  FSM_FLOOR_3_5,
  FSM_FLOOR_4,
} fsm_floor_t;

typedef struct fsm_state_s {
  fsm_direction_t direction;
  fsm_floor_t floor;
} fsm_state_t;

// Different type events
typedef enum {
  FSM_EVENT_NONE,
  FSM_EVENT_BOOT,
  FSM_EVENT_UP,
  FSM_EVENT_DOWN,
  FSM_EVENT_IDLE,
  FSM_EVENT_HALT,
} fsm_event_t;



typedef struct stateFunctionRow_s {
  const char *name;
  void (*enter_function)(void);
  void (*update_function)(void); /// continuously while in this state
  void (*exit_function)(void);
} stateFunctionRow_t;

typedef struct transitions_s {
  fsm_state_t currState;
  fsm_event_t event;
  fsm_state_t nextState;
} transitions_t;


// Enter functions
void IdleEnter();
void UpEnter();
void DownEnter();
void ErrorEnter();

// Update functions
void IdleUpdate();
void UpUpdate();
void DownUpdate();
void ErrorUpdate();

// Exit functions
void IdleExit();
void UpExit();
void DownExit();
void ErrorExit();



#endif
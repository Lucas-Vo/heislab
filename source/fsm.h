#ifndef __FSM_H__
#define __FSM_H__

#include <stdbool.h> // booleans
#include <stdint.h>  // xintx_t types
#include <stdio.h>   // sprintf


// Different state of ATM machine
/// @warning This has to reflect the order of the stateFunctionRow_t array


typedef enum {
  FSM_FLOOR_1,
  FSM_FLOOR_1_5,
  FSM_FLOOR_2,
  FSM_FLOOR_2_5,
  FSM_FLOOR_3,
  FSM_FLOOR_3_5,
  FSM_FLOOR_4,
} fsm_floor_t;

// Different type events
typedef enum {
  FSM_EVENT_IDLE,
  FSM_EVENT_UP,
  FSM_EVENT_DOWN,
  FSM_EVENT_HALT,
  FSM_EVENT_ATFLOOR,
} fsm_event_t;


typedef struct fsm_state_s {
  fsm_floor_t floor;
  fsm_event_t event;
  bool transition;
  uint64_t timer;
} fsm_state_t;


// Enter functions

void IdleEnter(fsm_state_t *p_state);
void AtFloorEnter(fsm_state_t *p_state);
void UpEnter(fsm_state_t *p_state);
void DownEnter(fsm_state_t *p_state);
void HaltEnter(fsm_state_t *p_state);

// Update functions
void IdleUpdate(fsm_state_t *p_state);
void AtFloorUpdate(fsm_state_t *p_state);
void UpUpdate(fsm_state_t *p_state);
void DownUpdate(fsm_state_t *p_state);
void HaltUpdate(fsm_state_t *p_state);

// Exit functions
void IdleExit(fsm_state_t *p_state);
void AtFloorExit(fsm_state_t *p_state);
void UpExit(fsm_state_t *p_state);
void DownExit(fsm_state_t *p_state);
void HaltExit(fsm_state_t *p_state);

void service_all_lights(fsm_state_t *p_state);

#endif
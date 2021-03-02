#include "switch_case.h"
#include "timer.c"

ElevatorState elevator_state; 
int timer_already_started = 0; 
time_t timer_start; 
int timer_duration = 3; 

switch (elevator_state)
{
case STATE_IDLE:
    /* code */
    break;

case STATE_MOVING:

    break;

case STATE_DOOR_OPEN:
    if (!timer_already_started){
        timer_start = get_time();
        timer_already_started = 1; 
    }
    if (hardware_read_obstruction_signal()){
        timer_start = get_time();
    }
    if (check_timer_done(timer_start, timer_duration)){
        hardware_command_door_open(0); //closes door
        timer_already_started = 0; 
        elevator_state = STATE_IDLE;
    }
    break;

case STATE_STOP_BUTTON_PRESSED:

    break;

default:
    break;
}
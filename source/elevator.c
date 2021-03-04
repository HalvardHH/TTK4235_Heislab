
#include "elevator.h"
#include <stdio.h>
#include <timer.c>
#include "hardware.h"

void door_timer(time_t* timer_start, int timer_duration, int* timer_already_started, ElevatorState* elevator_state){
    if (!*timer_already_started){
        *timer_start = get_time();
        *timer_already_started = 1; 
    }
    if (hardware_read_obstruction_signal()){
        printf("OBSTRUCIUOTN"); //testing only
        *timer_start = get_time();
    }
    if (check_timer_done(*timer_start, timer_duration)){
        hardware_command_door_open(0); //closes door
        printf("closing door");
        *timer_already_started = 0; 
        *elevator_state = STATE_IDLE;
    }   
}
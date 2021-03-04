#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "elevator.h"



int main(){
   
 int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
    ElevatorState elevator_state; 
    int timer_already_started = 0; 
    time_t timer_start; 
    int timer_duration = 3; 

    hardware_command_door_open(1); 
    printf("Opening the door");
    elevator_state = STATE_DOOR_OPEN;
    while(1){
        if(hardware_read_stop_signal()){
            elevator_state = STATE_STOP_BUTTON_PRESSED;
        }

        
        switch (elevator_state)
        {
        case STATE_IDLE:

            break;

        case STATE_MOVING:

            break;

        case STATE_DOOR_OPEN:
            door_timer(&timer_start, timer_duration, &timer_already_started, &elevator_state);
            break;

        case STATE_STOP_BUTTON_PRESSED:
            hardware_command_stop_light(1);
            
            
            hardware_command_stop_light(0);
            exit(1);
            break;
        default: 
            printf("NO VALID CASE");

        }
    }
}
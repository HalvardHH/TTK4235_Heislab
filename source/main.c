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
    clock_t timer_start = clock(); 
    HardwareMovement previous_direction = HARDWARE_MOVEMENT_STOP;
    HardwareMovement between_floor_direction = HARDWARE_MOVEMENT_STOP;
    int previous_legal_floor;
    int current_floor = return_legal_floor();
    queue_node *head = NULL;  

    elevator_software_init(&elevator_state, &previous_legal_floor);
    
    while(1){
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            elevator_state = STATE_STOP_BUTTON_PRESSED;
        }
        poll_order_buttons(&head);
        switch (elevator_state)
        {
        case STATE_IDLE:
            if (head != NULL){
                if (current_floor == -1){
                   idle_between_floors(&head, &previous_direction, between_floor_direction, previous_legal_floor);
                }
                else{
                    idle_on_floor(&head, &previous_direction, &between_floor_direction, current_floor);
                }
                elevator_state = STATE_MOVING;
            }
            break;

        case STATE_MOVING:
            if (moving_handler(&head, previous_direction, &current_floor, &previous_legal_floor)){
                hardware_command_door_open(1); //opens door
                elevator_state = STATE_DOOR_OPEN;
            }
            break;

        case STATE_DOOR_OPEN:
            if (door_timer(&timer_start, TIMER_DURATION, &timer_already_started)){
                hardware_command_door_open(0); //closes door
                elevator_state = STATE_IDLE;
            }
            break;

        case STATE_STOP_BUTTON_PRESSED:
            if (stop_button_handler(&head)){
                elevator_state = STATE_IDLE;
            }
            else{
                hardware_command_door_open(1);
                elevator_state = STATE_DOOR_OPEN;
            }
            break;
            
        default: 
            fprintf(stderr, "Out of state machine. Shutting down. \n");
            exit(1);

        }
    }
}
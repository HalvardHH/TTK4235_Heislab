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
    clock_t timer_start; 
    double timer_duration = 3; 
    HardwareMovement previous_direction = HARDWARE_MOVEMENT_STOP;
    int previous_legal_floor;

    int current_floor = return_legal_floor();
    hardware_command_floor_indicator_on(current_floor);

    queue_node *head = NULL;  

    elevator_state = STATE_IDLE;

    while (return_legal_floor() == -1) {
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            elevator_state = STATE_STOP_BUTTON_PRESSED;  //SPØR STUDASS
            break;
        }
        else if (return_legal_floor() > 3) {
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            previous_direction = HARDWARE_MOVEMENT_DOWN;
        }
        else if (return_legal_floor() < 0) {
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            previous_direction = HARDWARE_MOVEMENT_UP;

        }
        // hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    
    previous_legal_floor = return_legal_floor();

    
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
                   idle_between_floors(&head, &previous_direction, previous_legal_floor);
                }
                else if (current_floor == head->floor){
                    /* Do nothing */
                }
                else if (current_floor < head->floor){
                    hardware_command_movement(HARDWARE_MOVEMENT_UP);
                    previous_direction = HARDWARE_MOVEMENT_UP;
                }
                else{
                    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
                    previous_direction = HARDWARE_MOVEMENT_DOWN;
                }
                elevator_state = STATE_MOVING;
            }
            break;

        case STATE_MOVING:
            current_floor = return_legal_floor();
            if (current_floor != -1){
                hardware_command_floor_indicator_on(current_floor);
                previous_legal_floor = current_floor;
            }
                if (complete_orders_floor(&head, current_floor, previous_direction)){
                    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                    remove_completed_orders(&head, current_floor);

                    hardware_command_door_open(1);
                    elevator_state = STATE_DOOR_OPEN;
            }
            break;

        case STATE_DOOR_OPEN:
            if (door_timer(&timer_start, timer_duration, &timer_already_started)){
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
            printf("NO VALID CASE");

        }
    }
}
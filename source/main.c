#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "elevator.h"
#include "order_queue.h"



int main(){
   
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    while (!check_legal_floor()) {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);

    ElevatorState elevator_state; 
    int timer_already_started = 0; 
    clock_t timer_start; 
    double timer_duration = 3; 

    queue_node *head = NULL;


    elevator_state = STATE_IDLE;
    while(1){
        if(hardware_read_stop_signal()){
            elevator_state = STATE_STOP_BUTTON_PRESSED;
            break;
        }
        poll_order_buttons();
        set_floor_indicator();
        
        switch (elevator_state)
        {
        case STATE_IDLE:
            
            break;

        case STATE_MOVING:
            int current_floor = return_legal_floor();
            if (current_floor != -1){
                //Etasjelys på og slukk forrige 
            }
                if (complete_orders_floor(&head, current_floor)){ 
                    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                    hardware_command_door_open(1);
                    //slå av bestillingslys
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
            if (stop_button_handler()){
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
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
    ElevatorState elevator_state; 
    int timer_already_started = 0; 
    clock_t timer_start; 
    double timer_duration = 3; 

    queue_node test_order;
    test_order.floor = 3;
    test_order.order_type = HARDWARE_ORDER_INSIDE;

    hardware_command_door_open(1); //TESTING
    hardware_command_order_light(0, HARDWARE_ORDER_INSIDE, 1);
    hardware_command_order_light(1, HARDWARE_ORDER_INSIDE, 1);
    hardware_command_order_light(2, HARDWARE_ORDER_INSIDE, 1);
    hardware_command_order_light(3, HARDWARE_ORDER_INSIDE, 1);
    hardware_command_order_light(1, HARDWARE_ORDER_UP, 1);
    hardware_command_order_light(2, HARDWARE_ORDER_DOWN, 1);


    printf("Opening the door");
    elevator_state = STATE_DOOR_OPEN;
    while(1){
        if(hardware_read_stop_signal()){
            elevator_state = STATE_STOP_BUTTON_PRESSED;
            break;
        }

        
        switch (elevator_state)
        {
        case STATE_IDLE:
            if (excecute_order_from_idle(test_order)){
                elevator_state = STATE_MOVING;
            }
            break;

        case STATE_MOVING:

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
                elevator_state = STATE_DOOR_OPEN;
            }
        
            break;
        default: 
            printf("NO VALID CASE");

        }
    }
}
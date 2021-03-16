#include <stdio.h>
#include <stdlib.h>

#include "hardware.h"
#include "elevator.h"


int main(){
    
    ElevatorState g_elevator_state; 
    queue_node *g_elevator_order_list = NULL;  

    HardwareMovement g_previous_direction = HARDWARE_MOVEMENT_STOP;
    HardwareMovement g_between_floor_direction = HARDWARE_MOVEMENT_STOP;

    int g_previous_legal_floor;
    int g_current_floor;

    clock_t g_timer_start; 
    int g_timer_already_started = 0; 


    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    elevator_software_init(&g_elevator_state, &g_previous_legal_floor);
    
    while(1){
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            g_elevator_state = STATE_STOP_BUTTON_PRESSED;
        }
        elevator_add_order_if_button_pressed(&g_elevator_order_list);

        switch (g_elevator_state)
        {
        case STATE_IDLE:
            if (g_elevator_order_list != NULL){
                if (g_current_floor == -1){
                   elevator_set_movement_when_between_floors(&g_elevator_order_list, &g_previous_direction, g_between_floor_direction, g_previous_legal_floor);
                }
                else{
                    elevator_set_movement_when_on_floor(&g_elevator_order_list, &g_previous_direction, &g_between_floor_direction, g_current_floor);
                }
                g_elevator_state = STATE_MOVING;
            }
            break;

        case STATE_MOVING:
            if (elevator_complete_order_at_current_floor(&g_elevator_order_list, g_previous_direction, &g_current_floor, &g_previous_legal_floor)){
                hardware_command_door_open(1); //opens door
                g_elevator_state = STATE_DOOR_OPEN;
            }
            break;

        case STATE_DOOR_OPEN:
            if (elevator_door_timer(&g_timer_start, TIMER_DURATION, &g_timer_already_started)){
                hardware_command_door_open(0); //closes door
                g_elevator_state = STATE_IDLE;
            }
            break;

        case STATE_STOP_BUTTON_PRESSED:
            if (elevator_stop_button_pressed(&g_elevator_order_list)){
                g_elevator_state = STATE_IDLE;
            }
            else{
                hardware_command_door_open(1);
                g_elevator_state = STATE_DOOR_OPEN;
            }
            break;
            
        default: 
            fprintf(stderr, "Out of state machine. Shutting down. \n");
            exit(1);

        }
    }
}
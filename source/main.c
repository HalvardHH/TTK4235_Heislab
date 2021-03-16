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

    ElevatorState g_elevator_state; 
    int g_timer_already_started = 0; 
    clock_t g_timer_start = clock(); 
    HardwareMovement g_previous_direction = HARDWARE_MOVEMENT_STOP;
    HardwareMovement g_between_floor_direction = HARDWARE_MOVEMENT_STOP;
    int g_previous_legal_floor;
    int g_current_floor = elevator_current_floor();
    queue_node *head = NULL;  

    elevator_software_init(&g_elevator_state, &g_previous_legal_floor);
    
    while(1){
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            g_elevator_state = STATE_STOP_BUTTON_PRESSED;
        }
        elevator_poll_order_buttons(&head);
        switch (g_elevator_state)
        {
        case STATE_IDLE:
            if (head != NULL){
                if (g_current_floor == -1){
                   elevator_idle_between_floors(&head, &g_previous_direction, g_between_floor_direction, g_previous_legal_floor);
                }
                else{
                    elevator_idle_on_floor(&head, &g_previous_direction, &g_between_floor_direction, g_current_floor);
                }
                g_elevator_state = STATE_MOVING;
            }
            break;

        case STATE_MOVING:
            if (elevator_complete_order_at_current_floor(&head, g_previous_direction, &g_current_floor, &g_previous_legal_floor)){
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
            if (elevator_stop_button_pressed(&head)){
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
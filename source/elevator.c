

#include <stdio.h>
#include <stdlib.h>
#include "elevator.h"
#include "hardware.h"
#include "timer.h"



void elevator_software_init(ElevatorState* g_elevator_state, int* g_previous_legal_floor){
    
    *g_elevator_state = STATE_IDLE;
    elevator_clear_all_order_lights();
    
    while (elevator_current_floor() == -1) {
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            *g_elevator_state = STATE_STOP_BUTTON_PRESSED; 
            fprintf(stderr, "Stop button pressed during initalization. Shutting down. \n");
            exit(1);
        }
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    
    *g_previous_legal_floor = elevator_current_floor();
    hardware_command_floor_indicator_on(*g_previous_legal_floor);
}



int elevator_door_timer(clock_t* g_timer_start, int timer_duration, int* g_timer_already_started){
    if (!*g_timer_already_started){
        *g_timer_start = get_time();
        *g_timer_already_started = 1; 
    }
    if (hardware_read_obstruction_signal()){
        *g_timer_start = get_time();
    }
    if (check_timer_done(*g_timer_start, timer_duration)){
        *g_timer_already_started = 0; 
        return 1;
    }   
    return 0;
}

int elevator_stop_button_pressed(queue_node ** head){
    int between_floors = 1;
    hardware_command_stop_light(1);
    while ((*head) != NULL){
        queue_pop(head);
    }
    
    elevator_clear_all_order_lights();

    if (elevator_current_floor() != -1) {
        hardware_command_door_open(1);
        between_floors = 0;
    }

    while (hardware_read_stop_signal())
    {
        /* do nothing */
    }
    
    hardware_command_stop_light(0);
    if (between_floors){
        return 1;
    }
    return 0; 
    
}

void elevator_clear_all_order_lights(){
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}

int elevator_current_floor() {
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        if (hardware_read_floor_sensor(i)) {
            return i;
        }
    }
    return -1;
}

void elevator_poll_order_buttons(queue_node ** head) { //only turns on light
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        if ( (hardware_read_order(i, HARDWARE_ORDER_INSIDE)) && 
        (!queue_check_duplicate_orders(head, i, HARDWARE_ORDER_INSIDE ))){

            hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 1);
            queue_push_back(head, i, HARDWARE_ORDER_INSIDE);
            
        }
    }
    for (int i = 0; i < HARDWARE_NUMBER_OF_BUTTONS; i++) {
        if (hardware_read_order(i, HARDWARE_ORDER_UP) && 
        (!queue_check_duplicate_orders(head, i, HARDWARE_ORDER_UP ))) {
            
            hardware_command_order_light(i, HARDWARE_ORDER_UP, 1);
            queue_push_back(head, i, HARDWARE_ORDER_UP);

        }
        if (hardware_read_order(i+1, HARDWARE_ORDER_DOWN) && 
        (!queue_check_duplicate_orders(head, i+1, HARDWARE_ORDER_DOWN ))) {
            
            hardware_command_order_light(i+1, HARDWARE_ORDER_DOWN, 1);
            queue_push_back(head, i+1, HARDWARE_ORDER_DOWN);

        }
    }
}

void elevator_set_floor_indicator() {
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        if (hardware_read_floor_sensor(i)) {
            hardware_command_floor_indicator_on(i);
        }
    }
}

void elevator_idle_between_floors(queue_node ** head, HardwareMovement* g_previous_direction, HardwareMovement g_between_floor_direction, int g_previous_legal_floor){
    if (g_previous_legal_floor == (*head)->floor){
        if (g_between_floor_direction == HARDWARE_MOVEMENT_DOWN){
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            *g_previous_direction = HARDWARE_MOVEMENT_UP;
        }
    
         else{
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            *g_previous_direction = HARDWARE_MOVEMENT_DOWN;
        }
    }
                
    else if (g_previous_legal_floor < (*head)->floor){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        *g_previous_direction = HARDWARE_MOVEMENT_UP;
    }
                    
    else{
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        *g_previous_direction = HARDWARE_MOVEMENT_DOWN;
    }    
}
void elevator_idle_on_floor(queue_node ** head, HardwareMovement* g_previous_direction, HardwareMovement* g_between_floor_direction, int g_current_floor){
    if (g_current_floor == (*head)->floor){
        /* Do nothing */
    }
    else if (g_current_floor < (*head)->floor){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        *g_previous_direction = HARDWARE_MOVEMENT_UP;
        *g_between_floor_direction = HARDWARE_MOVEMENT_UP;

    }
    else{
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        *g_previous_direction = HARDWARE_MOVEMENT_DOWN;
        *g_between_floor_direction = HARDWARE_MOVEMENT_DOWN;

    }
}

int elevator_complete_order_at_current_floor(queue_node ** head, HardwareMovement g_previous_direction, int* g_current_floor, int* g_previous_legal_floor){
    *g_current_floor = elevator_current_floor();
    if (*g_current_floor != -1){
        hardware_command_floor_indicator_on(*g_current_floor);
        *g_previous_legal_floor = *g_current_floor;
    }
    
    if (queue_complete_orders_floor(head, *g_current_floor, g_previous_direction)){
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        queue_remove_completed_orders(head, *g_current_floor);
        return 1;
        
    }
    return 0; 
}

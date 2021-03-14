
#include "elevator.h"
#include <stdio.h>
#include <timer.c>
#include "hardware.h"

void elevator_software_init(ElevatorState* elevator_state, int* previous_legal_floor){
    
    *elevator_state = STATE_IDLE;
    clear_all_order_lights();
    
    while (return_legal_floor() == -1) {
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            *elevator_state = STATE_STOP_BUTTON_PRESSED; 
            fprintf(stderr, "Stop button pressed during initalization. Shutting down. \n");
            exit(1);
        }
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    
    *previous_legal_floor = return_legal_floor();
    hardware_command_floor_indicator_on(*previous_legal_floor);
}



int door_timer(clock_t* timer_start, int timer_duration, int* timer_already_started){
    if (!*timer_already_started){
        *timer_start = get_time();
        *timer_already_started = 1; 
    }
    if (hardware_read_obstruction_signal()){
        *timer_start = get_time();
    }
    if (check_timer_done(*timer_start, timer_duration)){
        *timer_already_started = 0; 
        return 1;
    }   
    return 0;
}

int stop_button_handler(queue_node ** head){
    int between_floors = 1;
    hardware_command_stop_light(1);
    while ((*head) != NULL){
        queue_pop(head);
    }
    
    clear_all_order_lights();
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
        if (hardware_read_floor_sensor(i)){
            hardware_command_door_open(1);
            between_floors = 0; 
            break;
        }
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

void clear_all_order_lights(){
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

int return_legal_floor() {
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        if (hardware_read_floor_sensor(i)) {
            return i;
        }
    }
    return -1;
}

void poll_order_buttons(queue_node ** head) { //only turns on light
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

void set_floor_indicator() {
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        if (hardware_read_floor_sensor(i)) {
            hardware_command_floor_indicator_on(i);
        }
    }
}

void idle_between_floors(queue_node ** head, HardwareMovement* previous_direction, HardwareMovement between_floor_direction, int previous_legal_floor){
    if (previous_legal_floor == (*head)->floor){
        if (between_floor_direction == HARDWARE_MOVEMENT_DOWN){
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            *previous_direction = HARDWARE_MOVEMENT_UP;
        }
    
         else{
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            *previous_direction = HARDWARE_MOVEMENT_DOWN;
        }
    }
                
    else if (previous_legal_floor < (*head)->floor){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        *previous_direction = HARDWARE_MOVEMENT_UP;
    }
                    
    else{
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        *previous_direction = HARDWARE_MOVEMENT_DOWN;
    }    
}
void idle_on_floor(queue_node ** head, HardwareMovement* previous_direction, HardwareMovement* between_floor_direction, int current_floor){
    if (current_floor == (*head)->floor){
        /* Do nothing */
    }
    else if (current_floor < (*head)->floor){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        *previous_direction = HARDWARE_MOVEMENT_UP;
        *between_floor_direction = HARDWARE_MOVEMENT_UP;

    }
    else{
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        *previous_direction = HARDWARE_MOVEMENT_DOWN;
        *between_floor_direction = HARDWARE_MOVEMENT_DOWN;

    }
}
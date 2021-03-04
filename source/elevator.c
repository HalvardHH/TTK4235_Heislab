
#include "elevator.h"
#include <stdio.h>
#include <timer.c>
#include "hardware.h"

int door_timer(clock_t* timer_start, int timer_duration, int* timer_already_started){
    if (!*timer_already_started){
        *timer_start = get_time();
        *timer_already_started = 1; 
    }
    if (hardware_read_obstruction_signal()){
        printf("OBSTRUCIUOTN"); //testing only
        *timer_start = get_time();
    }
    if (check_timer_done(*timer_start, timer_duration)){
        printf("closing door");
        *timer_already_started = 0; 
        return 1;
    }   
    return 0;
}

int stop_button_handler(){
    int between_floors = 1;
    hardware_command_stop_light(1);
    //slette ubetjente bestillinger
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


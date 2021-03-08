#include "hardware.h"
#include "order_queue.h"
#include "stdio.h"

queue_node add_node(int floor, HardwareOrder order_type, queue_node* head) {
    queue_node order;
    order.floor = floor;
    order.order_type = order_type;
    head->next = &order;
    return order;
}

void update_orders(queue_node* head) {
    for (int i = 0; i < 4; i++) {
        if (hardware_read_order(i, HARDWARE_ORDER_INSIDE)) {
            add_node(i,HARDWARE_ORDER_INSIDE, head);
        }
        if (hardware_read_order(i, HARDWARE_ORDER_UP)) {
            add_node(i,HARDWARE_ORDER_UP, head);
        }
        if (hardware_read_order(i, HARDWARE_ORDER_DOWN)) {
            add_node(i,HARDWARE_ORDER_DOWN, head);
        }
    }
}

int excecute_order_from_idle(queue_node order) {
    int current_floor = 0;
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
        if (hardware_read_floor_sensor(i)){
            current_floor = i;
            break;
        }
    }
    if (current_floor == order.floor){
        return 0;
    }

    if (order.order_type == HARDWARE_ORDER_INSIDE){
        if (order.floor > current_floor){
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            return 1;
        }

        else{
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            return 1;
        }     
    } 
} 


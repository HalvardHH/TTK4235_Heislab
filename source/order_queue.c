#include "hardware.h"
#include "order_queue.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Adds an order to the front of the list
 */
void push (queue_node ** head, int floor, HardwareOrder order_type){
    queue_node * new_queue_node;
    new_queue_node = (queue_node*) malloc(sizeof(queue_node));
    
    new_queue_node->floor = floor;
    new_queue_node->order_type = order_type;
    
    new_queue_node->next = *head; 
    (*head) = new_queue_node;
    
}
/**
 * @brief Removes the first order.
 */
void pop(queue_node ** head){
    if ((*head) = NULL){
        return;
    }
    queue_node* next_node = (*head)->next;
    free(*head);
    *head = next_node; 
}
/**
 * @brief Removes a spesific order with index i. Head is 0. 
 */
void remove_indexed(queue_node **head, int n){
    queue_node* current = *head; 

    if (n == 0){
        pop(*head);
        return;
    }
    for (int i = 0; i < n-1; i++){
        if (current->next == NULL){
            printf("out of bonds");
            return;
        }
        current = current->next;
    }
    queue_node* temp = current->next;
    current->next = temp->next;
    free(temp);
}


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


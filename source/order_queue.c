#include "hardware.h"
#include "order_queue.h"
#include <stdio.h>
#include <stdlib.h>


void push (queue_node ** head, int floor, HardwareOrder order_type){
    if ((*head) == NULL){
        (*head) = malloc(sizeof(queue_node));
        (*head)->floor = floor;
        (*head)->order_type = order_type;
        (*head)->next = NULL;
        return;
    }
    
    queue_node * new_queue_node;
    new_queue_node = (queue_node*) malloc(sizeof(queue_node));
    
    new_queue_node->floor = floor;
    new_queue_node->order_type = order_type;
    
    new_queue_node->next = *head; 
    (*head) = new_queue_node;
}
void push_back(queue_node ** head, int floor, HardwareOrder order_type){
    if ((*head) == NULL){
        (*head) = malloc(sizeof(queue_node));
        (*head)->floor = floor;
        (*head)->order_type = order_type;
        (*head)->next = NULL;
        return;
    }
    queue_node * current = (*head);
    while (current->next != NULL){
        current = current->next; 
    }
    (current->next) = (queue_node *) malloc(sizeof(queue_node));
    (current->next)->floor = floor;
    (current->next)->order_type = order_type;
    (current->next)->next = NULL;
}
void pop(queue_node ** head){
    if ((*head) == NULL){
        return;
    }
    queue_node* next_node = (*head)->next;
    free(*head);
    *head = next_node; 
}

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

int complete_orders_floor(queue_node **head, int floor){
    int retval = 0; 
    if ((*head) == NULL){
        return retval; 
    }
    while ((*head)->floor == floor){
        pop(head);
        hardware_command_order_light(floor, (*head)->order_type,0);
        retval = 1; 
    }

    queue_node * prev = (*head);
    queue_node * current = (*head)->next;
    
    while (current != NULL){
        if (current->floor == floor){
            
            prev->next = current->next;
            free(current);  
            current = prev->next;
            hardware_command_order_light(floor, current->order_type,0);
            retval = 1; 
        }
        else{
            prev = prev->next;
            current = current->next;
        }
    }
    return retval; 
}

int check_duplicate_orders(queue_node ** head, int floor, HardwareOrder order_type){
    queue_node* current = *head; 
    while (current != NULL){
        if ((current->floor == floor) && (current->order_type == order_type)){
            return 1; 
        }
        current = current->next;
    }
    return 0; 
}
void add_order_ordered(queue_node ** head, int floor, HardwareOrder order_type){ //might be redundant
    
    if ((*head) == NULL){
        push(head, floor, order_type);
        return; 
    }
    queue_node * current = *head; 
    if (order_type == HARDWARE_ORDER_INSIDE){
        while(current->order_type == HARDWARE_ORDER_INSIDE){

        }
    }
}



int is_empty(queue_node **head) {
    if (*head = NULL) {
        return 1;
    }
    return 0;
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


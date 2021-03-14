#include "hardware.h"
#include "order_queue.h"
#include <stdio.h>
#include <stdlib.h>
void queue_pop(queue_node ** head){
    if ((*head) == NULL){
        return;
    }
    queue_node* next_node = (*head)->next;
    free(*head);
    *head = next_node; 
}
void queue_push_back(queue_node ** head, int floor, HardwareOrder order_type){
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

int queue_complete_orders_floor(queue_node **head, int floor, HardwareMovement previous_direction){
    if ((*head) == NULL){
        return 0;
    }
    queue_node * current = (*head);

    if ((*head)->floor == floor){
        if ((*head)->next == NULL){ 
            return 1; 
        }
    }
    
    while (current != NULL){
        if (current->floor == floor){
            if (!queue_check_if_ignore(&current, floor, previous_direction)){
                return 1;
            }
        }
    current = current->next;
    }
    return 0; 

}

void queue_remove_completed_orders(queue_node ** head, int floor){
    while ((*head)->floor == floor){
        hardware_command_order_light(floor, (*head)->order_type,0);
        queue_pop(head);
        
        if ((*head) == NULL){
            return;
        }
    }
    if ((*head) == NULL){
        return;
    }

    queue_node * prev = (*head);
    queue_node * current = (*head)->next;

    while (current != NULL){
        if (current->floor == floor){
            hardware_command_order_light(floor, current->order_type,0);
            prev->next = current->next;
            free(current);  
            current = prev->next;
        }
        else{
            prev = prev->next;
            current = current->next;
        }
    }
}

int queue_check_duplicate_orders(queue_node ** head, int floor, HardwareOrder order_type){
    if (head == NULL){
        return 0;
    }
    queue_node * current = (*head); 
    while (current != NULL){
        if ((current->floor == floor) && (current->order_type == order_type)){
            return 1; 
        }
        current = current->next;
    }
    return 0; 
}

int queue_is_empty(queue_node **head) {
    if ((*head) == NULL) {
        return 1;
    }
    return 0;
} 

int queue_check_if_ignore(queue_node ** head, int current_floor, HardwareMovement previous_direction){
    
    if ((*head)->order_type == HARDWARE_ORDER_INSIDE){
        return 0; 
    }

    if ((*head)->floor == current_floor){

        if (previous_direction == HARDWARE_MOVEMENT_UP){
            if ((*head)->order_type == HARDWARE_ORDER_DOWN && (*head)->floor != HARDWARE_NUMBER_OF_FLOORS-1){
                return 1;
            }
        }
        else if (previous_direction == HARDWARE_MOVEMENT_DOWN && (*head)->floor != 0){
            if ((*head)->order_type == HARDWARE_ORDER_UP){
                return 1; 
            }
        }
    }
    return 0; 
}


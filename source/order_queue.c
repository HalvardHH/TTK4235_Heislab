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

int queue_highest_order(queue_node ** head){
    queue_node * current = *head;
    int retval = 0; 
    while (current != NULL){
        if (current->floor > retval){
            retval = current->floor;
        }
        current = current->next;
    }
    return retval;

}

int queue_lowest_order(queue_node ** head){
    queue_node * current = *head;
    int retval = HARDWARE_NUMBER_OF_FLOORS-1; 
    while (current != NULL){
        if (current->floor < retval){
            retval = current->floor;
        }
    current = current->next;

    }
    return retval;

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
    int lowest_ordered_floor = queue_lowest_order(head);
    int highest_ordered_floor = queue_highest_order(head);
    while (current != NULL){
        if (current->floor == floor){
            if (!queue_check_if_ignore(&current, floor, previous_direction, lowest_ordered_floor, highest_ordered_floor)){
                return 1;
            }
        }
    current = current->next;
    }
    return 0; 

}

void queue_remove_completed_orders(queue_node ** head, int floor){
    
    if ((*head) == NULL){
        return;
    }
    while ((*head)->floor == floor){
        hardware_command_order_light(floor, (*head)->order_type,0);
        queue_pop(head);
        
        if ((*head) == NULL){
            return;
        }
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


int queue_check_if_ignore(queue_node ** head, int current_floor, HardwareMovement previous_direction, int lowest_ordered_floor, int highest_ordered_floor){
    
    if ((*head)->order_type == HARDWARE_ORDER_INSIDE){
        return 0; 
    }

    if ((*head)->floor == current_floor){

        if (previous_direction == HARDWARE_MOVEMENT_UP){
            if ((*head)->order_type == HARDWARE_ORDER_DOWN && (*head)->floor != HARDWARE_NUMBER_OF_FLOORS-1){
                if ((*head)->floor < highest_ordered_floor){
                    return 1;
                }
                return 0;
            }
        }
        else if (previous_direction == HARDWARE_MOVEMENT_DOWN && (*head)->floor != 0){
            if ((*head)->order_type == HARDWARE_ORDER_UP){
                if ((*head)->floor > lowest_ordered_floor){
                    return 1;
                }
                return 0; 
            }
        }
    }
    return 0; 
}





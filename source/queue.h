#include "hardware.h"
#ifndef QUEUE_H
#define QUEUE_H

/**
 * @file
 * @brief Queue library.
 */

/**
 * @brief Node that contains the information for an order. 
 * 
 * @param floor The ordered floor.
 * @param order_type The HardwareOrder type of the order.
 * @param next The next node in the list.
 */
typedef struct node {
    int floor; 
    HardwareOrder order_type;
    struct node* next;
} queue_node;


/**
 * @brief Removes the first order in a queue_node linked_list.
 * 
 * @param head Start of a queue_node linked_list.
 */
void queue_pop(queue_node ** head);

/**
 * @brief Adds an order to the end of the list.
 * 
 * @param head Start of a queue_node linked_list.
 * @param floor The ordered floor.
 * @param order_type The HardwareOrder type of the order.
 */
void queue_push_back(queue_node ** head, int floor, HardwareOrder order_type);

/**
 * @brief Finds the highest ordered floor in a queue_node linked_list.
 * 
 * @param head Start of a queue_node linked_list.
 * 
 * @return The highest ordered floor. 
 */
int queue_highest_order(queue_node ** head);

/**
 * @brief Finds the lowest ordered floor in a queue_node linked_list.
 * 
 * @param head Start of a queue_node linked_list.
 * 
 * @return The lowest ordered floor. 
 */
int queue_lowest_order(queue_node ** head);
/**
 * @brief Checks if the elevator should complete order at a floor.
 * 
 * @param head Start of a queue_node linked_list.
 * @param floor The floor we want to check.
 * @param previous_direction The elevators previous direction.
 * 
 * @return 1 if true, 0 otherwise.
 */
int queue_complete_orders_floor(queue_node **head, int floor, HardwareMovement previous_direction);
/**
 * @brief Removes all orderes to or from selected floor and turns off order lights.
 * 
 * @param head Start of a queue_node linked_list.
 * @param floor The floor we want to remove orders from.
 */
void queue_remove_completed_orders(queue_node ** head, int floor);


/**
 * @brief Checks for duplicate orders already in the queue. 
 * 
 * @param head Start of a queue_node linked_list.
 * @param floor The ordered floor.
 * @param order_type The HardwareOrder type of the order.
 * 
 * @return 1 if a duplicate is in the queue, 0 otherwise. 
 */
int queue_check_duplicate_orders(queue_node ** head, int floor, HardwareOrder order_type);

/**
 * @brief Checks if an order should be ignored.
 * 
 * @param head Start of a queue_node linked_list.
 * @param current_floor The elevators current floor.
 * @param previous_direction The elevators previous direction.
 * @param lowest_ordered_floor The lowest ordered floor in the queue_node linked_list.
 * @param highest_ordered_floor The highest ordered floor in the queue_node linked_list.
 * 
 * @return 1 if ignore, 0 otherwise. 
 */
int queue_check_if_ignore(queue_node ** head, int current_floor, HardwareMovement previous_direction, int lowest_ordered_floor, int highest_ordered_floor);

#endif

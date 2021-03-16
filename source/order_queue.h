#include "hardware.h"
#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H
/**
 * @brief Node that contains the information for a spesific order. 
 */
typedef struct node {
    int floor; 
    HardwareOrder order_type;
    struct node* next;
} queue_node;


/**
 * @brief Removes the first order.
 */
void queue_pop(queue_node ** head);

/**
 * @brief Adds an order to the end of the list
 */
void queue_push_back(queue_node ** head, int floor, HardwareOrder order_type);

/**
 * @brief checks if the elevator should complete order at a floor
 * @return 1 if true , 0 otherwise
 */
int queue_complete_orders_floor(queue_node **head, int floor, HardwareMovement g_previous_direction);
/**
 * @brief removes all orderes to or from selected floor and turns off light
 */
void queue_remove_completed_orders(queue_node ** head, int floor);


/**
 * @brief Checks for duplicate orders already in the queue. 
 * @return 1 if a duplicate is in the queue, 0 otherwise. 
 */
int queue_check_duplicate_orders(queue_node ** head, int floor, HardwareOrder order_type);

/**
 * @brief Checks if the queue is empty.
 * @return 1 if the queue is empy, 0 otherwise.
 */
int queue_is_empty(queue_node **head);

/**
 * @brief Checks if an order should be ignored
 * @return 1 if ignore, 0 otherwise. 
 */
int queue_check_if_ignore(queue_node ** head, int g_current_floor, HardwareMovement g_previous_direction, int lowest_ordered_floor, int highest_ordered_floor);

int queue_highest_order(queue_node ** head);

int queue_lowest_order(queue_node ** head);

#endif

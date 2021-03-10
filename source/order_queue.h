#include "hardware.h"

/**
 * @brief Node that contains the information for a spesific order. 
 */
typedef struct node {
    int floor; 
    HardwareOrder order_type;
    struct node* next;
} queue_node;

/**
 * @brief Adds an order to the front of the list
 */
void push (queue_node ** head, int floor, HardwareOrder order_type);


/**
 * @brief Adds an order to the end of the list
 */
void push_back(queue_node ** head, int floor, HardwareOrder order_type);

/**
 * @brief removes all orderes to or from selected floor and turns off light
 * @return 1 if 1 or more order(s) got completed.
 */
int complete_orders_floor(queue_node **head, int floor);

/**
 * @brief Removes the first order.
 */
void pop(queue_node ** head);


/**
 * @brief Checks for duplicate orders already in the queue. 
 * @return 1 if a duplicate is in the queue, 0 otherwise. 
 */
int check_duplicate_orders(queue_node ** head, int floor, HardwareOrder order_type);

/**
 * @brief Adds an order to the queue at the right place. Assumes the order is not a duplicate. 
 */
void add_order_ordered(queue_node ** head, int floor, HardwareOrder order_type);

/**
 * @brief Checks if the queue is empty.
 * @return 1 if the queue is empy, 0 otherwise.
 */
int is_empty(queue_node **head);


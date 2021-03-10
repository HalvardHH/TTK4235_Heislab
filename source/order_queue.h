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
 * @brief removes all orderes to or from selected floor and turns off light
 * @return 1 if 1 or more order(s) got completed.
 */
int complete_orders_floor(queue_node **head, int floor);

/**
 * @brief Removes the first order.
 */
void pop(queue_node ** head);

/**
 * @brief Removes a spesific order with index i. Head is 0. 
 */
void remove_indexed(queue_node **head, int i);

/**
 * @brief Checks for duplicate orders already in the queue. Return 1 if a duplicate is in the queue. 
 */
int check_duplicate_orders(queue_node ** head, int floor, HardwareOrder order_type);

/**
 * @brief Checks if the queue is empty.
 * @return 1 if the queue is empy, 0 otherwise.
 */
int is_empty(queue_node **head);

/**
 * @brief Unfinished
 */
int excecute_order_from_idle(queue_node order);
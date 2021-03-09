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
void pop(queue_node ** head);
void remove_indexed(queue_node **head, int i);
void complete_orders_floor(queue_node **head, int floor);

/**
 * @brief Removes the first order.
 */
void pop(queue_node ** head);

/**
 * @brief Removes a spesific order with index i. Head is 0. 
 */
void remove_indexed(queue_node **head, int i);

/**
 * @brief Checks if the queue is empty.
 * @return 1 if the queue is empy, 0 otherwise.
 */
int is_empty(queue_node **head);

/**
 * @brief Unfinished
 */
int excecute_order_from_idle(queue_node order);
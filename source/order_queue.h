#include "hardware.h"

typedef struct node {
    int floor; 
    HardwareOrder order_type;
    struct node* next;
} queue_node;

queue_node add_node(int floor, HardwareOrder order_type, queue_node* next_order);

void update_orders(queue_node* head);

int excecute_order_from_idle(queue_node order);
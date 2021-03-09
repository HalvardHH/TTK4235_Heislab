#include "hardware.h"

typedef struct node {
    int floor; 
    HardwareOrder order_type;
    struct node* next;
} queue_node;

void push (queue_node ** head, int floor, HardwareOrder order_type);
void pop(queue_node ** head);
void remove_indexed(queue_node **head, int i);


queue_node add_node(int floor, HardwareOrder order_type, queue_node* next_order);

void update_orders(queue_node* head);

int excecute_order_from_idle(queue_node order);
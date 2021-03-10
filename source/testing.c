#include "timer.c"
#include "order_queue.h"

#include <stdio.h>
void timer_test(){
    printf("Starting a timer: ");
    time_t timer_start = get_time();
    
    while(!check_timer_done(timer_start, 3)){
    }
    printf("%lf", difftime(get_time(), timer_start));
    printf("timer is done!");
}
void print_order_queue(queue_node * head){
    printf("\n Printing order queue: \n");
    queue_node * current = head;
    while (current != NULL){
        printf("%d\n", current->floor);
        current = current->next;
    }
}

void order_queue_test(){
    queue_node * head = NULL;
    push(&head, 2, HARDWARE_ORDER_INSIDE);
    push(&head, 1, HARDWARE_ORDER_UP);
    push(&head, 2, HARDWARE_ORDER_UP);
    push(&head, 3, HARDWARE_ORDER_UP);
    push(&head, 4, HARDWARE_ORDER_UP);
    push(&head, 1, HARDWARE_ORDER_DOWN);
    push(&head, 1, HARDWARE_ORDER_INSIDE);
    print_order_queue(head);
    printf("POPPING ALL!: ");
    while (head != NULL){
        pop(&head);
    }
    print_order_queue(head);
    push(&head, 2, HARDWARE_ORDER_INSIDE);
    push(&head, 2, HARDWARE_ORDER_UP);
    push(&head, 3, HARDWARE_ORDER_UP);
    push(&head, 4, HARDWARE_ORDER_UP);
    push(&head, 1, HARDWARE_ORDER_UP);
    push(&head, 2, HARDWARE_ORDER_DOWN);
    push(&head, 3, HARDWARE_ORDER_INSIDE);
    push(&head, 4, HARDWARE_ORDER_INSIDE);

    print_order_queue(head);
    complete_orders_floor(&head, 1);
    printf("completed all orders at floor 1");
    print_order_queue(head);
    push(&head, 2, HARDWARE_ORDER_UP);
    push(&head, 2, HARDWARE_ORDER_UP);
    push(&head, 2, HARDWARE_ORDER_UP);
    push(&head, 2, HARDWARE_ORDER_DOWN);
    print_order_queue(head);
    printf("Completed all orders at floor 2");
    complete_orders_floor(&head, 2);
    print_order_queue(head);
    if (check_duplicate_orders(&head, 3, HARDWARE_ORDER_INSIDE)){
        print("DUPLICATE!");
    }
    if (!check_duplicate_orders(&head, 2, HARDWARE_ORDER_INSIDE)){
        print("NOT DUPLICATE!");
    }


    printf("DONE!");
}


#include <time.h>
#include "order_queue.h"

/**
 * @brief States for the state machine that controls the elevator.
 */
typedef enum {
    STATE_IDLE,
    STATE_MOVING,
    STATE_DOOR_OPEN,
    STATE_STOP_BUTTON_PRESSED,
    
} ElevatorState;

/**
 * @brief Turns off the all the order lights. 
 */
void clear_all_order_lights();

/**
 * @brief Starts a timer to hold the door open for a specified amout of time. If there is an obstruction the timer will restart. 
 */
int door_timer(clock_t* timer_start, int timer_duration, int* timer_already_started);

/**
 * @brief Turns on the stoplight, deletes orders and opens the door if the elevator is at a legal floor.
 */
int stop_button_handler(queue_node ** head);

/**
 * @brief Checks if the elevator is at a legal floor.
 * @return the floor number if the elevator is at a legal floor, -1 otherwise. 
 */
int return_legal_floor();

/**
 * @brief Checks if any of the order buttons have been pressed. Turns on the corresponding lights and adds orders to queue.
 */
void poll_order_buttons();

/**
 * @brief Turns on the light corresponding to the current floor.
 */
void set_floor_indicator();
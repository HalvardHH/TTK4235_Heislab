
#include <time.h>
#include "order_queue.h"

#ifndef ELEVATOR_H
#define ELEVATOR_H
#define TIMER_DURATION 3


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
 * @brief Initializes the elevator states and starting position.
 * Must be called once before other calls to the elevator control software. 
 *
 * @return 0 on success. Non-zero for failure.
 */
void elevator_software_init(ElevatorState* elevator_state, int* previous_legal_floor);

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

/**
 * @brief Sets movement when idle between floors. 
 */
void idle_between_floors(queue_node ** head, HardwareMovement* previous_direction, HardwareMovement between_floor_direction, int previous_legal_floor);

/**
 * @brief Sets movement when idle on a floor. 
 */
void idle_on_floor(queue_node ** head, HardwareMovement* previous_direction, HardwareMovement* between_floor_direction, int current_floor);

/**
 * @brief Decides if the elevator should stop at a floor and complete an order. 
 * @return 1 if the elevator should stop and complete order. 0 otherwise. 
 */
int moving_handler(queue_node ** head, HardwareMovement previous_direction, int* current_floor, int* previous_legal_floor);

#endif

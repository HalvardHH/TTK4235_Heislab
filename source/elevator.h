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
 * @brief Initializes g_elevator_state and the elevators starting position. 
 * Changes g_previous_legal_floor
 * Must be called once before other calls to the elevator control software.
 *
 * @param g_elevator_state Current state of the elevator
 * @param g_previous_legal_floor The previous floor the elevator was in
 * 
 * @return 0 on success. Non-zero for failure.
 */
void elevator_software_init(ElevatorState* g_elevator_state, int* g_previous_legal_floor);

/**
 * @brief Turns off the all the order lights. 
 */
void elevator_clear_all_order_lights();

/**
 * @brief Starts a timer to hold the door open for a specified amout of time. If there is an obstruction the timer will restart. 
 * 
 * 
 * @param timer_start The timers start time.
 * @param timer_duration The wanted duration of the timer
 * @param timer_already_started Bool to check if has previously been started. 1 if yes, 0 otherwise.  
 * 
 * @return 1 if the timer is done, 0 otherwise. 
 */
int elevator_door_timer(clock_t* timer_start, int timer_duration, int* timer_already_started);

/**
 * @brief Turns on the stoplight, deletes orders and opens the door if the elevator is at a legal floor.
 * @param head Start of a queue_node linked_list. 
 * @param timer_already_started Bool to check if the timer has been started. 1 if started, 0 otherwise
 * 
 * @return 1 if the elevator is between floors, 0 otherwise. 
 */
int elevator_stop_button_pressed(queue_node ** head, int* timer_already_started);

/**
 * @brief Checks if the elevator is at a legal floor.
 * @return the floor number if the elevator is at a legal floor, -1 otherwise. 
 */
int elevator_current_floor();

/**
 * @brief Checks if any of the order buttons have been pressed. 
 * Turns on the corresponding lights and adds orders to queue.
 * @param head Start of a queue_node linked_list.
 */
void elevator_add_order_if_button_pressed(queue_node ** head);

/**
 * @brief Turns on the light corresponding to the current floor.
 */
void elevator_set_floor_indicator();

/**
 * @brief Sets movement when idle between floors. 
 * 
 * @param head Start of a queue_node linked_list.
 * @param previous_direction The previous HardwareMovement direction of the elevator.
 * @param between_floor_direction The previous HardwareMovement direction of the elevator when it left a floor. 
 * @param previous_legal_floor The previous floor the elevator was in.
 */
void elevator_set_movement_when_between_floors(queue_node ** head, HardwareMovement* previous_direction, HardwareMovement between_floor_direction, int previous_legal_floor);

/**
 * @brief Sets movement when idle on a floor. 
 * 
 * @param head Start of a queue_node linked_list.
 * @param between_floor_direction The previous HardwareMovement direction of the elevator when it left a floor. 
 * @param previous_legal_floor The previous floor the elevator was in.
 */
void elevator_set_movement_when_on_floor(queue_node ** head, HardwareMovement* previous_direction, HardwareMovement* between_floor_direction, int current_floor);

/**
 * @brief Decides if the elevator should stop at a floor and complete an order. 
 * @param head Start of a queue_node linked_list.
 * @param previous_direction The previous HardwareMovement direction of the elevator.
 * @param current_floor The current floor of the elevator. 
 * @param previous_legal_floor The previous floor the elevator was in.
 * @return 1 if the elevator should stop and complete order. 0 otherwise. 
 */
int elevator_complete_order_at_current_floor(queue_node ** head, HardwareMovement previous_direction, int* current_floor, int* previous_legal_floor);

#endif

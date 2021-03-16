
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
void elevator_software_init(ElevatorState* g_elevator_state, int* g_previous_legal_floor);

/**
 * @brief Turns off the all the order lights. 
 */
void elevator_clear_all_order_lights();

/**
 * @brief Starts a timer to hold the door open for a specified amout of time. If there is an obstruction the timer will restart. 
 */
int elevator_door_timer(clock_t* g_timer_start, int timer_duration, int* g_timer_already_started);

/**
 * @brief Turns on the stoplight, deletes orders and opens the door if the elevator is at a legal floor.
 */
int elevator_stop_button_pressed(queue_node ** head);

/**
 * @brief Checks if the elevator is at a legal floor.
 * @return the floor number if the elevator is at a legal floor, -1 otherwise. 
 */
int elevator_current_floor();

/**
 * @brief Checks if any of the order buttons have been pressed. Turns on the corresponding lights and adds orders to queue.
 */
void elevator_poll_order_buttons();

/**
 * @brief Turns on the light corresponding to the current floor.
 */
void elevator_set_floor_indicator();

/**
 * @brief Sets movement when idle between floors. 
 */
void elevator_idle_between_floors(queue_node ** head, HardwareMovement* g_previous_direction, HardwareMovement g_between_floor_direction, int g_previous_legal_floor);

/**
 * @brief Sets movement when idle on a floor. 
 */
void elevator_idle_on_floor(queue_node ** head, HardwareMovement* g_previous_direction, HardwareMovement* g_between_floor_direction, int g_current_floor);

/**
 * @brief Decides if the elevator should stop at a floor and complete an order. 
 * @return 1 if the elevator should stop and complete order. 0 otherwise. 
 */
int elevator_complete_order_at_current_floor(queue_node ** head, HardwareMovement g_previous_direction, int* g_current_floor, int* g_previous_legal_floor);

#endif

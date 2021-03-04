
#include <time.h>

/**
 * @brief States for the state machine that controls the elevator.
 */
typedef enum {
    STATE_IDLE,
    STATE_MOVING,
    STATE_DOOR_OPEN,
    STATE_STOP_BUTTON_PRESSED,
    
} ElevatorState;
void clear_all_order_lights();
int door_timer(clock_t* timer_start, int timer_duration, int* timer_already_started);
int stop_button_handler();

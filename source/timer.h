#include <time.h>
#ifndef TIMER_H
#define TIMER_H

/** 
 * @file
 * @brief Timer library.
 */

/**
 * @brief Returns current time.
 */
clock_t timer_get_time();
/**
 * @brief Checks if a timer is done
 * @param start Start time for the timer.
 * @param timer_duration Timer duration. 
 * @return 1 if timer is done, else 0. 
 */
int timer_check_timer_done(clock_t start, double timer_duration);

#endif
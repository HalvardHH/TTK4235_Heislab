
#include <time.h>
#ifndef TIMER_H
#define TIMER_H
int g_timer_already_started = 0; 

/** 
 * @file
 * @brief Timer library
 */
 #define TIMER_DURATION 3
/**
 * @brief Returns current time.
 */
clock_t get_time();

/**
 * @brief Checks if a timer is done
 * @param start Start time for the timer.
 * @param timer_duration Timer duration. 
 * @return 1 if timer is done, else 0. 
 */
int check_timer_done(clock_t start, double timer_duration);

#endif
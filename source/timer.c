#include <stdlib.h> 
#include <time.h>
/** 
 * @file
 * @brief Timer library
 */
 
/**
 * @brief Returns current time.
 */

#define TIMER_DURATION 3

clock_t get_time(){
    return clock();
}

/**
 * @brief Checks if a timer is done
 * @param start Start time for the timer.
 * @param timer_duration Timer duration. 
 * @return 1 if timer is done, else 0. 
 */
int check_timer_done(clock_t start, double timer_duration){
    if ((clock()/CLOCKS_PER_SEC - start/CLOCKS_PER_SEC) >= timer_duration){
        return 1; 
    }
    else{
        return 0; 
    }
}

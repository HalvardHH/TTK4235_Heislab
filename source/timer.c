#include "timer.h"

/**
 * @file
 * @brief Implementation file for the timer.
 */

clock_t timer_get_time(){
    return clock();
}

int timer_check_timer_done(clock_t start, double timer_duration){
    if ((clock()/CLOCKS_PER_SEC - start/CLOCKS_PER_SEC) >= timer_duration){
        return 1; 
    }
    else{
        return 0; 
    }
}

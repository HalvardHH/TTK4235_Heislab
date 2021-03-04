#include <stdlib.h> 
#include <time.h>
/**
 * @brief Returns current time.
 */
time_t get_time(){
    return time(NULL);
}

/**
 * @brief Checks if a timer is done
 * @param start Start time for the timer.
 * @param duration Timer duration. 
 * @return 1 if timer is done, else 0. 
 */
int check_timer_done(time_t start, double duration){
    if (difftime(get_time(), start) >= duration){
        return 1; 
    }
    else{
        return 0; 
    }
}


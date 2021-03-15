#include "timer.h"

clock_t get_time(){
    return clock();
}

int check_timer_done(clock_t start, double timer_duration){
    if ((clock()/CLOCKS_PER_SEC - start/CLOCKS_PER_SEC) >= timer_duration){
        return 1; 
    }
    else{
        return 0; 
    }
}

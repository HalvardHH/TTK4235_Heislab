#include <stdlib.h> 
#include <time.h>

time_t get_time(){
    return time(NULL);
};

double check_timer_done(time_t start, time_t end, double duration){
    if (difftime(end, start) >= duration){
        return 1; 
    }
    else{
        return 0; 
    }
};


#include "timer.c"

#include <stdio.h>
void timer_test(){
    printf("Starting a timer: ");
    time_t timer_start = get_time();
    
    while(!check_timer_done(timer_start, 3)){
    };
    printf("%lf", difftime(get_time(), timer_start));
    printf("timer is done!");
}
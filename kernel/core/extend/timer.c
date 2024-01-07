#include <types.h>

#include <extend/timer.h>
#include <extend/intr.h>

uint64_t current_ticks = 0;

// Increment the timer by one
void increment_timer() {
    current_ticks++;
}

// Waits for n seconds
void wait(int seconds) {
    uint64_t end = current_ticks + PIT_CHANNEL0_FREQUENCY * seconds;
    while(current_ticks < end); // Waiting until condition met
}

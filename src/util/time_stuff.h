#include <time.h>

long millis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000L) + (ts.tv_nsec / 1000000L);
}

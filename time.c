#include "codexion.h"

void start_timer(s_timer *t)
{
    gettimeofday(&t->start, NULL);
}

int stop_timer(s_timer *t)
{
    struct timeval now;
    int elapsed;

    gettimeofday(&now, NULL);
    elapsed =
        ((now.tv_sec - t->start.tv_sec) * 1000)
        + ((now.tv_usec - t->start.tv_usec) / 1000);
    return (elapsed);
}

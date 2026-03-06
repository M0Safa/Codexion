#include "codexion.h"

void start_timer(s_timer *t)
{
    gettimeofday(&t->start, NULL);
}

int stop_timer(s_timer *t)
{
    gettimeofday(&t->end, NULL);

    t->elapsed =
        ((t->end.tv_sec - t->start.tv_sec) * 1000)
        + ((t->end.tv_usec - t->start.tv_usec) / 1000);
    return(t->elapsed);
}

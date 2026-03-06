#include "codexion.h"

void unlock_dongle(s_dongle *d)
{
    pthread_mutex_lock(&d->mutex);
    start_timer(&d->time);
    d->check_time = 1;
    pthread_mutex_unlock(&d->mutex);
}

void release_dongle(s_dongle *d)
{
    pthread_mutex_lock(&d->mutex);
    d->available = 1;
    d->check_time = 0;
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->mutex);
}

void request_dongle(s_dongle *d, s_coder *coder)
{
    pthread_mutex_lock(&d->mutex);
    enqueue(&d->queue, coder->id, stop_timer(&coder->time), (coder->par).edf);
    while (!d->available || front(&d->queue) != coder->id)
        pthread_cond_wait(&d->cond, &d->mutex);
    d->available = 0;
    dequeue(&d->queue);
    printing(coder, 4);
    pthread_mutex_unlock(&d->mutex);
}
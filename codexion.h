# ifndef CODEXION_H
# define CODEXION_H

#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    struct timeval start;
    struct timeval end;
    int elapsed;
}s_timer;

typedef struct s_node
{
    int coder_id;
    int duration;
}   t_node;

typedef struct s_queue
{
    t_node front;
    t_node rear;
    s_timer time;
}   t_queue;

typedef struct {
    int nb_coders;
    int burnout;
    int compile;
    int debug;
    int refactor;
    int nb_of_compiles;
    int cooldown;
    int edf;
}s_parameters;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int available;
    int check_time;
    s_timer time;
    t_queue queue;
}s_dongle;

typedef struct {
    int id;
    int nb_compiles;
    int check_time;
    int *stop;
    s_timer time;
    s_timer *g_time;
    s_dongle *left;
    s_dongle *right;
    pthread_mutex_t *print;
    pthread_mutex_t mutex;
    s_parameters par;
}s_coder;

typedef struct{
    s_coder *coders;
    s_dongle *dongles;
    s_parameters par;
}s_monitor;



void start_timer(s_timer *t);
int stop_timer(s_timer *t);
void enqueue(t_queue *q, int id, int duration, int edf);
void dequeue(t_queue *q);
int front(t_queue *q);
void unlock_dongle(s_dongle *d);
void release_dongle(s_dongle *d, int cooldown);
void request_dongle(s_dongle *d, s_coder *coder);
void *coder_routine(void *arg);
void *monitoring(void *args);
int printing(s_coder *coder, int task);
int what_time(s_coder *coder);

#endif
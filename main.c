#include "codexion.h"

void printing(s_coder *coder, int task)
{
    int i;
    if (*(coder->stop) == 1)
        return ;
    pthread_mutex_lock(coder->print);
    i = stop_timer(coder->g_time);
    if (task == 1)
        printf("%d  %d is compiling\n", i, coder->id);
    if (task == 2)
        printf("%d  %d is debugging\n", i, coder->id);
    if (task == 3)
        printf("%d  %d is refactoring\n", i, coder->id);
    if (task == 4)
        printf("%d  %d has taken a dongle\n",i, coder->id);
    pthread_mutex_unlock(coder->print);
}

s_coder *init_coders(s_parameters par, s_dongle *dongles)
{
    s_coder * coders;
    pthread_mutex_t *print;
    int *stop;
    s_timer *g_time;
    int i;

    i = 0;
    print = malloc(sizeof(pthread_mutex_t));
    stop = malloc(sizeof(int));
    g_time = malloc(sizeof(s_timer));
    coders = malloc(sizeof(s_coder) * par.nb_coders);
    pthread_mutex_init(print, NULL);
    *stop = 0;
    while(i < par.nb_coders)
    {
        coders[i].id = i + 1;
        coders[i].nb_compiles = 0;
        coders[i].check_time = 0;
        coders[i].nb_compiles = 0;
        coders[i].g_time = g_time;
        coders[i].right = &dongles[i];
        coders[i].stop = stop;
        coders[i].print = print;
        if (i == 0)
            coders[i].left = &dongles[par.nb_coders - 1];
        else
            coders[i].left = &dongles[i - 1];
        coders[i].par = par;
        i++;
    }
    return (coders);
}

s_dongle *init_dongles(s_parameters par)
{
    s_dongle *dongles;
    int i;

    dongles = malloc(sizeof(s_dongle) * par.nb_coders);
    i = 0;
    while(i < par.nb_coders)
    {
        pthread_mutex_init(&dongles[i].mutex, NULL);
        pthread_cond_init(&dongles[i].cond, NULL);
        dongles[i].available = 1;
        dongles[i].check_time = 0;
        dongles[i].queue.rear = NULL;
        dongles[i].queue.front = NULL;
        i++;
    }
    return (dongles);
}

void ft_clean(s_coder *coders, s_dongle *dongles, int nb_coders)
{
    int i;

    i = 0;
    pthread_mutex_destroy(coders[0].print);
    free(coders[0].print);
    free(coders[0].stop);
    free(coders[0].g_time);
    while(i < nb_coders)
    {
        pthread_mutex_destroy(&dongles[i].mutex);
        pthread_cond_destroy(&dongles[i].cond);
        i++;
    }
    free(coders);
    free(dongles);
}

int is_int(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    if (i > 10)
        return (0);
    return (1);
}

s_parameters validation(char **argv)
{
    s_parameters par;
    int i;

    i = 1;
    par.nb_coders = atoi(argv[1]);
    par.burnout = atoi(argv[2]);
    par.compile = atoi(argv[3]);
    par.debug = atoi(argv[4]);
    par.refactor = atoi(argv[5]);
    par.nb_of_compiles = atoi(argv[6]);
    par.cooldown = atoi(argv[7]);
    while (i < 8)
    {
        if (!is_int(argv[i++]))
            par.nb_coders = -1;
    }
    if(strcmp(argv[8], "fifo") == 0)
        par.edf = 0;
    else if(strcmp(argv[8], "edf") == 0)
        par.edf = 1;
    else
        par.nb_coders = -1;
    return (par);
}

int main(int argc, char **argv)
{
    int i;
    s_parameters par;
    pthread_t *threads;
    s_monitor arg;
    s_coder *coders;
    s_dongle *dongles;

    if (argc != 9)
        return (0 * printf("error: no enough argument.\n"));
    par = validation(argv);
    if (par.nb_coders == -1)
        return (0 * printf("error: you enter an invalid argument.\n"));
    dongles = init_dongles(par);
    coders = init_coders(par, dongles);
    threads = malloc(sizeof(pthread_t) * (par.nb_coders + 1));
    arg.coders = coders;
    arg.dongles =dongles;
    arg.par = par;
    start_timer(coders[0].g_time);
    i = -1;
    while (i++ < par.nb_coders - 1)
        pthread_create(&threads[i], NULL, coder_routine, (void *) &coders[i]);
    pthread_create(&threads[par.nb_coders], NULL, monitoring, (void *) &arg);
    pthread_join(threads[par.nb_coders], NULL);
    ft_clean(coders, dongles, par.nb_coders);
    return (0);
}

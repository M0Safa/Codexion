#include "codexion.h"

int what_time(s_coder *coder)
{
    int i;

    pthread_mutex_lock(&coder->mutex);
    if(coder->check_time)
        i = stop_timer(&coder->time);
    else
        i = 0;
    pthread_mutex_unlock(&coder->mutex);
    return (i);
}

void *coder_routine(void *arg)
{
    s_coder *coder;

    coder = (s_coder *) arg;
    while ((coder->nb_compiles < (coder->par).nb_of_compiles) && !*(coder->stop))
    {
        pthread_mutex_lock(&coder->mutex);
        start_timer(&coder->time);
        coder->check_time = 1;
        pthread_mutex_unlock(&coder->mutex);
        if(coder->id % 2 == 0)
        {
            request_dongle(coder->left, coder);
            request_dongle(coder->right, coder);
        }
        else
        {
            request_dongle(coder->right, coder);
            request_dongle(coder->left, coder);
        }
        pthread_mutex_lock(&coder->mutex);
        coder->nb_compiles ++;
        coder->check_time = 0;
        pthread_mutex_unlock(&coder->mutex);
        printing(coder, 1);
        usleep((coder->par).compile * 1000);
        unlock_dongle(coder->right);
        unlock_dongle(coder->left);
        if (!printing(coder, 2))
            break;
        usleep((coder->par).debug * 1000);
        if(!printing(coder, 3))
            break;
        usleep((coder->par).refactor * 1000);
    }
    coder->nb_compiles = -1;
    return (NULL);
}

void *monitoring(void *args)
{
    s_monitor *arg;
    int i;
    int flag;

    arg = (s_monitor *) args;
    flag = 1;
    while (flag)
    {
        i = 0;
        flag = 0;
        while (i < (arg->par).nb_coders)
        {
            if(!*((arg->coders[i]).stop))
            {
                if(what_time(&(arg->coders[i])) > (arg->par).burnout)
                    printing(&(arg->coders[i]), 5);
            }
            if ((arg->coders[i]).nb_compiles != -1)
                flag = 1;
            release_dongle(&(arg->dongles[i]), (arg->par).cooldown);
            i++;
        }
        usleep(500);
    }
    return (NULL);
}
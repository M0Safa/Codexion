#include "codexion.h"

void *coder_routine(void *arg)
{
    s_coder *coder;
    int stoper;

    stoper = 0;
    coder = (s_coder *) arg;
    while ((coder->nb_compiles < (coder->par).nb_of_compiles) && !stoper)
    {
        start_timer(&coder->time);
        coder->check_time = 1;
        if(coder->id % 2 == 0)
        {
            request_dongle(coder->right, coder);
            request_dongle(coder->left, coder);
        }
        else
        {
            request_dongle(coder->left, coder);
            request_dongle(coder->right, coder);
        }
        coder->check_time = 0;
        printing(coder, 1);
        usleep((coder->par).compile * 1000);
        unlock_dongle(coder->right);
        unlock_dongle(coder->left);
        coder->nb_compiles ++;
        printing(coder, 2);
        usleep((coder->par).debug * 1000);
        printing(coder, 3);
        usleep((coder->par).refactor * 1000);
        stoper = *(coder->stop);
    }
    coder->nb_compiles = -1;
    return (NULL);
}

void *monitoring(void *args)
{
    s_monitor *arg;
    int i;
    int flag;
    int stop;

    arg = (s_monitor *) args;
    flag = 1;
    stop = 0;
    while (flag)
    {
        i = 0;
        flag = 0;
        while (i < (arg->par).nb_coders)
        {
            if((arg->coders[i]).check_time && !stop)
            {
                if(stop_timer(&(arg->coders[i]).time) > (arg->par).burnout)
                {
                    *((arg->coders[i]).stop) = 1;
                    stop = 1;
                    printf("%d  %d burned out\n", stop_timer((arg->coders[i]).g_time), (arg->coders[i]).id);
                }
            }
            if ((arg->coders[i]).nb_compiles != -1)
                flag = 1;
            if (stop_timer(&(arg->dongles[i]).time) > (arg->par).cooldown
                && (arg->dongles[i]).check_time)
                release_dongle(&(arg->dongles[i]));
            i++;
        }
        usleep(100);
    }
    return (NULL);
}
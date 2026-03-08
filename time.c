#include "codexion.h"

void	start_timer(t_timer *t)
{
	gettimeofday(&(t->start), NULL);
}

long	stop_timer(t_timer *t)
{
	struct		timeval now;
	long long	seconds;
	long long	useconds;

	gettimeofday(&now, NULL);
	seconds = now.tv_sec - t->start.tv_sec;
	useconds = now.tv_usec - t->start.tv_usec;
	return ((long)((seconds * 1000) + (useconds / 1000)));
}

long	what_time(t_coder *coder)
{
	long	i;

	pthread_mutex_lock(&coder->mutex);
	if (coder->check_time)
		i = stop_timer(&coder->time);
	else
		i = 0;
	pthread_mutex_unlock(&coder->mutex);
	return (i);
}

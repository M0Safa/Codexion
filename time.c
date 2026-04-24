/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mosafa <mosafa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 21:42:55 by mosafa            #+#    #+#             */
/*   Updated: 2026/03/09 21:42:55 by mosafa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	start_timer(t_timer *t)
{
	gettimeofday(&(t->start), NULL);
}

long	stop_timer(t_timer *t)
{
	struct timeval	now;
	long long		start_us;
	long long		now_us;

	gettimeofday(&now, NULL);
	start_us = ((long long)t->start.tv_sec * 1000LL)
		+ ((long long)t->start.tv_usec / 1000LL);
	now_us = ((long long)now.tv_sec * 1000LL)
		+ ((long long)now.tv_usec / 1000LL);
	return ((long)(now_us - start_us));
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


bool	ft_usleep(t_coder *coder, long usec)
{
	struct timespec	ts;
	struct timeval	tv;
	int				result;

	pthread_mutex_lock(&coder->mutex);
	if (!printing(coder, 0))
	{
		pthread_mutex_unlock(&coder->mutex);
		return (false);
	}
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + (usec / 1000000);
	ts.tv_nsec = (tv.tv_usec + (usec % 1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000;
	}
	result = pthread_cond_timedwait(&coder->cond, &coder->mutex, &ts);
	pthread_mutex_unlock(&coder->mutex);

	if (result == ETIMEDOUT)
		return (true);
	return (false);
}

void	ft_sleep(long usec)
{
	struct timespec	ts;
	struct timeval	tv;
	pthread_mutex_t	dummy_mutex;
	pthread_cond_t	dummy_cond;

	pthread_mutex_init(&dummy_mutex, NULL);
	pthread_cond_init(&dummy_cond, NULL);
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + (usec / 1000000);
	ts.tv_nsec = (tv.tv_usec + (usec % 1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000;
	}
	pthread_mutex_lock(&dummy_mutex);
	pthread_cond_timedwait(&dummy_cond, &dummy_mutex, &ts);
	pthread_mutex_unlock(&dummy_mutex);
	pthread_mutex_destroy(&dummy_mutex);
	pthread_cond_destroy(&dummy_cond);
}

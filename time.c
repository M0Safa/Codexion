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

void	ft_usleep(long usec)
{
	struct timeval	start;
	struct timeval	now;
	long			elapsed;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&now, NULL);
		elapsed = (now.tv_sec - start.tv_sec) * 1000000 + 
		          (now.tv_usec - start.tv_usec);
		if (elapsed >= usec)
			break ;
	}
}
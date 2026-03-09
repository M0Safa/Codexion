/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mosafa <mosafa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 21:41:30 by mosafa            #+#    #+#             */
/*   Updated: 2026/03/09 21:41:30 by mosafa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	unlock_dongle(t_dongle *dl, t_dongle *dr)
{
	pthread_mutex_lock(&dl->mutex);
	pthread_mutex_lock(&dr->mutex);
	start_timer(&dl->time);
	start_timer(&dr->time);
	dl->available = 1;
	dr->available = 1;
	pthread_mutex_unlock(&dl->mutex);
	pthread_mutex_unlock(&dr->mutex);
}

bool	dongle_avail(t_dongle *dl, t_dongle *dr, int c)
{
	bool	avail;

	pthread_mutex_lock(&dl->mutex);
	pthread_mutex_lock(&dr->mutex);
	if (!(dl->available) || !(dr->available))
		avail = false;
	else if (stop_timer(&dl->time) < c || stop_timer(&dr->time) < c)
		avail = false;
	else
		avail = true;
	pthread_mutex_unlock(&dl->mutex);
	pthread_mutex_unlock(&dr->mutex);
	return (avail);
}

void	lock_dongle(t_dongle *dl, t_dongle *dr)
{
	pthread_mutex_lock(&dl->mutex);
	pthread_mutex_lock(&dr->mutex);
	dl->available = 0;
	dr->available = 0;
	pthread_mutex_unlock(&dl->mutex);
	pthread_mutex_unlock(&dr->mutex);
}

bool	request_dongle(t_coder *coder, t_dongle *dl, t_dongle *dr)
{
	pthread_mutex_lock(coder->queue_lock);
	enqueue(coder->queue, coder);
	pthread_mutex_unlock(coder->queue_lock);
	while (true)
	{
		pthread_mutex_lock(coder->queue_lock);
		if (front(*(coder->queue), (coder->par).edf) == coder->id)
		{
			lock_dongle(dl, dr);
			queue_pop(coder->queue, coder->id);
			pthread_mutex_unlock(coder->queue_lock);
			printing(coder, 4);
			return (1);
		}
		pthread_mutex_unlock(coder->queue_lock);
		if (*(coder->stop))
		{
			pthread_mutex_lock(coder->queue_lock);
			queue_pop(coder->queue, coder->id);
			pthread_mutex_unlock(coder->queue_lock);
			return (0);
		}
		usleep(100);
	}
}

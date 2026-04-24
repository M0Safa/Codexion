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
	t_dongle	*first;
	t_dongle	*second;

	if (dl < dr)
	{
		first = dl;
		second = dr;
	}
	else 
	{
		first = dr;
		second = dl;
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	start_timer(&first->time);
	start_timer(&second->time);
	first->check_time = true;
	first->available = true;
	second->check_time = true;
	second->available = true;
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
}

bool	dongle_time(t_dongle *d, int c)
{
	if (d->check_time)
		return (stop_timer(&d->time) > c);
	else
		return (true);
}

bool	dongle_avail(t_dongle *dl, t_dongle *dr, int c)
{
	bool		avail;
	t_dongle	*first;
	t_dongle	*second;

	if (dl < dr) { first = dl; second = dr; }
	else { first = dr; second = dl; }

	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);

	avail = true;
	if (!dl->available || !dr->available)
		avail = false;
	else if (dl->check_time && stop_timer(&dl->time) < c)
		avail = false;
	else if (dr->check_time && stop_timer(&dr->time) < c)
		avail = false;

	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
	return (avail);
}

void	lock_dongle(t_coder *c)
{
	t_dongle	*first;
	t_dongle	*second;

	if (c->left < c->right)
	{
		first = c->left;
		second =  c->right;
	}
	else 
	{
		first =  c->right;
		second = c->left;
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	first->available = 0;
	second->available = 0;
	printing(c, 4);
	printing(c, 4);
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
}

bool	request_dongle(t_coder *coder)
{
	pthread_mutex_lock(coder->queue_lock);
	enqueue(coder->queue, coder);
	pthread_mutex_unlock(coder->queue_lock);
	pthread_mutex_lock(&coder->mutex);
    while (coder->check_time)
    {
        pthread_cond_wait(&coder->cond, &coder->mutex);
        if (!printing(coder, 0)) 
        {
            pthread_mutex_unlock(&coder->mutex);
            return (false);
        }
    }
    pthread_mutex_unlock(&coder->mutex);
    return (true);
}

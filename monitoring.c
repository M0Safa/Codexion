/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mosafa <mosafa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 21:42:11 by mosafa            #+#    #+#             */
/*   Updated: 2026/03/09 21:42:11 by mosafa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	sch_helper(t_coder *coder)
{
	lock_dongle(coder);
	queue_pop(coder->queue, coder->id);
	pthread_mutex_lock(&(coder->mutex));
	coder->check_time = false;
	pthread_cond_broadcast(&coder->cond);
	pthread_mutex_unlock(&(coder->mutex));
}

void	scheduller(t_coder *coders)
{
	int		f;

		pthread_mutex_lock(coders->queue_lock);
		f = front(*(coders->queue), (coders->par).edf);
		if (f != 0)
			sch_helper(&coders[f - 1]);
		pthread_mutex_unlock(coders->queue_lock);
}

void	*monitoring(void *arg)
{
	t_coder	*coders;
	int		j;
	int		flag;

	coders = (t_coder *) arg;
	flag = 1;
	while (flag)
	{
		j = 0;
		flag = 0;
		while (j < (coders->par).nb_coders)
		{
			if (what_time(&(coders[j])) > (coders->par).burnout)
			{
				printing(&(coders[j]), 5);
				j = 0;
				while (j < (coders->par).nb_coders)
				{
					pthread_mutex_lock(&(coders[j].mutex));
					pthread_cond_broadcast(&(coders[j].cond));
					pthread_mutex_unlock(&(coders[j++].mutex));	
				}
				return (NULL);
			}
			if (get_nb_com(&coders[j++]) != -1)
				flag = 1;
			scheduller(coders);
		}
		ft_sleep(400);
	}
	return (NULL);
}

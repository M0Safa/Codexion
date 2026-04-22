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
				return (NULL);
			}
			if (get_nb_com(&coders[j++]) != -1)
				flag = 1;
		}
		ft_usleep(100);
	}
	return (NULL);
}

void	sch_helper(t_coder *coder)
{
	lock_dongle(coder);
	queue_pop(coder->queue, coder->id);
	pthread_mutex_lock(&(coder->mutex));
	coder->check_time = false;
	pthread_mutex_unlock(&(coder->mutex));
}

bool	is_finished(t_coder *coders)
{
	int		j;

	j = 0;
	while (j < (coders->par).nb_coders)
	{
		if (get_nb_com(&coders[j++]) != -1)
			return (false);
	}
	return (true);
}

void	*scheduller(void *arg)
{
	t_coder	*coders;
	int		f;

	coders = (t_coder *) arg;
	while (!is_finished(coders))
	{
		if (!printing(coders, 0))
			return (NULL);
		pthread_mutex_lock(coders->queue_lock);
		f = front(*(coders->queue), (coders->par).edf);
		if (f != 0)
			sch_helper(&coders[f - 1]);
		pthread_mutex_unlock(coders->queue_lock);
		ft_usleep(100);
	}
	return (NULL);
}

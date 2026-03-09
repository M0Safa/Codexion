/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mosafa <mosafa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 21:39:04 by mosafa            #+#    #+#             */
/*   Updated: 2026/03/09 21:39:04 by mosafa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	debug_refactor(t_coder *coder)
{
	if (*(coder->stop))
		return (false);
	printing(coder, 2);
	usleep((coder->par).debug * 1000);
	if (*(coder->stop))
		return (false);
	printing(coder, 3);
	usleep((coder->par).refactor * 1000);
	coder->nb_compiles ++;
	if (*(coder->stop))
		return (false);
	return (true);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	while ((coder->nb_compiles < (coder->par).nb_of_compiles))
	{
		pthread_mutex_lock(&coder->mutex);
		start_timer(&coder->time);
		coder->check_time = true;
		pthread_mutex_unlock(&coder->mutex);
		if (!request_dongle(coder, coder->left, coder->right))
			break ;
		printing(coder, 1);
		pthread_mutex_lock(&coder->mutex);
		coder->check_time = false;
		pthread_mutex_unlock(&coder->mutex);
		usleep((coder->par).compile * 1000);
		unlock_dongle(coder->left, coder->right);
		if (!debug_refactor(coder))
			break ;
	}
	coder->nb_compiles = -1;
	return (NULL);
}

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

int	get_nb_com(t_coder *coder)
{
	int	i;

	pthread_mutex_lock(&coder->mutex);
	i = coder->nb_compiles;
	pthread_mutex_unlock(&coder->mutex);
	return (i);
}

bool	debug_refactor(t_coder *coder)
{
	if (!printing(coder, 0))
		return (false);
	printing(coder, 2);
	ft_usleep((coder->par).debug * 1000);
	if (!printing(coder, 0))
		return (false);
	printing(coder, 3);
	ft_usleep((coder->par).refactor * 1000);
	pthread_mutex_lock(&coder->mutex);
	coder->nb_compiles ++;
	pthread_mutex_unlock(&coder->mutex);
	if (!printing(coder, 0))
		return (false);
	return (true);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	while ((get_nb_com(coder) < (coder->par).nb_of_compiles))
	{
		pthread_mutex_lock(&coder->mutex);
		start_timer(&coder->time);
		coder->check_time = true;
		pthread_mutex_unlock(&coder->mutex);
		if (!request_dongle(coder))
			break ;
		printing(coder, 1);
		pthread_mutex_lock(&coder->mutex);
		coder->check_time = false;
		pthread_mutex_unlock(&coder->mutex);
		ft_usleep((coder->par).compile * 1000);
		unlock_dongle(coder->left, coder->right);
		if (!debug_refactor(coder))
			break ;
	}
	pthread_mutex_lock(&coder->mutex);
	coder->nb_compiles = -1;
	pthread_mutex_unlock(&coder->mutex);
	return (NULL);
}

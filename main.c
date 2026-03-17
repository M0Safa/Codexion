/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mosafa <mosafa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 21:41:49 by mosafa            #+#    #+#             */
/*   Updated: 2026/03/09 21:41:49 by mosafa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	printing(t_coder *coder, int task)
{
	long	i;
	bool	avail;

	avail = true;
	i = stop_timer(coder->g_time);
	pthread_mutex_lock(coder->print);
	if (*(coder->stop))
		avail = false;
	else if (task == 1 && avail)
		printf("%ld  %d is compiling\n", i, coder->id);
	else if (task == 2 && avail)
		printf("%ld  %d is debugging\n", i, coder->id);
	else if (task == 3 && avail)
		printf("%ld  %d is refactoring\n", i, coder->id);
	else if (task == 4 && avail)
		printf ("%ld  %d has taken a dongle\n", i, coder->id);
	else if (task == 5 && avail)
	{
		printf ("%ld  %d burned out\n", i, coder->id);
		*(coder->stop) = 1;
	}
	pthread_mutex_unlock(coder->print);
	return (avail);
}

void	ft_clean(t_coder *coders, t_dongle *dongles, pthread_t *t, int n)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(coders[0].print);
	pthread_mutex_destroy(coders[0].queue_lock);
	queue_destroy(coders[0].queue);
	free(coders[0].print);
	free(coders[0].queue_lock);
	free(coders[0].queue);
	free(coders[0].stop);
	free(coders[0].g_time);
	while (i < n)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		pthread_mutex_destroy(&coders[i].mutex);
		i++;
	}
	free(coders);
	free(dongles);
	free(t);
}

int	main(int argc, char **argv)
{
	int				i;
	t_parameters	par;
	pthread_t		*threads;
	t_coder			*coders;
	t_dongle		*dongles;

	if (argc != 9)
		return (0 * printf("error: no enough argument.\n"));
	par = validation(argv);
	if (par.nb_coders == -1)
		return (0 * printf("error: you enter an invalid argument.\n"));
	dongles = init_dongles(par);
	coders = init_coders(par, dongles);
	threads = malloc(sizeof(pthread_t) * (par.nb_coders + 1));
	start_timer(coders[0].g_time);
	i = -1;
	while (i++ < par.nb_coders - 1)
		pthread_create(&threads[i], NULL, coder_routine, (void *) &coders[i]);
	pthread_create(&threads[par.nb_coders], NULL, monitoring, (void *) coders);
	i = 0;
	while (i <= par.nb_coders)
		pthread_join(threads[i++], NULL);
	return (ft_clean(coders, dongles, threads, par.nb_coders), 0);
}

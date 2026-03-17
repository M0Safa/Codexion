/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mosafa <mosafa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 21:43:13 by mosafa            #+#    #+#             */
/*   Updated: 2026/03/09 21:43:13 by mosafa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	help_coders(t_coder *coders)
{
	pthread_mutex_t	*print;
	pthread_mutex_t	*queue_lock;
	t_queue			**queue;
	int				*stop;
	int				i;

	print = malloc(sizeof(pthread_mutex_t));
	queue_lock = malloc(sizeof(pthread_mutex_t));
	queue = malloc(sizeof(t_queue *));
	stop = malloc(sizeof(int));
	pthread_mutex_init(print, NULL);
	pthread_mutex_init(queue_lock, NULL);
	*stop = 0;
	i = 0;
	*queue = NULL;
	while (i < coders[0].par.nb_coders)
	{
		coders[i].print = print;
		coders[i].queue_lock = queue_lock;
		coders[i].stop = stop;
		coders[i++].queue = queue;
	}
}

t_coder	*init_coders(t_parameters par, t_dongle *dongles)
{
	t_coder	*coders;
	t_timer	*g_time;
	int		i;

	i = 0;
	g_time = malloc(sizeof(t_timer));
	memset(g_time, 0, sizeof(t_timer));
	coders = malloc(sizeof(t_coder) * par.nb_coders);
	while (i < par.nb_coders)
	{
		coders[i].id = i + 1;
		coders[i].nb_compiles = 0;
		coders[i].check_time = false;
		coders[i].g_time = g_time;
		coders[i].right = &dongles[i];
		pthread_mutex_init(&(coders[i].mutex), NULL);
		if (i == 0)
			coders[i].left = &dongles[par.nb_coders - 1];
		else
			coders[i].left = &dongles[i - 1];
		coders[i++].par = par;
	}
	help_coders(coders);
	return (coders);
}

t_dongle	*init_dongles(t_parameters par)
{
	t_dongle	*dongles;
	int			i;

	dongles = malloc(sizeof(t_dongle) * par.nb_coders);
	i = 0;
	while (i < par.nb_coders)
	{
		pthread_mutex_init(&(dongles[i].mutex), NULL);
		dongles[i].available = true;
		dongles[i].check_time = false;
		i++;
	}
	return (dongles);
}

int	is_int(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (i > 10)
		return (0);
	return (1);
}

t_parameters	validation(char **argv)
{
	t_parameters	par;
	int				i;

	i = 1;
	par.nb_coders = atoi(argv[1]);
	par.burnout = atoi(argv[2]);
	par.compile = atoi(argv[3]);
	par.debug = atoi(argv[4]);
	par.refactor = atoi(argv[5]);
	par.nb_of_compiles = atoi(argv[6]);
	par.cooldown = atoi(argv[7]);
	while (i < 8)
	{
		if (!is_int(argv[i++]))
			par.nb_coders = -1;
	}
	if (strcmp(argv[8], "fifo") == 0)
		par.edf = false;
	else if (strcmp(argv[8], "edf") == 0)
		par.edf = true;
	else
		par.nb_coders = -1;
	return (par);
}

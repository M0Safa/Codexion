/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mosafa <mosafa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 21:39:13 by mosafa           #+#    #+#             */
/*   Updated: 2026/03/09 21:39:13 by mosafa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <errno.h>

struct	s_coder;

typedef struct s_timer
{
	struct timeval	start;
}	t_timer;

typedef struct s_queue
{
	struct s_coder	*coder;
	struct s_queue	*next;
}	t_queue;

typedef struct s_parameters
{
	int		nb_coders;
	int		burnout;
	int		compile;
	int		debug;
	int		refactor;
	int		nb_of_compiles;
	int		cooldown;
	bool	edf;
}	t_parameters;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	bool			available;
	bool			check_time;
	t_timer			time;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				nb_compiles;
	bool			check_time;
	int				*stop;
	t_timer			time;
	t_timer			*g_time;
	t_dongle		*left;
	t_dongle		*right;
	pthread_mutex_t	*print;
	pthread_mutex_t	mutex;
	pthread_mutex_t	*queue_lock;
	t_queue			**queue;
	pthread_cond_t  cond;
	t_parameters	par;
}	t_coder;

void			start_timer(t_timer *t);
long			stop_timer(t_timer *t);
void			*coder_routine(void *arg);
void			*monitoring(void *args);
bool			printing(t_coder *coder, int task);
long			what_time(t_coder *coder);
void			enqueue(t_queue **lst, t_coder *coder);
int				front(t_queue *head, bool edf);
void			queue_pop(t_queue **head, int id);
void			queue_destroy(t_queue **head);
bool			dongle_avail(t_dongle *dl, t_dongle *dr, int cooldown);
void			unlock_dongle(t_dongle *dl, t_dongle *dr);
bool			request_dongle(t_coder *coder);
t_parameters	validation(char **argv);
t_dongle		*init_dongles(t_parameters par);
t_coder			*init_coders(t_parameters par, t_dongle *dongles);
int				get_nb_com(t_coder *coder);
void			lock_dongle(t_coder *c);
bool			ft_usleep(t_coder *coder, long usec);
void			ft_sleep(long usec);

#endif

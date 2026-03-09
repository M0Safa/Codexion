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
			if (!*((coders[j]).stop))
			{
				if (what_time(&(coders[j])) > (coders->par).burnout)
					return (printing(&(coders[j]), 5), NULL);
			}
			if ((coders[j++]).nb_compiles != -1)
				flag = 1;
		}
		usleep(100);
	}
	return (NULL);
}

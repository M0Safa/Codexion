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

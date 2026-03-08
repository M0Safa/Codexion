#include "codexion.h"

t_queue	*queuenew(t_coder *coder)
{
	t_queue	*head;

	head = malloc(sizeof(t_queue));
	head->coder = coder;
	head->next = NULL;
	return (head);
}

void	enqueue(t_queue **lst, t_coder *coder)
{
	t_queue	*last;
	t_queue	*new;

	if (!lst)
		return ;
	new = queuenew(coder);
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next != NULL)
	{
		last = last->next;
	}
	last->next = new;
}

int	front(t_queue *head, bool edf)
{
	int		max;
	int		id;
	int		cooldown;
	t_coder	*coder;

	cooldown = (head->coder->par).cooldown;
	max = 0;
	id = 0;
	while (head)
	{
		coder = head->coder;
		if (dongle_avail(coder->left, coder->right, cooldown))
		{
			if (!edf)
				return (coder->id);
			if (stop_timer(&coder->time) > max)
			{
				max = stop_timer(&coder->time);
				id = coder->id;
			}
		}
		head = head->next;
	}
	return (id);
}

void	queue_pop(t_queue **head, int id)
{
	t_queue	*prev;
	t_queue	*tmp;

	prev = NULL;
	tmp = *head;
	if (tmp != NULL && tmp->coder->id == id)
	{
		*head = tmp->next;
		free(tmp);
		return ;
	}
	while (tmp != NULL && tmp->coder->id != id)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp == NULL) 
		return ;
	prev->next = tmp->next;
	free(tmp);
}

void	queue_destroy(t_queue **head)
{
	t_queue	*tmp;
	t_queue	*next;

	tmp = *head;
	while (tmp)
	{
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
}

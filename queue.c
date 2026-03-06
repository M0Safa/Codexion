#include "codexion.h"

void enqueue(t_queue *q, int id, int duration, int edf)
{
    t_node *new = malloc(sizeof(t_node));

    new->coder_id = id;
    new->duration = duration;
    new->next = NULL;

    if (q->front == NULL)
    {
        q->front = new;
        q->rear = NULL;
        return;
    }

    q->rear = new;
    if (edf && q->rear->duration > q->front->duration)
    {
        new = q->front;
        q->front = q->rear;
        q->rear = new;
    }
}

void dequeue(t_queue *q)
{
    t_node *tmp;

    if (q->front == NULL)
        return;

    tmp = q->front;
    q->front = q->rear;
    q->rear = NULL;
    free(tmp);
}

int front(t_queue *q)
{
    if (q->front == NULL)
        return (-1);

    return (q->front->coder_id);
}

#include "codexion.h"

void enqueue(t_queue *q, int id, int duration, int edf) {
    t_node new;
    int a;
    int b;

    new.coder_id = id;
    new.duration = duration;
    if ((q->front).coder_id == 0)
    {
        q->front = new;
    }
    else 
    {
        q->rear = new;
        a =(q->rear).duration;
        b = (q->front).duration ;
        if (edf && a > b)
        {
            q->rear = q->front;
            q->front = new;
        }
    }
}

void dequeue(t_queue *q)
{
    q->front = q->rear;
    q->rear.coder_id = 0;
}

int front(t_queue *q)
{
    return ((q->front).coder_id);
}

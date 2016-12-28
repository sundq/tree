#include "queue.h"

queue_t *queue_init(int size)
{
  queue_t *q = (queue_t *)allocate_memory(sizeof(queue_t));
  q->buf = allocate_memory(size * sizeof(void *));
  q->size = size;
  return q;
}

int queue_push(queue_t *q, void *data)
{
  q->buf[q->rear] = data;
  q->rear = (q->rear + 1) % q->size;
  q->num++;
  if (q->size == q->num) //resize queue
  {
    printf("resize...\n");
    void *new_buf = allocate_memory(q->size * 2);
    memcpy(new_buf, q->buf, (q->size) * sizeof(void *));
    q->size *= 2;
    release_memory(q->buf);
    q->buf = new_buf;
  }
  return 1;
}

void *queue_pop(queue_t *q)
{
  void *ret = NULL;
  if (q->num > 0)
  {
    ret = q->buf[q->front];
    q->front = (q->front + 1) % q->size;
    q->num--;
  }
  return ret;
}

int queue_destroy(queue_t *q)
{
  release_memory(q->buf);
  release_memory(q);
  return 1;
}

int queue_is_empty(queue_t *q)
{
  return q->num == 0;
}

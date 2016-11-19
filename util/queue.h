/* 
 * File:   queue.h
 * Author: sundq
 *
 * Created on 2016年11月18日, 上午10:31
 */

#ifndef QUEUE_H
#define	QUEUE_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "./memory_pool.h"

typedef struct queue queue_t;

struct queue
{
    void **buf;
    size_t size;
    size_t num;
    size_t front;
    size_t rear;
};

queue_t *queue_init(int size);
int queue_push(queue_t *q, void *data);
void *queue_pop(queue_t *q);
int queue_destroy(queue_t *q);
int queue_is_empty(queue_t *q);


#ifdef	__cplusplus
}
#endif

#endif	/* QUEUE_H */


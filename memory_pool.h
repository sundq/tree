/* 
 * File:   memory_pool.h
 * Author: sundq
 *
 * Created on 2016年11月16日, 下午6:36
 */

#ifndef MEMORY_POOL_H
#define	MEMORY_POOL_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h> 


void *allocate_memory(int size);
int release_memory(void *p);


#ifdef	__cplusplus
}
#endif

#endif	/* MEMORY_POOL_H */


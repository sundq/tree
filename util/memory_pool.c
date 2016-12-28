#include "memory_pool.h"

void *allocate_memory(int size)
{
    return calloc(1, size);
}

int release_memory(void *p)
{
    free(p);
    return 0;
}

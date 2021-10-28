#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "libapportion.h"


void  *apportion(unsigned int size)
{
    //printf("THIS IS THE SIZE: %d\n", size);
    void *p = sbrk(0);
    void *request = sbrk(size);
    if (request == (void*) -1)
    {
        return NULL;
    }
    else
    {
        assert(p == request);
        return p;
    }
}

void release(void *ptr)
{
    brk(ptr);
}
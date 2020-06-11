#ifndef HEAP_MEM_H
#define HEAP_MEM_H

#include <stdlib.h>

/*
 * These functions reimplement the standard calloc, malloc and free functions
 * using the FreeRTOS function calls, whilst also maintaining thread safety.
 * Thus these should be used by any process running on FreeRTOS instead of the built-in
 * calloc, malloc and free, because these standard function calls will fail!
 */

void* rcalloc(size_t num, size_t size);
void* rmalloc(size_t size);
void rfree(void* ptr);

#endif // HEAP_MEM_H

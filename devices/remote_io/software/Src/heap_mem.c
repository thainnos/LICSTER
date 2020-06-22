#include "heap_mem.h"
#include "cmsis_os.h"

extern SemaphoreHandle_t alloc_mutex;

void* rmalloc(size_t size)
{
	void* ptr = NULL;

	if(size > 0)
	{
		while (xSemaphoreTake(alloc_mutex, 10) != pdTRUE);
		ptr = pvPortMalloc(size);
		xSemaphoreGive(alloc_mutex);
	}
	return ptr;
}

void* rcalloc(size_t num, size_t size)
{
	size_t total = num * size;
	void* ptr = NULL;

	if(total > 0)
	{
		while (xSemaphoreTake(alloc_mutex, 10) != pdTRUE);
		ptr = pvPortMalloc(total);
		xSemaphoreGive(alloc_mutex);
		for(size_t i = 0; i < total; i++)
			((uint8_t *) ptr)[i] = 0;
	}
	return ptr;
}

void rfree(void* ptr)
{
	if(ptr)
	{
		while (xSemaphoreTake(alloc_mutex, 10) != pdTRUE);
		vPortFree(ptr);
		xSemaphoreGive(alloc_mutex);
	}
}

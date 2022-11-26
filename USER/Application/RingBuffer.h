#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>

    typedef struct ringbuffer_t
    {
        void *buffer;
        void *buffer_end;
        size_t capacity;
        size_t count;
        size_t size;
        void *head;
        void *tail;
    } ringbuffer_t;

    bool ringBuffer_Init(ringbuffer_t *bfData, size_t capacity, size_t sizeData);
    bool ringBuffer_Free(ringbuffer_t *bfData);
    bool ringBuffer_Push(ringbuffer_t *bfData, const void *item);
    bool ringBuffer_Pop(ringbuffer_t *bfData, void *item);
    bool ringBuffer_IsEmpty(ringbuffer_t *bfData);
    bool ringBuffer_IsFull(ringbuffer_t *bfData);

#ifdef __cplusplus
    extern
}
#endif

#endif // RINGBUFFER_H

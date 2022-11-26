#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "RingBuffer.h"

bool ringBuffer_Init(ringbuffer_t *bfData, size_t capacity, size_t sizeData)
{
    bfData->buffer = malloc(capacity * sizeData);
    if (bfData->buffer == NULL)
    {
        printf("Fail to  init Buffer");
        return false;
    }

    bfData->buffer_end = (char *)bfData->buffer + capacity * sizeData;
    bfData->capacity = capacity;
    bfData->count = 0;
    bfData->head = bfData->buffer;
    bfData->tail = bfData->buffer;
    bfData->size = sizeData;

    return true;
}

bool ringBuffer_Free(ringbuffer_t *bfData)
{
    if (bfData->buffer)
    {
        free(bfData->buffer);
        bfData->buffer = NULL;
        bfData->buffer_end = NULL;
        bfData->capacity = 0;
        bfData->count = 0;
        bfData->size = 0;
        bfData->head = 0;
        bfData->tail = 0;
    }
    else
    {

        return false;
    }

    return true;
}

bool ringBuffer_Push(ringbuffer_t *bfData, const void *item)
{
    if (bfData->count == bfData->capacity)
    {
        return false;
    }

    memcpy(bfData->head, item, bfData->size);
    bfData->head = (char *)(bfData->head) + bfData->size;
    if (bfData->head == bfData->buffer_end)
    {
        bfData->head = bfData->buffer;
    }

    bfData->count++;

    return true;
}

bool ringBuffer_Pop(ringbuffer_t *bfData, void *item)
{
    if (bfData->count == 0)
    {
        return false;
    }

    memcpy(item, bfData->tail, bfData->size);
    bfData->tail = (char *)(bfData->tail) + bfData->size;
    if (bfData->tail == bfData->buffer_end)
    {
        bfData->tail = bfData->buffer;
    }

    bfData->count--;
    return true;
}

bool ringBuffer_IsEmpty(ringbuffer_t *bfData)
{
    if (bfData->count != 0)
    {
        return false;
    }

    return true;
}

bool ringBuffer_IsFull(ringbuffer_t *bfData)
{
    if (bfData->count < bfData->capacity)
    {
        return false;
    }

    return true;
}
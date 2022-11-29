#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "RingBuffer.h"

bool ringBuffer_Init(ringbuffer_t *bfData, uint8_t *Buffer, size_t capacity){
    bfData->buffer = Buffer;
    if (bfData->buffer == NULL){
        printf("Fail to  init Buffer");
        return 1;
    }

    bfData->buffer_end = (char *)bfData->buffer + capacity * sizeof(uint8_t);
    bfData->capacity = capacity;
    bfData->count = 0;
    bfData->head = bfData->buffer;
    bfData->tail = bfData->buffer;
    bfData->size = sizeof(uint8_t);

    return 0;
}

bool ringBuffer_Free(ringbuffer_t *bfData){
    return true;
}

bool ringBuffer_Push(ringbuffer_t *bfData, uint8_t byte){
    if (bfData->count == bfData->capacity){
        return false;
    }

    memcpy(bfData->head, byte, bfData->size);
    bfData->head = (char *)(bfData->head) + bfData->size;
    if (bfData->head == bfData->buffer_end){
        bfData->head = bfData->buffer;
    }

    bfData->count++;

    return true;
}

bool ringBuffer_Pop(ringbuffer_t *bfData, uint8_t *byte){
    if (bfData->count == 0)
    {
        return false;
    }

    memcpy(byte, bfData->tail, bfData->size);
    bfData->tail = (char *)(bfData->tail) + bfData->size;
    if (bfData->tail == bfData->buffer_end)
    {
        bfData->tail = bfData->buffer;
    }

    bfData->count--;
    return true;
}

bool ringBuffer_IsEmpty(ringbuffer_t *bfData){
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



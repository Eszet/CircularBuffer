//
//  CircularBuffer.c
//  testing
//
//  Created by Johannes Lieder on 28.09.12.
//  Copyright (c) 2012 Johannes Lieder. All rights reserved.
//

#include "CircularBuffer.h"

#include <stdlib.h>
#include <assert.h>

struct CircularBufferStruct {
    int capacity;
    int size;
    int *element;
    int inPointer;
    int outPointer;
};

static const struct CircularBufferStruct resetStruct;


static bool isBufferEmpty(const CircularBuffer cb)
{
    return (cb->size == 0);
}

static bool isBufferFull(const CircularBuffer cb)
{
    return (cb->size == cb->capacity);
}

static void setInElement(CircularBuffer cb, const int value)
{
    cb->element[cb->inPointer] = value;
}

static int getOutElement(CircularBuffer cb)
{
    return cb->element[cb->outPointer];
}

static void advanceInPointer(CircularBuffer cb)
{
    cb->inPointer++;
    cb->inPointer %= cb->capacity;
    cb->size++;
}

static void advanceOutPointer(CircularBuffer cb)
{
    cb->outPointer++;
    cb->outPointer %= cb->capacity;
    cb->size--;
}


CircularBuffer CircularBuffer_Create(const int capacity)
{
    CircularBuffer cb = NULL;

    if(capacity < 0)
        return NULL;

    cb = malloc(sizeof(struct CircularBufferStruct));

    if(cb != NULL)
    {
        const size_t elementSize = sizeof(*cb->element);

        *cb = resetStruct;
        cb->element = malloc(elementSize * capacity);
        cb->capacity = capacity;

        if(cb->element == NULL)
        {
            CircularBuffer_Destroy(cb);
            cb = NULL;
        }
    }

    return cb;
}

int CircularBuffer_GetSize(const CircularBuffer cb)
{
    return cb->size;
}

int CircularBuffer_GetCapacity(const CircularBuffer cb)
{
    return cb->capacity;
}

bool CircularBuffer_IsEmpty(const CircularBuffer cb)
{
    return isBufferEmpty(cb);
}

bool CircularBuffer_Push(CircularBuffer cb, const int value)
{
    if(isBufferFull(cb))
        return false;

    setInElement(cb, value);
    advanceInPointer(cb);
    return true;
}

bool CircularBuffer_Pop(CircularBuffer cb, int * const value)
{
    if(value == NULL)
        return false;

    if(isBufferEmpty(cb))
        return false;

    *value = getOutElement(cb);
    advanceOutPointer(cb);
    return true;
}

void CircularBuffer_Destroy(CircularBuffer cb)
{
    if(cb != NULL)
    {
        free(cb->element);
        *cb = resetStruct;
        free(cb);
    }
}

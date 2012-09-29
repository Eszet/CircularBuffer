//
//  CircularBuffer.c
//  testing
//
//  Created by Johannes Lieder on 28.09.12.
//  Copyright (c) 2012 Johannes Lieder. All rights reserved.
//

#include "CircularBuffer.h"

enum { MAX_CAPACITY = 10 };

static struct {
    int capacity;
    int size;
    int element[MAX_CAPACITY];
    int inPointer;
    int outPointer;
} mgmt;

static Bool isBufferEmpty(void)
{
    return (mgmt.size == 0);
}

static Bool isBufferFull(void)
{
    return (mgmt.size == mgmt.capacity);
}

static void setInElement(const int value)
{
    mgmt.element[mgmt.inPointer] = value;
}

static int getOutElement(void)
{
    return mgmt.element[mgmt.outPointer];
}

static void advanceInPointer(void)
{
    mgmt.inPointer++;
    mgmt.inPointer %= mgmt.capacity;
    mgmt.size++;
}

static void advanceOutPointer(void)
{
    mgmt.outPointer++;
    mgmt.outPointer %= mgmt.capacity;
    mgmt.size--;
}


Bool CircularBuffer_Create(int capacity)
{
    if(capacity < 0)
        return FALSE;
    
    /*
     if(capacity > MAX_CAPACITY)
     return FALSE;
     */
    
    mgmt.capacity = capacity;
    mgmt.size = 0;
    mgmt.inPointer = 0;
    mgmt.outPointer = 0;
    return TRUE;
}

int CircularBuffer_GetSize(void)
{
    return mgmt.size;
}

int CircularBuffer_GetCapacity(void)
{
    return mgmt.capacity;
}

Bool CircularBuffer_IsEmpty(void)
{
    return isBufferEmpty();
}

Bool CircularBuffer_Push(const int value)
{
    if(isBufferFull())
        return FALSE;
    
    setInElement(value);
    advanceInPointer();
    return TRUE;
}

Bool CircularBuffer_Pop(int * const value)
{
    if(value == 0)
        return FALSE;
    
    if(isBufferEmpty())
        return FALSE;
    
    *value = getOutElement();
    advanceOutPointer();
    return TRUE;
}

void CircularBuffer_Destroy(void)
{
    
}

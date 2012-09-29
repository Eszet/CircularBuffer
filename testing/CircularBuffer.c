//
//  CircularBuffer.c
//  testing
//
//  Created by Johannes Lieder on 28.09.12.
//  Copyright (c) 2012 Johannes Lieder. All rights reserved.
//

#include "CircularBuffer.h"

static struct {
    int capacity;
    int size;
    int element;
} mgmt = {
    0, /* capacity */
    0, /* size */
    0  /* element */
};

void CircularBuffer_Create(int capacity)
{
    mgmt.capacity = capacity;
    mgmt.size = 0;
}

int CircularBuffer_GetSize(void)
{
    return mgmt.size;
}

int CircularBuffer_GetCapacity(void)
{
    return mgmt.capacity;
}

Bool CircularBuffer_Push(const int value)
{
    if(mgmt.size == mgmt.capacity)
        return FALSE;
    
    mgmt.element = value;
    mgmt.size++;
    return TRUE;
}

Bool CircularBuffer_Pop(int * const value)
{
    if(value == 0)
        return FALSE;
    
    if(mgmt.size == 0)
        return FALSE;
    
    *value = mgmt.element;
    mgmt.size--;
    return TRUE;
}

void CircularBuffer_Destroy(void)
{
    
}

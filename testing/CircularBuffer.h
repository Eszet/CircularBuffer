//
//  CircularBuffer.h
//  testing
//
//  Created by Johannes Lieder on 28.09.12.
//  Copyright (c) 2012 Johannes Lieder. All rights reserved.
//

#ifndef testing_CircularBuffer_h
#define testing_CircularBuffer_h

#include <stdbool.h>

// First-in, first-out buffer handle
typedef struct CircularBufferStruct * CircularBuffer;

// Multiple-instance API
CircularBuffer CircularBuffer_Create(const int capacity);
int CircularBuffer_GetSize(const CircularBuffer cb);
int CircularBuffer_GetCapacity(const CircularBuffer cb);
bool CircularBuffer_IsEmpty(const CircularBuffer cb);
bool CircularBuffer_Push(CircularBuffer cb, const int value);
bool CircularBuffer_Pop(CircularBuffer cb, int * const value);
void CircularBuffer_Destroy(const CircularBuffer const cb);

#endif

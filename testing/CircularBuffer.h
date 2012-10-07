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

// Opaque first-in, first-out buffer
struct CircularBufferStruct;
typedef struct CircularBufferStruct CircularBuffer;

// Multiple-instance API
CircularBuffer * CircularBuffer_Create(const int capacity);
int CircularBuffer_GetSize(const CircularBuffer * const cb);
int CircularBuffer_GetCapacity(const CircularBuffer * const cb);
bool CircularBuffer_IsEmpty(const CircularBuffer * const cb);
bool CircularBuffer_Push(CircularBuffer * const cb, const int value);
bool CircularBuffer_Pop(CircularBuffer * const cb, int * const value);
void CircularBuffer_Destroy(CircularBuffer * const cb);

#endif

//
//  CircularBuffer.h
//  testing
//
//  Created by Johannes Lieder on 28.09.12.
//  Copyright (c) 2012 Johannes Lieder. All rights reserved.
//

#ifndef testing_CircularBuffer_h
#define testing_CircularBuffer_h

typedef enum { FALSE = 0, TRUE = 1 } Bool;

enum { MAX_CAPACITY = 10 };

typedef struct CircularBufferStruct * CircularBuffer;

// Multiple instance API: new-style
CircularBuffer CircularBuffer_Create(int capacity);
int CircularBuffer_GetSize(CircularBuffer cb);
int CircularBuffer_GetCapacity(CircularBuffer cb);
Bool CircularBuffer_IsEmpty(CircularBuffer cb);
Bool CircularBuffer_Push(CircularBuffer cb, const int value);
Bool CircularBuffer_Pop(CircularBuffer cb, int * const value);
void CircularBuffer_Destroy(CircularBuffer cb);

#endif

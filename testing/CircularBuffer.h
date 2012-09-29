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
typedef struct CircularBufferStruct * CircularBuffer;

void CircularBuffer_Create(int capacity);
int CircularBuffer_GetSize(void);
int CircularBuffer_GetCapacity(void);
Bool CircularBuffer_Push(const int value);
Bool CircularBuffer_Pop(int * const value);
void CircularBuffer_Destroy(void);

#endif

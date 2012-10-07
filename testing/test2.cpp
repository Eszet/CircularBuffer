//
//  test2.cpp
//  testing
//
//  Created by Johannes Lieder on 28.09.12.
//  Copyright (c) 2012 Johannes Lieder. All rights reserved.
//

#include <TestHarness.h>

extern "C" {
#include "CircularBuffer.h"
//#include "RuntimeErrorStub.h"
}

TEST_GROUP(CircularBuffer0)
{
    CircularBuffer * cb = 0;
    
    void setup(void)
    {
        cb = CircularBuffer_Create(0);
    }
    
    void tearDown(void)
    {
        CircularBuffer_Destroy(cb);
    }
};

TEST(CircularBuffer0, AlwaysEmpty)
{
    CHECK_EQUAL(0, CircularBuffer_GetSize(cb));
}

TEST(CircularBuffer0, ZeroCapacity)
{
    CHECK_EQUAL(0, CircularBuffer_GetCapacity(cb));
}

TEST(CircularBuffer0, PushAlwaysFails)
{
    CHECK_FALSE(CircularBuffer_Push(cb, 1));
    CHECK_EQUAL(0, CircularBuffer_GetSize(cb));
}

TEST(CircularBuffer0, PopAlwaysFails)
{
    int discard;
    CHECK_FALSE(CircularBuffer_Pop(cb, &discard));
    CHECK_EQUAL(0, CircularBuffer_GetSize(cb));
}


TEST_GROUP(CircularBuffer1)
{
    CircularBuffer * cb = 0;
    int value, discard;

    void setup(void)
    {
        cb = CircularBuffer_Create(1);
    }
    
    void tearDown(void)
    {
        CircularBuffer_Destroy(cb);
    }
};

TEST(CircularBuffer1, NewBufferIsEmptyCapacity)
{
    CHECK_EQUAL(0, CircularBuffer_GetSize(cb));
    CHECK_EQUAL(1, CircularBuffer_GetCapacity(cb));
}

TEST(CircularBuffer1, PushOneElement)
{
    CHECK_TRUE(CircularBuffer_Push(cb, 1));
    CHECK_EQUAL(CircularBuffer_GetSize(cb), 1);
}

TEST(CircularBuffer1, PushTwoElements)
{
    CHECK_TRUE(CircularBuffer_Push(cb, 1));
    CHECK_FALSE(CircularBuffer_Push(cb, 2));
    CHECK_EQUAL(1, CircularBuffer_GetSize(cb));
}

TEST(CircularBuffer1, PopOnEmptyBufferFails)
{
    CHECK_FALSE(CircularBuffer_Pop(cb, &value));
}

TEST(CircularBuffer1, PopOnFullBufferOk)
{
    CHECK_TRUE(CircularBuffer_Push(cb, 0));
    CHECK_TRUE(CircularBuffer_Pop(cb, &value));
}

TEST(CircularBuffer1, PushAndPopElementCorrect)
{
    const int n = 42;
    CHECK_TRUE(CircularBuffer_Push(cb, n));
    CHECK_EQUAL(1, CircularBuffer_GetSize(cb));

    value = -1;
    CHECK_TRUE(CircularBuffer_Pop(cb, &value));
    CHECK_EQUAL(0, CircularBuffer_GetSize(cb));
    CHECK_EQUAL(n, value);
}

TEST(CircularBuffer1, PopTwoElements)
{
    const int n = 17;
    CHECK_TRUE(CircularBuffer_Push(cb, n));
    CHECK_TRUE(CircularBuffer_Pop(cb, &value));

    CHECK_FALSE(CircularBuffer_Pop(cb, &value));
    CHECK_EQUAL(0, CircularBuffer_GetSize(cb));
    CHECK_EQUAL(n, value);
}

TEST(CircularBuffer1, NoOperationOnInvalidRef)
{
    int * const null = 0;
    CHECK_FALSE(CircularBuffer_Pop(cb, null));
    CHECK_EQUAL(0, CircularBuffer_GetSize(cb));

    CHECK_TRUE(CircularBuffer_Push(cb, 0));
    CHECK_FALSE(CircularBuffer_Pop(cb, null));
    CHECK_EQUAL(1, CircularBuffer_GetSize(cb));
}

TEST_GROUP(CircularBufferN)
{
    CircularBuffer * cb = 0;
    const int capacity = 5;

    int value, discard;
    
    enum fifoState { HALF_FULL, FULL, OVER_FULL, HALF_EMPTY, EMPTY, OVER_EMPTY };

    int makeFifo(CircularBuffer * cb, const fifoState state, const int offset = 888)
    {
        int count, targetSize;

        if(state < HALF_EMPTY)
        {
            targetSize = CircularBuffer_GetCapacity(cb);
            
            if(state == HALF_FULL)
                targetSize /= 2;
            
            for(count = 0; CircularBuffer_GetSize(cb) < targetSize; count++)
                CHECK_TRUE(CircularBuffer_Push(cb, count + offset));
            
            if(state == OVER_FULL)
            {
                targetSize = CircularBuffer_GetCapacity(cb) / 2;

                for(count = 0; count < targetSize; count++)
                    CHECK_FALSE(CircularBuffer_Push(cb, count));
                
                return targetSize;
            }
        }
        else if(state > OVER_FULL)
        {
            targetSize = CircularBuffer_GetCapacity(cb) / 2;
            
            if(state == EMPTY || state == OVER_EMPTY)
                targetSize = 0;
            
            for(count = 0; CircularBuffer_GetSize(cb) > targetSize; count++)
            {
                int value = -1;
                CHECK_TRUE(CircularBuffer_Pop(cb, &value));
                CHECK_EQUAL(count + offset, value);
            }
            
            if(state == OVER_EMPTY)
            {
                targetSize = CircularBuffer_GetCapacity(cb) / 2;

                for(count = 0; count < targetSize; count++)
                    CHECK_FALSE(CircularBuffer_Pop(cb, &discard));

                return targetSize;
            }
        }
        
        return count;
    }
    
    void setup(void)
    {
        CHECK_TRUE(capacity > 1);
        cb = CircularBuffer_Create(capacity);
    }
    
    void tearDown(void)
    {
        CircularBuffer_Destroy(cb);
    }
};


TEST(CircularBufferN, ExceedCapacityFail)
{
    CHECK_TRUE(makeFifo(cb, OVER_FULL) > 0);
    CHECK_EQUAL(capacity, CircularBuffer_GetSize(cb));
}

TEST(CircularBufferN, PopOnEmptyBufferDoesNotAffectSize)
{
    CHECK_TRUE(CircularBuffer_IsEmpty(cb));
    CHECK_TRUE(makeFifo(cb, OVER_EMPTY) > 0);

    CHECK_EQUAL(0, CircularBuffer_GetSize(cb));
}

TEST(CircularBufferN, PushAndPopTwoCorrectValues)
{
    CHECK_TRUE(CircularBuffer_Push(cb, 1));
    CHECK_TRUE(CircularBuffer_Push(cb, 2));
    
    value = 17;
    CHECK_TRUE(CircularBuffer_Pop(cb, &value));
    CHECK_EQUAL(1, value);
    CHECK_TRUE(CircularBuffer_Pop(cb, &value));
    CHECK_EQUAL(2, value);
}

TEST(CircularBufferN, FailedPopDoesNotCorruptValue)
{
    const int n = 42;

    discard = 7;
    CHECK_TRUE(CircularBuffer_Push(cb, discard));
    CHECK_TRUE(CircularBuffer_Pop(cb, &discard));
    
    value = n;
    CHECK_FALSE(CircularBuffer_Pop(cb, &value));
    CHECK_EQUAL(n, value);
}

TEST(CircularBufferN, OnlyZeroSizedBufferIsEmpty)
{
    CHECK_TRUE(CircularBuffer_IsEmpty(cb));
    makeFifo(cb, HALF_FULL);
    CHECK_FALSE(CircularBuffer_IsEmpty(cb));
    
    makeFifo(cb, EMPTY);
    CHECK_TRUE(CircularBuffer_IsEmpty(cb));
}

TEST(CircularBufferN, MultipleInstances)
{
    CircularBuffer * another = CircularBuffer_Create(4);

    makeFifo(another, FULL, 222);
    makeFifo(cb, FULL, 111);
    
    makeFifo(cb, EMPTY, 111);
    makeFifo(another, EMPTY, 222);
    
    CircularBuffer_Destroy(another);
}

TEST(CircularBufferN, PipeDataThrough)
{
    int value = 0;
    int added = 0, removed = 0;
    
    while(value < 200)
    {
        enum fifoState addOp = HALF_FULL;
        if(value % 3 == 1) addOp = FULL;
        if(value % 3 == 2) addOp = OVER_FULL;

        added += makeFifo(cb, addOp, value);
        removed += makeFifo(cb, EMPTY, value);
        value += added;
    }
    
    CHECK_TRUE(CircularBuffer_IsEmpty(cb));
}


TEST_GROUP(CircularBufferInvariant)
{
    CircularBuffer * cb = 0;
    
    void tearDown(void)
    {
        CircularBuffer_Destroy(cb);
    }
};

TEST(CircularBufferInvariant, NegativeCapacityFail)
{
    CHECK_FALSE(CircularBuffer_Create(-1));
    CHECK_FALSE(CircularBuffer_Create(-137));
}

TEST(CircularBufferInvariant, PositiveCapacityOk)
{
    CHECK_TRUE(CircularBuffer_Create(100));
}

IGNORE_TEST(CircularBufferInvariant, ApiIsSafeAgainstNull)
{
    // At the moment, behavior of the API for NULL input is undefined.
    // CircularBuffer_Destroy is covered by tearDown
    
    cb = NULL;
    {
        CircularBuffer_GetSize(cb);
        CircularBuffer_GetCapacity(cb);
        CircularBuffer_IsEmpty(cb);
        
        CircularBuffer_Push(cb, 0);
        CircularBuffer_Pop(cb, NULL);
    }
}

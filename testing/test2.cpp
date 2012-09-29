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
    CircularBuffer cb = 0;
    
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
    CircularBuffer cb = 0;
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

TEST(CircularBuffer1, PopOnFilledBufferOk)
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
    CircularBuffer cb = 0;
    const int capacity = 5;

    int value, discard;
    
    int addSomeElements(const int count, const int offset = 111)
    {
        int successful = 0;

        for(int i = 0; i < count; i++)
            if(CircularBuffer_Push(cb, i + offset))
                successful++;
        
        return successful;
    }
    
    void setup(void)
    {
        cb = CircularBuffer_Create(capacity);
    }
    
    void tearDown(void)
    {
        CircularBuffer_Destroy(cb);
    }
};


TEST(CircularBufferN, ExceedCapacityFail)
{
    CHECK_EQUAL(capacity, addSomeElements(capacity));
    CHECK_EQUAL(0, addSomeElements(capacity));

    CHECK_EQUAL(capacity, CircularBuffer_GetSize(cb));
}

TEST(CircularBufferN, PopOnEmptyBufferDoesNotAffectSize)
{
    CHECK_TRUE(CircularBuffer_IsEmpty(cb));

    CHECK_FALSE(CircularBuffer_Pop(cb, &discard));
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
    CHECK_EQUAL(2, addSomeElements(2));

    CHECK_EQUAL(2, CircularBuffer_GetSize(cb));
    CHECK_FALSE(CircularBuffer_IsEmpty(cb));

    CHECK_TRUE(CircularBuffer_Pop(cb, &discard));
    CHECK_EQUAL(1, CircularBuffer_GetSize(cb));
    CHECK_FALSE(CircularBuffer_IsEmpty(cb));

    CHECK_TRUE(CircularBuffer_Pop(cb, &discard));
    CHECK_EQUAL(0, CircularBuffer_GetSize(cb));
    CHECK_TRUE(CircularBuffer_IsEmpty(cb));
}

TEST(CircularBufferN, MultipleInstances)
{
    CircularBuffer another = CircularBuffer_Create(4);
    CircularBuffer_Push(another, 200);
    CircularBuffer_Push(another, 201);

    addSomeElements(20, 100);
    
    int value1 = -1, value2 = -2;
    CircularBuffer_Pop(cb, &value1);
    CHECK_EQUAL(100, value1);

    CircularBuffer_Pop(another, &value1);
    CHECK_EQUAL(200, value1);
    
    CircularBuffer_Pop(cb, &value2);
    CHECK_EQUAL(101, value2);
    
    CircularBuffer_Pop(another, &value2);
    CHECK_EQUAL(201, value2);
    
    CircularBuffer_Destroy(another);
}


TEST_GROUP(CircularBufferInvariant)
{
    CircularBuffer cb = 0;
    
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
    CHECK_TRUE(CircularBuffer_Create(10));
}

IGNORE_TEST(CircularBufferInvariant, ApiIsSafeAgainstNull)
{
    // At this point we have not created a CircularBuffer yet;
    // let's check all calls to the API.

    // At the moment, behavior of the API for NULL input is undefined.
    // CircularBuffer_Destroy is covered by tearDown
    
    CircularBuffer_GetSize(NULL);
    CircularBuffer_GetCapacity(NULL);
    CircularBuffer_IsEmpty(NULL);
    
    CircularBuffer_Push(NULL, 0);
    CircularBuffer_Pop(NULL, NULL);
}


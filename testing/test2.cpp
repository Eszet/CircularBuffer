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
#include "RuntimeError.h"
#include "RuntimeErrorStub.h"
}

TEST_GROUP(CircularBuffer0)
{
    int value;

    void setup(void)
    {
        CircularBuffer_Create(0);
    }
    
    void tearDown(void)
    {
        CircularBuffer_Destroy();
    }
};

TEST(CircularBuffer0, NewBufferIsEmpty)
{
    CHECK_EQUAL(0, CircularBuffer_GetSize());
}

TEST(CircularBuffer0, NewBufferCapacity)
{
    CHECK_EQUAL(0, CircularBuffer_GetCapacity());
}

TEST(CircularBuffer0, CapZeroBufferPushFail)
{
    CHECK_FALSE(CircularBuffer_Push(1));
    CHECK_EQUAL(0, CircularBuffer_GetSize());
}

TEST(CircularBuffer0, CapZeroBufferPopFail)
{
    CHECK_FALSE(CircularBuffer_Pop(&value));
    CHECK_EQUAL(0, CircularBuffer_GetSize());
}

TEST_GROUP(CircularBuffer1)
{
    int value;

    void setup(void)
    {
        CircularBuffer_Create(1);
    }
    
    void tearDown(void)
    {
        CircularBuffer_Destroy();
    }
};

TEST(CircularBuffer1, NewBufferIsEmptyCapacity)
{
    CHECK_EQUAL(CircularBuffer_GetSize(), 0);
    CHECK_EQUAL(CircularBuffer_GetCapacity(), 1);
}

TEST(CircularBuffer1, PushOneElement)
{
    CHECK_TRUE(CircularBuffer_Push(1));
    CHECK_EQUAL(CircularBuffer_GetSize(), 1);
}

TEST(CircularBuffer1, PushTwoElements)
{
    CHECK_TRUE(CircularBuffer_Push(1));
    CHECK_FALSE(CircularBuffer_Push(2));
    CHECK_EQUAL(CircularBuffer_GetSize(), 1);
}

TEST(CircularBuffer1, PopOnEmptyBufferFails)
{
    CHECK_FALSE(CircularBuffer_Pop(&value));
}

TEST(CircularBuffer1, PopOnFilledBufferOk)
{
    CHECK_TRUE(CircularBuffer_Push(0));
    CHECK_TRUE(CircularBuffer_Pop(&value));
}

TEST(CircularBuffer1, PushAndPopElementCorrect)
{
    const int n = 42;
    value = -1;
    CHECK_TRUE(CircularBuffer_Push(n));
    CHECK_EQUAL(1, CircularBuffer_GetSize());

    CHECK_TRUE(CircularBuffer_Pop(&value));
    CHECK_EQUAL(0, CircularBuffer_GetSize());
    CHECK_EQUAL(n, value);
}

TEST(CircularBuffer1, PopTwoElements)
{
    const int n = 17;
    CHECK_TRUE(CircularBuffer_Push(n));
    CHECK_TRUE(CircularBuffer_Pop(&value));

    CHECK_FALSE(CircularBuffer_Pop(&value));
    CHECK_EQUAL(0, CircularBuffer_GetSize());
    CHECK_EQUAL(n, value);
}

TEST(CircularBuffer1, NoOperationOnInvalidRef)
{
    int * const null = 0;
    CHECK_FALSE(CircularBuffer_Pop(null));
    CHECK_EQUAL(0, CircularBuffer_GetSize());

    CHECK_TRUE(CircularBuffer_Push(0));
    CHECK_FALSE(CircularBuffer_Pop(null));
    CHECK_EQUAL(1, CircularBuffer_GetSize());
}

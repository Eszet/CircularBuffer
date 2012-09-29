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
//#include "RuntimeError.h"
//#include "RuntimeErrorStub.h"
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

TEST(CircularBuffer0, AlwaysEmpty)
{
    CHECK_EQUAL(0, CircularBuffer_GetSize());
}

TEST(CircularBuffer0, ZeroCapacity)
{
    CHECK_EQUAL(0, CircularBuffer_GetCapacity());
}

TEST(CircularBuffer0, PushAlwaysFails)
{
    CHECK_FALSE(CircularBuffer_Push(1));
    CHECK_EQUAL(0, CircularBuffer_GetSize());
}

TEST(CircularBuffer0, PopAlwaysFails)
{
    CHECK_FALSE(CircularBuffer_Pop(&value));
    CHECK_EQUAL(0, CircularBuffer_GetSize());
}


TEST_GROUP(CircularBuffer1)
{
    int value, discard;

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

TEST_GROUP(CircularBufferN)
{
    const int capacity = 5;
    int value, discard;
    
    int addSomeElements(const int count, const int offset = 111)
    {
        int successful = 0;

        for(int i = 0; i < count; i++)
            if(CircularBuffer_Push(i+offset))
                successful++;
        
        return successful;
    }
    
    void setup(void)
    {
        // CircularBufferInvariant makes sure positive capacity
        // values return successfully.
        CircularBuffer_Create(capacity);
    }
    
    void tearDown(void)
    {
        CircularBuffer_Destroy();
    }
};


TEST(CircularBufferN, ExceedCapacityFail)
{
    CHECK_EQUAL(capacity, addSomeElements(capacity));
    CHECK_EQUAL(0, addSomeElements(capacity));

    CHECK_EQUAL(capacity, CircularBuffer_GetSize());
}

TEST(CircularBufferN, PushAndPopTwoCorrectValues)
{
    CHECK_TRUE(CircularBuffer_Push(1));
    CHECK_TRUE(CircularBuffer_Push(2));
    
    value = 17;
    CHECK_TRUE(CircularBuffer_Pop(&value));
    CHECK_EQUAL(1, value);
    CHECK_TRUE(CircularBuffer_Pop(&value));
    CHECK_EQUAL(2, value);
}

TEST(CircularBufferN, FailedPopDoesNotCorruptValue)
{
    const int n = 42;
    {
        discard = 7;
        CHECK_TRUE(CircularBuffer_Push(discard));
        CHECK_TRUE(CircularBuffer_Pop(&discard));
    }
    
    value = n;
    CHECK_FALSE(CircularBuffer_Pop(&value));
    CHECK_EQUAL(n, value);
}

TEST(CircularBufferN, OnlyZeroSizedBufferIsEmpty)
{
    CHECK_EQUAL(2, addSomeElements(2));

    CHECK_EQUAL(2, CircularBuffer_GetSize());
    CHECK_FALSE(CircularBuffer_IsEmpty());

    CHECK_TRUE(CircularBuffer_Pop(&discard));
    CHECK_EQUAL(1, CircularBuffer_GetSize());
    CHECK_FALSE(CircularBuffer_IsEmpty());

    CHECK_TRUE(CircularBuffer_Pop(&discard));
    CHECK_EQUAL(0, CircularBuffer_GetSize());
    CHECK_TRUE(CircularBuffer_IsEmpty());
}

TEST_GROUP(CircularBufferInvariant)
{
};

// Remove too many elements, post-cond: size >= 0

TEST(CircularBufferInvariant, NegativeCapacityFail)
{
    CHECK_FALSE(CircularBuffer_Create(-1));
    CHECK_FALSE(CircularBuffer_Create(-137));
}

TEST(CircularBufferInvariant, PositiveCapacityOk)
{
    CHECK_TRUE(CircularBuffer_Create(10));
}

#include "gtest/gtest.h"
#include "test.hpp"

TEST(BasicTest, PassingTest)
{
	TrivialClass t;
	EXPECT_TRUE(t.returnsTrue());
}

TEST(BasicTest, FailingTest)
{
	TrivialClass t;
	EXPECT_TRUE(t.returnsFalse());
}

/* Testing/test_main.cpp */
#include "gtest/gtest.h"
#define __TEST

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}



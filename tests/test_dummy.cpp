#include <gtest/gtest.h>
TEST(SampleTest, BasicAssertion)
{
  EXPECT_EQ(1 + 1, 2);
}

// TEST(SampleTest, BasicFailure)
// {
//   EXPECT_EQ(1 + 1, 1);
// }
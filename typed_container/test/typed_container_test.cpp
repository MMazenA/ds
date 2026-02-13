#include "typed_container.h"
#include <gtest/gtest.h>

// TEST(GroupName, TestName) — each TEST is an independent test case
// EXPECT_EQ(a, b)    — checks a == b, continues on failure
// ASSERT_EQ(a, b)    — checks a == b, STOPS the test on failure

TEST(TypedContainerTest, PutAndGetInt) {
  ds::TypedContainer<int> container;
  container.put(42);
  EXPECT_EQ(container.get<int>(0), 42);
}

TEST(TypedContainerTest, PutAndGetMultipleTypes) {
  ds::TypedContainer<int, std::string> container;
  container.put(10);
  container.put<std::string>("hello");
  EXPECT_EQ(container.get<int>(0), 10);
  EXPECT_EQ(container.get<std::string>(0), "hello");
}

TEST(TypedContainerTest, GetReturnsReference) {
  ds::TypedContainer<int> container;
  container.put(1);
  container.get<int>(0) = 99; // modify through reference
  EXPECT_EQ(container.get<int>(0), 99);
}

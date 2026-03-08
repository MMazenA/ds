#include "typed_container.h"
#include <gtest/gtest.h>

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
  container.get<int>(0) = 99;
  EXPECT_EQ(container.get<int>(0), 99);
}

TEST(TypedContainerTest, PutAndGetWithoutType) {
  ds::TypedContainer<int, std::string, bool> container;
  container.put(42);
  container.put<std::string>("Hi");

  auto v0 = container.get(0);
  std::visit(
      [](auto &&val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) {
          EXPECT_EQ(val, 42);
        } else {
          FAIL() << "Expected int";
        }
      },
      v0);

  auto v1 = container.get(1);
  std::visit(
      [](auto &&val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::string>) {
          EXPECT_EQ(val, "Hi");
        } else {
          FAIL() << "Expected std::string";
        }
      },
      v1);
}

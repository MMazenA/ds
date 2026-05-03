#include "any_map.h"
#include <gtest/gtest.h>

class AnyMapTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(AnyMapTest, InsertAndRetrieveValue) {
  ds::AnyMap<std::string> map;
  map.set("key1", 42);

  std::expected<int, std::string> result = map.get("key1");
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), 42);
}

TEST_F(AnyMapTest, StoreDifferentTypes) {
  ds::AnyMap<std::string> map;
  map.set("int_key", 100);
  map.set("string_key", std::string("hello"));
  map.set("double_key", 3.14);

  std::expected<int, std::string> int_result = map.get("int_key");
  std::expected<std::string, std::string> str_result = map.get("string_key");
  std::expected<double, std::string> dbl_result = map.get("double_key");

  ASSERT_TRUE(int_result.has_value());
  EXPECT_EQ(int_result.value(), 100);

  ASSERT_TRUE(str_result.has_value());
  EXPECT_EQ(str_result.value(), "hello");

  ASSERT_TRUE(dbl_result.has_value());
  EXPECT_DOUBLE_EQ(dbl_result.value(), 3.14);
}

TEST_F(AnyMapTest, AccessNonexistentKeyThrows) {
  ds::AnyMap<std::string> map;

  EXPECT_THROW(auto _ = map.get("nonexistent"), std::out_of_range);
}

TEST_F(AnyMapTest, WrongTypeAccessReturnsUnexpected) {
  ds::AnyMap<std::string> map;
  map.set("key1", 42);

  std::expected<std::string, std::string> result = map.get("key1");
  EXPECT_FALSE(result.has_value());
}

TEST_F(AnyMapTest, ContainsKey) {
  ds::AnyMap<std::string> map;
  map.set("exists", 123);

  EXPECT_TRUE(map.contains("exists"));
  EXPECT_FALSE(map.contains("does_not_exist"));
}

TEST_F(AnyMapTest, IntStoredAccessedAsDouble) {
  ds::AnyMap<std::string> map;
  map.set("val", 42); // stored as int

  std::expected<double, std::string> result = map.get("val");
  EXPECT_FALSE(result.has_value());
}

TEST_F(AnyMapTest, DoubleStoredAccessedAsInt) {
  ds::AnyMap<std::string> map;
  map.set("val", 3.14);

  std::expected<int, std::string> result = map.get("val");
  EXPECT_FALSE(result.has_value());
}

TEST_F(AnyMapTest, StringLiteralStoredAccessedAsStdString) {
  ds::AnyMap<std::string> map;
  map.set("val", "hello");

  std::expected<std::string, std::string> result = map.get("val");
  EXPECT_FALSE(result.has_value());
}

TEST_F(AnyMapTest, AutoRequiresExplicitExpectedType) {
  ds::AnyMap<std::string> map;
  map.set("val", 99);

  auto result = std::expected<int, std::string>(std::move(map.get("val")));
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), 99);

  auto bad = std::expected<double, std::string>(std::move(map.get("val")));
  EXPECT_FALSE(bad.has_value());
}

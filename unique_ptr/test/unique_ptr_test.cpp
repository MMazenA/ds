#include <gtest/gtest.h>

#include "unique_ptr.h"

// ---------------------------------------------------------------------------
// Category A - Member functions
// ---------------------------------------------------------------------------

TEST(UniquePtr, CreateAndAccess) {

  DS::UniquePtr<int> p{new int(10)};

  EXPECT_NE(p.get(), nullptr);
  EXPECT_EQ(*p, 10);
}

TEST(UniquePtr, Reset) {

  DS::UniquePtr<int> p{new int(10)};
  p.reset();

  EXPECT_EQ(p.get(), nullptr);
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST(UniquePtr, Release) {

  DS::UniquePtr<int> p{new int(10)};
  int *raw = p.release();

  EXPECT_NE(raw, nullptr);
  EXPECT_EQ(*raw, 10);
  EXPECT_EQ(p.get(), nullptr);

  delete raw;
}

TEST(UniquePtr, Swap) {

  DS::UniquePtr<int> a{new int(1)};
  DS::UniquePtr<int> b{new int(2)};
  int *a_raw = a.get();
  int *b_raw = b.get();

  a.swap(b);

  EXPECT_EQ(a.get(), b_raw);
  EXPECT_EQ(b.get(), a_raw);
  EXPECT_EQ(*a, 2);
  EXPECT_EQ(*b, 1);
}

TEST(UniquePtr, Get) {
  DS::UniquePtr<int> p{new int(42)};

  EXPECT_NE(p.get(), nullptr);
  EXPECT_EQ(*p.get(), 42);

  DS::UniquePtr<int> empty;
  EXPECT_EQ(empty.get(), nullptr);
}

TEST(UniquePtr, OperatorBool) {

  DS::UniquePtr<int> p{new int(1)};
  EXPECT_TRUE(static_cast<bool>(p));

  DS::UniquePtr<int> empty;
  EXPECT_FALSE(static_cast<bool>(empty));
}

TEST(UniquePtr, OperatorDereference) {

  DS::UniquePtr<int> p{new int(7)};
  EXPECT_EQ(*p, 7);

  *p = 9;
  EXPECT_EQ(*p, 9);
}

TEST(UniquePtr, OperatorArrow) {

  struct Point {
    int x;
    int y;
  };
  DS::UniquePtr<Point> p{new Point{3, 4}};

  EXPECT_EQ(p->x, 3);
  EXPECT_EQ(p->y, 4);
}

TEST(UniquePtr, OperatorEquals) {

  DS::UniquePtr<int> a{new int(1)};
  DS::UniquePtr<int> b;

  EXPECT_TRUE(a == a);
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(b == b);
}

TEST(UniquePtr, OperatorEqualsDistinctNonNull) {

  DS::UniquePtr<int> a{new int(1)};
  DS::UniquePtr<int> b{new int(2)};

  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
}

TEST(UniquePtr, OperatorNotEquals) {

  DS::UniquePtr<int> a{new int(1)};
  DS::UniquePtr<int> b;

  EXPECT_FALSE(a != a);
  EXPECT_TRUE(a != b);
  EXPECT_FALSE(b != b);
}

// ---------------------------------------------------------------------------
// Category C - Non-member functions
// ---------------------------------------------------------------------------

TEST(UniquePtr, MakeUniqueBasic) {

  auto p = DS::make_unique<int>(10);

  EXPECT_NE(p.get(), nullptr);
  EXPECT_EQ(*p, 10);
}

TEST(UniquePtr, MakeUniqueForwardsArgs) {

  struct Point {
    int x;
    int y;
    Point(int x_pos, int y_pos) : x(x_pos), y(y_pos) {}
  };

  auto p = DS::make_unique<Point>(3, 4);

  EXPECT_NE(p.get(), nullptr);
  EXPECT_EQ(p->x, 3);
  EXPECT_EQ(p->y, 4);
}

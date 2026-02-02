#include "Vector.h"
#include <gtest/gtest.h>
#include <string>
#include <utility>

class VectorTest : public ::testing::Test {
protected:
  ds::Vector<int> vec;
};

TEST_F(VectorTest, DefaultConstructorCreatesEmptyVector) {
  EXPECT_EQ(vec.getSize(), 0);
  EXPECT_EQ(vec.getCapacity(), 0);
}

TEST_F(VectorTest, CopyConstructorCopiesElements) {
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);

  ds::Vector<int> copy(vec);

  EXPECT_EQ(copy.getSize(), 3);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
  EXPECT_EQ(copy[2], 3);
}

TEST_F(VectorTest, CopyConstructorIsIndependent) {
  vec.push_back(1);
  ds::Vector<int> copy(vec);

  vec.push_back(2);

  EXPECT_EQ(vec.getSize(), 2);
  EXPECT_EQ(copy.getSize(), 1);
}

TEST_F(VectorTest, MoveConstructorTransfersOwnership) {
  vec.push_back(1);
  vec.push_back(2);

  ds::Vector<int> moved(std::move(vec));

  EXPECT_EQ(moved.getSize(), 2);
  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);
  EXPECT_EQ(vec.getSize(), 0);
}

TEST_F(VectorTest, CopyAssignmentCopiesElements) {
  vec.push_back(1);
  vec.push_back(2);

  ds::Vector<int> other;
  other = vec;

  EXPECT_EQ(other.getSize(), 2);
  EXPECT_EQ(other[0], 1);
  EXPECT_EQ(other[1], 2);
}

TEST_F(VectorTest, MoveAssignmentTransfersOwnership) {
  vec.push_back(1);
  vec.push_back(2);

  ds::Vector<int> other;
  other = std::move(vec);

  EXPECT_EQ(other.getSize(), 2);
  EXPECT_EQ(other[0], 1);
  EXPECT_EQ(vec.getSize(), 0);
}

// ============== Push Back Tests ==============

TEST_F(VectorTest, PushBackLvalueAddsElement) {
  int val = 42;
  vec.push_back(val);

  EXPECT_EQ(vec.getSize(), 1);
  EXPECT_EQ(vec[0], 42);
}

TEST_F(VectorTest, PushBackRvalueAddsElement) {
  vec.push_back(42);

  EXPECT_EQ(vec.getSize(), 1);
  EXPECT_EQ(vec[0], 42);
}

TEST_F(VectorTest, PushBackMultipleElements) {
  for (int i = 0; i < 10; ++i) {
    vec.push_back(i);
  }

  EXPECT_EQ(vec.getSize(), 10);
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(vec[i], i);
  }
}

TEST_F(VectorTest, PushBackTriggersResize) {
  vec.push_back(1);
  EXPECT_EQ(vec.getCapacity(), 1);

  vec.push_back(2);
  EXPECT_EQ(vec.getCapacity(), 2);

  vec.push_back(3);
  EXPECT_EQ(vec.getCapacity(), 4);

  vec.push_back(4);
  vec.push_back(5);
  EXPECT_EQ(vec.getCapacity(), 8);
}

// ============== Element Access Tests ==============

TEST_F(VectorTest, OperatorBracketAccessesElements) {
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);

  EXPECT_EQ(vec[0], 10);
  EXPECT_EQ(vec[1], 20);
  EXPECT_EQ(vec[2], 30);
}

TEST_F(VectorTest, OperatorParenAccessesElements) {
  vec.push_back(10);
  vec.push_back(20);

  EXPECT_EQ(vec(0), 10);
  EXPECT_EQ(vec(1), 20);
}

TEST_F(VectorTest, NegativeIndexAccessesFromEnd) {
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);

  EXPECT_EQ(vec(-1), 30);
  EXPECT_EQ(vec(-2), 20);
  EXPECT_EQ(vec(-3), 10);
}

TEST_F(VectorTest, OperatorBracketAllowsModification) {
  vec.push_back(10);
  vec[0] = 99;

  EXPECT_EQ(vec[0], 99);
}

TEST_F(VectorTest, OutOfRangeThrowsException) {
  vec.push_back(1);

  EXPECT_THROW(vec[5], std::out_of_range);
  EXPECT_THROW(vec(10), std::out_of_range);
}

// ============== Iterator Tests ==============

TEST_F(VectorTest, BeginEndIteratorTraversal) {
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);

  int sum = 0;
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    sum += *it;
  }

  EXPECT_EQ(sum, 6);
}

TEST_F(VectorTest, RangeBasedForLoop) {
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);

  int sum = 0;
  for (const auto &val : vec) {
    sum += val;
  }

  EXPECT_EQ(sum, 6);
}

TEST_F(VectorTest, IteratorPreIncrement) {
  vec.push_back(1);
  vec.push_back(2);

  auto it = vec.begin();
  ++it;

  EXPECT_EQ(*it, 2);
}

TEST_F(VectorTest, IteratorPostIncrement) {
  vec.push_back(1);
  vec.push_back(2);

  auto it = vec.begin();
  auto old = it++;

  EXPECT_EQ(*old, 1);
  EXPECT_EQ(*it, 2);
}

TEST_F(VectorTest, IteratorDecrement) {
  vec.push_back(1);
  vec.push_back(2);

  auto it = vec.end();
  --it;

  EXPECT_EQ(*it, 2);
}

TEST_F(VectorTest, IteratorAddAssign) {
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);

  auto it = vec.begin();
  it += 2;

  EXPECT_EQ(*it, 3);
}

TEST_F(VectorTest, IteratorSubtractAssign) {
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);

  auto it = vec.end();
  it -= 1;

  EXPECT_EQ(*it, 3);
}

// ============== Move Semantics Tests ==============

class MoveTracker {
public:
  static int copy_count;
  static int move_count;

  static void reset() {
    copy_count = 0;
    move_count = 0;
  }

  MoveTracker() = default;
  MoveTracker(const MoveTracker &) { ++copy_count; }
  MoveTracker(MoveTracker &&) noexcept { ++move_count; }
  MoveTracker &operator=(const MoveTracker &) {
    ++copy_count;
    return *this;
  }
  MoveTracker &operator=(MoveTracker &&) noexcept {
    ++move_count;
    return *this;
  }
};

int MoveTracker::copy_count = 0;
int MoveTracker::move_count = 0;

TEST(VectorMoveTest, PushBackRvalueUsesMoveConstruction) {
  MoveTracker::reset();
  ds::Vector<MoveTracker> vec;

  vec.push_back(MoveTracker{});

  EXPECT_EQ(MoveTracker::move_count, 1);
  EXPECT_EQ(MoveTracker::copy_count, 0);
}

TEST(VectorMoveTest, PushBackLvalueUsesCopyConstruction) {
  MoveTracker::reset();
  ds::Vector<MoveTracker> vec;

  MoveTracker tracker;
  vec.push_back(tracker);

  EXPECT_EQ(MoveTracker::copy_count, 1);
}

TEST(VectorMoveTest, ResizeUsesMoveForExistingElements) {
  MoveTracker::reset();
  ds::Vector<MoveTracker> vec;

  vec.push_back(MoveTracker{});
  int moves_after_first = MoveTracker::move_count;

  vec.push_back(MoveTracker{});

  // Should have moved the first element during resize
  EXPECT_GT(MoveTracker::move_count, moves_after_first + 1);
}

// ============== String Type Tests ==============

TEST(VectorStringTest, WorksWithStrings) {
  ds::Vector<std::string> vec;

  vec.push_back("hello");
  vec.push_back("world");

  EXPECT_EQ(vec.getSize(), 2);
  EXPECT_EQ(vec[0], "hello");
  EXPECT_EQ(vec[1], "world");
}

TEST(VectorStringTest, StringMoveSemantics) {
  ds::Vector<std::string> vec;

  std::string str = "test string";
  vec.push_back(std::move(str));

  EXPECT_EQ(vec[0], "test string");
}

// ============== Edge Cases ==============

TEST_F(VectorTest, EmptyVectorIteration) {
  int count = 0;
  for ([[maybe_unused]] auto &val : vec) {
    ++count;
  }

  EXPECT_EQ(count, 0);
}

TEST_F(VectorTest, SingleElementVector) {
  vec.push_back(42);

  EXPECT_EQ(vec.getSize(), 1);
  EXPECT_EQ(vec[0], 42);
  EXPECT_EQ(vec(-1), 42);
}

TEST_F(VectorTest, LargeNumberOfElements) {
  const int count = 10000;

  for (int i = 0; i < count; ++i) {
    vec.push_back(i);
  }

  EXPECT_EQ(vec.getSize(), count);
  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[count - 1], count - 1);
  EXPECT_EQ(vec(-1), count - 1);
}

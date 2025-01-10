#include "Vector.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <print>
#include <string>
#include <thread>

struct LargeStruct {
  LargeStruct(std::size_t s = 65536) : size(s), data(new double[s]) {
    for (std::size_t i = 0; i < size; ++i) {
      data[i] = static_cast<double>(i);
    }
  }

  LargeStruct(const LargeStruct &other)
      : size(other.size), data(new double[other.size]) {
    std::copy(other.data, other.data + other.size, data);
  }

  LargeStruct(LargeStruct &&other) noexcept
      : size(other.size), data(other.data) {
    other.size = 0;
    other.data = nullptr;
  }

  LargeStruct &operator=(const LargeStruct &other) {
    if (this != &other) {
      delete[] data;

      size = other.size;
      data = new double[size];
      std::copy(other.data, other.data + other.size, data);
    }
    return *this;
  }

  LargeStruct &operator=(LargeStruct &&other) noexcept {
    if (this != &other) {
      delete[] data;

      size = other.size;
      data = other.data;

      other.size = 0;
      other.data = nullptr;
    }
    return *this;
  }

  ~LargeStruct() { delete[] data; }

  std::size_t size;
  double *data;
};

void test_move(const int &iterations, bool move) {
  ds::vector<LargeStruct> vec;
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    LargeStruct temp;
    vec.push_back(move ? std::move(temp) : temp);
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = duration_cast<std::chrono::milliseconds>(end - start).count();
  std::println("Time {1}: {0} ms", duration,
               move ? "with move" : "without move");
}

int main() {
  const int iterations = 1000;
  test_move(iterations, false);
  test_move(iterations, true);
  ds::vector<int> vec;
  vec.push_back(5);
  vec.push_back(15);
  vec.push_back(20);
  vec.push_back(25);

  std::print("[");
  for (auto x : vec) {
    std::print("{0} ", x);
  }
  std::println("]");

  std::println("");
  for (auto x : vec.slice(ds::start, ds::end)) {
    std::println("{0}", x);
  }

  vec.push_back(3);

  std::println("");
  std::println("");
  std::println("reverse indexing: {0}", vec(-1));

  // for (auto x : vec.what()) {

  //   std::println("What? : {}", x);
  // }

  std::println("Passed");
  return 0;
}
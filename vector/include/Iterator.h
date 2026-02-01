#ifndef ITERATOR_H
#define ITERATOR_H

#include <Vector.h>
#include <algorithm>
#include <cstddef>
#include <format>
#include <memory>
#include <print>
#include <ranges>
#include <stdexcept>
#include <utility>

namespace ds {
template <class T> class Iterator {

public:
  using pointer_diff = std::ptrdiff_t;
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using const_reference = const T &;
  using self = Iterator;
  using optional_int = std::optional<int64_t>;

public:
  Iterator() = default;
  Iterator(pointer ptr_start)
      : m_ptr_current(ptr_start), m_ptr_start(ptr_start) {};

  Iterator(pointer ptr_start, int64_t step)
      : m_ptr_current(ptr_start), m_ptr_start(ptr_start), m_step(step) {};

  Iterator(pointer ptr_start, pointer ptr_end, optional_int begin,
           optional_int end, optional_int step)
      : m_ptr_current(ptr_start), m_ptr_start(ptr_start), m_ptr_end(ptr_end) {

    /**
     *
     * @todo: Need to run checks here that begin end and step make sense in
     * context to each other
     */

    m_begin = begin.has_value() ? begin.value() : 0;
    m_end = end.has_value() ? end.value() : -1;
    m_step = step.has_value() ? step.value() : 1;
  }
  Iterator(const Iterator &other) { m_ptr_current = other.m_ptr_current; }
  Iterator(Iterator &&other) {
    m_ptr_current = other.m_ptr_current;
    other.m_ptr_current = nullptr;
  }
  Iterator &operator=(const self &other) {
    m_ptr_current = other.m_ptr_current;
    return *this;
  }
  Iterator operator=(self &&other) {
    m_ptr_current = other.m_ptr_current;
    other.m_ptr_current = nullptr;
    return *this;
  }
  ~Iterator() = default;

  reference operator*() const { return *m_ptr_current; };
  pointer operator->() { return m_ptr_current; };

  Iterator &operator++() {
    m_ptr_current += m_step;
    return *this;
  }

  Iterator operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  Iterator &operator--() {
    m_ptr_current -= m_step;
    return *this;
  }

  Iterator operator--(int) {
    Iterator tmp = *this;
    --(*this);
    return tmp;
  }

  friend bool operator==(const Iterator &a, const Iterator &b) {
    return a.m_ptr_current == b.m_ptr_current;
  }
  friend bool operator!=(const Iterator &a, const Iterator &b) {
    return a.m_ptr_current != b.m_ptr_current;
  }

private:
  pointer m_ptr_current;
  pointer m_ptr_start;
  pointer m_ptr_end;
  int64_t m_begin;
  int64_t m_end;
  int64_t m_step = 1;
};

} // namespace ds

#endif
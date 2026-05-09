#ifndef VECTOR_ITERATOR_H
#define VECTOR_ITERATOR_H

#include <cstddef>
#include <iterator>
#include <optional>

namespace ds {

template <typename T> class VectorIterator {
public:
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;
  using optional_int = std::optional<int64_t>;

  VectorIterator() = default;
  VectorIterator(pointer ptr, optional_int iter_start = std::nullopt,
                 optional_int iter_end = std::nullopt,
                 optional_int step = std::nullopt)
      : m_ptr(ptr), m_start(iter_start), m_end(iter_end), m_step(step) {}

  VectorIterator(const VectorIterator &other) = default;
  VectorIterator &operator=(const VectorIterator &other) = default;
  VectorIterator(VectorIterator &&other) noexcept = default;
  VectorIterator &operator=(VectorIterator &&other) noexcept = default;
  ~VectorIterator() = default;

  reference operator*() const { return *m_ptr; }
  pointer operator->() { return m_ptr; }

  VectorIterator &operator++() {
    m_ptr += m_step.has_value() ? m_step.value() : 1;
    return *this;
  }

  VectorIterator operator++(int) {
    VectorIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  VectorIterator &operator--() {
    m_ptr--;
    return *this;
  }

  VectorIterator operator--(int) {
    VectorIterator tmp = *this;
    --(*this);
    return tmp;
  }

  VectorIterator &operator+=(int x) {
    m_ptr += x;
    return *this;
  }

  VectorIterator &operator-=(int x) {
    m_ptr -= x;
    return *this;
  }

  VectorIterator &operator+(int x) {
    m_ptr += x;
    return *this;
  }

  VectorIterator &operator-(int x) {
    m_ptr -= x;
    return *this;
  }

  friend bool operator==(const VectorIterator &a, const VectorIterator &b) {
    return a.m_ptr == b.m_ptr;
  }
  friend bool operator!=(const VectorIterator &a, const VectorIterator &b) {
    return a.m_ptr != b.m_ptr;
  }

private:
  pointer m_ptr = nullptr;
  optional_int m_start;
  optional_int m_end;
  optional_int m_step;
};

} // namespace ds

#endif

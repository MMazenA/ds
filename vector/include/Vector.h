#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cstddef>
#include <format>
#include <memory>
#include <print>
#include <ranges>
#include <stdexcept>
#include <utility>

namespace ds {
constexpr inline std::optional<int64_t> start = std::nullopt;
constexpr inline std::optional<int64_t> end = std::nullopt;

template <typename T> class Vector {

  /**
   * Vector Notes
   *
   * @warning ONLY WORKS WITH TRIVIALLY CONSTRUCTABLE OBJECTS
   *
   * @todo also need a constructor that will take a size and construct default
   * <T>'s vector<T>(10) should allocate + construct in one go
   *
   * @todo need emplace back, where user can pass params to me, and ill
   * construct the object into its location, this should call another function
   * maybe m_construct_at(index) or something
   *
   * @todo also need an iterator for this whole thing
   *
   * @todo slice 'operator'
   * https://stackoverflow.com/a/68568397/20313250
   *
   * @todo maybe make threadsafe?
   *
   */

public:
  using self = Vector;
  using value_type = T;
  using pointer = value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using optional_int = std::optional<int64_t>;

  template <std::bidirectional_iterator iter> class Iterator {
    // concept provides compile-time constraints, dont have to actually use iter
  public:
    using difference_type = std::ptrdiff_t;

    Iterator() = default;
    Iterator(pointer ptr, optional_int iter_start = std::nullopt,
             optional_int end = std::nullopt, optional_int step = std::nullopt)
        : m_ptr(ptr), m_start(iter_start), m_end(end), m_step(step) {};

    Iterator(const Iterator &other) = default;
    Iterator &operator=(const Iterator &other) = default;
    Iterator(Iterator &&other) noexcept = default;
    Iterator &operator=(Iterator &&other) noexcept = default;

    reference operator*() const { return *m_ptr; };
    pointer operator->() { return m_ptr; };

    Iterator &operator++() {
      m_ptr += m_step.has_value() ? m_step.value() : 1;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    Iterator &operator--() {
      m_ptr--;
      return *this;
    }

    Iterator operator--(int) {
      Iterator tmp = *this;
      --(*this);
      return tmp;
    }

    Iterator &operator-=(int x) {
      m_ptr -= x;
      return *this;
    }

    Iterator &operator+=(int x) {
      m_ptr += x;
      return *this;
    }

    Iterator &operator+(int x) {
      m_ptr += x;
      return *this;
    }

    Iterator &operator-(int x) {
      m_ptr -= x;
      return *this;
    }

    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.m_ptr == b.m_ptr;
    }
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.m_ptr != b.m_ptr;
    }

  private:
    pointer m_ptr;
    optional_int m_start;
    optional_int m_end;
    optional_int m_step;
  };

public:
  class SliceWrapper {
  public:
    explicit SliceWrapper(Vector &parent, optional_int start_idx,
                          optional_int end_idx, optional_int step_val)
        : m_parent(parent) {
      m_start = start_idx.has_value() ? start_idx.value() : 0;
      m_end = end_idx.has_value() ? end_idx.value()
                                  : static_cast<int64_t>(m_parent.m_size);
      m_step = step_val.has_value() ? step_val.value() : 1;
    };

    Iterator<pointer> begin() {
      return Iterator<pointer>(&m_parent.m_raw[m_start], std::nullopt,
                               std::nullopt, m_step);
    };
    Iterator<pointer> end() {
      return Iterator<pointer>(&m_parent.m_raw[m_end]);
    };

  private:
    Vector<value_type> &m_parent;
    int64_t m_start;
    int64_t m_end;
    int64_t m_step;
  };

  SliceWrapper slice(optional_int start_idx = std::nullopt,
                     optional_int end_idx = std::nullopt,
                     optional_int step_val = optional_int(1)) {
    return SliceWrapper(*this, start_idx, end_idx, step_val);
  }

  Vector() = default;
  Vector(const self &other);
  Vector &operator=(const self &other);
  Vector(self &&other) noexcept;
  Vector operator=(self &&other) noexcept;
  ~Vector();

  void push_back(const T &value);
  void push_back(T &&value);

  value_type pop();
  reference peek() const;
  reference operator[](int32_t i);
  reference operator()(int32_t i);
  reference operator()(int32_t i, int32_t j);
  reference operator()(int32_t i, int32_t j, int32_t k);

  Iterator<pointer> begin();
  Iterator<pointer> end();
  Iterator<pointer> rbegin();
  Iterator<pointer> rend();

  size_type getSize() const { return m_size; }
  size_type getCapacity() const { return m_capacity; }

private:
  pointer m_raw = nullptr;
  std::allocator<value_type> m_allocator;
  size_type m_size = 0;
  size_type m_capacity = 0;

  void m_resize(size_t new_capacity);
};

template <typename T> Vector<T>::Vector(const Vector &other) {
  m_capacity = other.m_size;
  m_resize(m_capacity);
  std::uninitialized_copy(other.m_raw, other.m_raw + other.m_size, m_raw);
  m_size = other.m_size;
}

template <typename T> Vector<T> &Vector<T>::operator=(const Vector &other) {
  if (&other != this) {
    m_capacity = other.m_size;
    m_resize(m_capacity);
    std::uninitialized_copy(other.m_raw, other.m_raw + other.m_size, m_raw);
    m_size = other.m_size;
  }
  return *this;
}

template <typename T> Vector<T>::Vector(Vector &&other) noexcept {
  this->m_capacity = other.m_capacity;
  this->m_size = other.m_size;
  this->m_raw = other.m_raw;

  other.m_capacity = 0;
  other.m_size = 0;
  other.m_raw = 0;
}

template <typename T> Vector<T> Vector<T>::operator=(Vector &&other) noexcept {
  this->m_capacity = other.m_capacity;
  this->m_size = other.m_size;
  this->m_raw = other.m_raw;

  other.m_capacity = 0;
  other.m_size = 0;
  other.m_raw = 0;

  return *this;
}

template <typename T> Vector<T>::~Vector() {
  std::destroy_n(m_raw, m_size);
  if (m_raw) {
    m_allocator.deallocate(m_raw, m_capacity);
  }
}

template <typename T> void Vector<T>::m_resize(size_t new_capacity) {
  /**
   * 1. Allocate double memory, but dont construct any objects in the newly
   * duplicated space
   *
   * 2. We steal the resources with move, we dont need to copy them, but
   * because we're dealing with moving objects into raw uninitialized memory
   * we need to use uninitialized_move. By the time this function is called,
   * my  is essentially just a regular array filled with [capacity * <T>] this
   * array now needs to move into the raw memory I just allocated in step 1
   *
   *
   * 3. Move has now transferred underlying resources, still have a lump of
   * memory that contains has stripped down skeleton objects, so need to
   * destroy them
   *
   * 4. Now have a lump of raw memory, so de-allocate that so it can be
   * free'd.
   *
   * 5. Point member data to previously allocated object
   *
   * 6. Update vector meta-data and exit, which destroys previous pointer on
   * the stack
   *
   *
   * @todo I THINK, allocator is returning a void * that has been casted to
   * <T>* to ensure type safety on constructions I'm not sure though ??
   *
   * @todo maybe there are some alignment optimizations I can make here ?
   * https://en.cppreference.com/w/cpp/memory/allocator_traits/allocate_at_least
   * https://en.cppreference.com/w/cpp/memory/c/aligned_alloc
   * https://stackoverflow.com/a/40909221/20313250
   *
   * I think that depending on the alignment of my <T>, I might end up with
   * more space than I know, which may be enough space to handle another <T>?
   */
  T *new_raw = m_allocator.allocate(new_capacity);
  std::uninitialized_move_n(m_raw, m_size, new_raw);
  std::destroy_n(m_raw, m_size);
  m_allocator.deallocate(m_raw, m_capacity);

  m_raw = new_raw;
  m_capacity = new_capacity;
}

template <typename T> void Vector<T>::push_back(const T &value) {
  /**
   * https://en.cppreference.com/w/cpp/memory/construct_at
   * construct_at needs trivial object
   */

  if (m_size == m_capacity) {
    m_resize(m_capacity == 0 ? 1 : m_capacity * 2);
  }

  std::construct_at(&m_raw[m_size], std::forward<const T>(value));
  ++m_size;
}

/**
 * We don't want to use std::move to pass args through, we can just forward
 * them https://stackoverflow.com/a/9716708/20313250 Conceptually, using move,
 * invalidates the original object, that isn't really needed I dont 'need' to
 * steal the resources here, but the functionality is here incase someone
 * wants to avoid copying T
 *
 * It makes more sense in a constructer to
 *
 */
template <typename T> void Vector<T>::push_back(T &&value) {
  if (m_size == m_capacity) {
    m_resize(m_capacity == 0 ? 1 : m_capacity * 2);
  }

  std::construct_at(&m_raw[m_size], std::move(value));
  ++m_size;
}

template <typename T> T &Vector<T>::operator()(int32_t i) {
  return operator()(i, 0, 0);
}

template <typename T> T &Vector<T>::operator[](int32_t i) {
  return operator()(i, 0, 0);
}
template <typename T> T &Vector<T>::operator()(int32_t i, int32_t j) {
  return operator()(i, j, 0);
}
template <typename T>
T &Vector<T>::operator()(int32_t i, int32_t j, int32_t k) {
  // @todo: actually use these for end, and indexing
  (void)j;
  (void)k;
  if (i >= static_cast<int32_t>(m_size)) {
    throw std::out_of_range(std::format(
        "ds::vector Error: Index {0} out of range for size {1}", i, m_size));
  }

  if (i < 0) {
    i += m_size;
  }

  return m_raw[i];
}

//==============================iter

template <typename T> Vector<T>::Iterator<T *> Vector<T>::begin() {
  return Iterator<T *>(&m_raw[0]);
}

template <typename T> Vector<T>::Iterator<T *> Vector<T>::end() {
  return Iterator<T *>(&m_raw[m_size]);
}

template <typename T> Vector<T>::Iterator<T *> Vector<T>::rbegin() {
  return Iterator<T *>(&m_raw[m_size]);
}

template <typename T> Vector<T>::Iterator<T *> Vector<T>::rend() {
  return Iterator<T *>(&m_raw[0]);
}

} // namespace ds

#endif

#ifndef DS_UNIQUE_PTR_H
#define DS_UNIQUE_PTR_H
#include <utility>

namespace DS {

template <typename T> class UniquePtr {
  T *m_ptr = nullptr;

public:
  UniquePtr() = default;
  // really important explicit here
  // if a func takes a uniqueptr, and someone passes in a raw ptr
  // function has silently taken ownership lol
  explicit UniquePtr(T *ptr) : m_ptr(ptr) {}

  UniquePtr(const UniquePtr &other) = delete;
  UniquePtr &operator=(const UniquePtr &other) = delete;

  UniquePtr(UniquePtr &&other) noexcept { m_ptr = other.release(); };
  UniquePtr &operator=(UniquePtr &&other) noexcept {
    if (this == &other) {
      return *this;
    }

    delete m_ptr;
    m_ptr = other.release();
    return *this;
  }

  ~UniquePtr() { delete m_ptr; }

  T *get() const { return m_ptr; }

  T &operator*() const noexcept { return *m_ptr; }

  T *operator->() const noexcept { return m_ptr; };

  void reset() noexcept {
    delete m_ptr;
    m_ptr = nullptr;
  }

  operator bool() const noexcept { return m_ptr != nullptr; }

  // should give up the underlying resource
  [[nodiscard]] T *release() noexcept {
    T *stolen = std::exchange(m_ptr, nullptr);
    return stolen;
  }

  void swap(UniquePtr &other) noexcept {
    if (&other == this) {
      return;
    }

    std::swap(other.m_ptr, this->m_ptr);
  };

  // friend is needed here to make non member func, lhs is no longer implicit
  friend bool operator==(const UniquePtr &lhs, const UniquePtr &rhs) noexcept {
    return lhs.get() == rhs.get();
  }
};

template <typename T, typename... P>
DS::UniquePtr<T> make_unique(P &&...params) {
  T *ptr = new T{std::forward<decltype(params)>(params)...};
  return DS::UniquePtr<T>{ptr};
}

} // namespace DS

#endif // DS_UNIQUE_PTR_H

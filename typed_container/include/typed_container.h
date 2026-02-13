#ifndef DS_TYPED_CONTAINER_H
#define DS_TYPED_CONTAINER_H

#include <cstddef>
#include <tuple>
#include <vector>
namespace ds {

template <typename... types> class TypedContainer {
public:
  //**
  // 1. Grab the underlying container with std::get<T>(tuple)
  // 2. Index into it
  // 3. deduce return type according decltype rules
  //
  // decltype ex.
  //
  // auto& ref = container.get<int>(0);
  // auto copy = container.get<int>(0);
  // int copy2 = container.get<int>(0);
  //  */
  template <typename T> auto get(size_t i) -> decltype(auto) {
    return std::get<std::vector<T>>(m_data)[i];
  }

  template <typename T> auto put(T object) -> void {
    std::get<std::vector<T>>(m_data).emplace_back(object);
  }

private:
  std::tuple<std::vector<types>...> m_data;
};

} // namespace ds

#endif

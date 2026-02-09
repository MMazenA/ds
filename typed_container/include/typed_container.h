#ifndef TYPED_CONTAINER
#define TYPED_CONTAINER

#include <cstddef>
#include <tuple>
#include <vector>
namespace ds {

template <typename... types> class TypedContainer {
  template <typename T> auto get(size_t i) {
    return std::get<std::vector<T>>(m_data)[i];
  }

private:
  std::tuple<std::vector<types>...> m_data;
};

} // namespace ds

#endif

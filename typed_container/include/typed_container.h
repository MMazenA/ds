#ifndef DS_TYPED_CONTAINER_H
#define DS_TYPED_CONTAINER_H

#include <cstddef>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>
namespace ds {

//**
// Comma operator to short circuit on type match on idx
//  */
template <typename T, typename... Ts>
constexpr std::size_t resolve_type_to_idx() {
  int idx = -1;
  ((++idx, std::is_same_v<T, Ts>) || ...);

  return static_cast<std::size_t>(idx);
}

template <typename... types> class TypedContainer {
public:
  using index_sequence = std::make_index_sequence<sizeof...(types)>;
  using size_type = std::size_t;

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
  //
  // all will get the correct type because of decltype(auto) generating them on
  // compilation
  // */
  template <typename T> auto get(size_type i) -> decltype(auto) {
    return std::get<std::vector<T>>(m_data)[i];
  }

  auto get(size_type i) -> std::variant<types...> {
    auto [type_idx, vec_idx] = m_tracking[i];

    return get_impl(type_idx, vec_idx,
                    std::make_index_sequence<sizeof...(types)>{});
  }

  template <typename T> auto put(T object) -> void {
    constexpr size_type idx = resolve_type_to_idx<T, types...>();
    m_tracking.emplace_back(idx, std::get<std::vector<T>>(m_data).size());
    std::get<std::vector<T>>(m_data).emplace_back(object);
  }

  template <typename T> auto operator[](int idx) -> decltype(auto) {
    return get(idx);
  }

private:
  using data_list_t = std::tuple<std::vector<types>...>;
  using tracking_vec_t = std::vector<std::pair<size_type, size_type>>;

  data_list_t m_data;
  tracking_vec_t m_tracking;

  template <size_t... Is>
  auto get_impl(size_t type_idx, size_t inner_idx, std::index_sequence<Is...>)
      -> std::variant<types...> {
    std::variant<types...> result;
    ((type_idx == Is && (result = std::get<Is>(m_data)[inner_idx], true)), ...);

    return result;
  }
};

} // namespace ds

#endif

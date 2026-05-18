#ifndef DS_MAP_INTERFACE_H
#define DS_MAP_INTERFACE_H

#include <concepts>
#include <cstddef>
#include <optional>

template <class T, class K, class V>
concept map_interface_c = requires(T &map, const T &const_map,
                                   const K &const_key, const V &const_value) {
  { map.insert(const_key, const_value) } -> std::same_as<bool>;
  { const_map.get(const_key) } -> std::same_as<std::optional<V>>;
  { const_map.contains(const_key) } -> std::same_as<bool>;
  { map.erase(const_key) } -> std::same_as<bool>;
  { const_map.size() } -> std::same_as<std::size_t>;
};

#endif

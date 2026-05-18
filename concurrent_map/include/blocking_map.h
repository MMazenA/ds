#ifndef DS_BLOCKING_MAP_H
#define DS_BLOCKING_MAP_H
#include "map_interface.h"

#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>

template <class K, class V> class BlockingMap {
  mutable std::shared_mutex m_mtx{};
  std::unordered_map<K, V> m_map{};

public:
  bool insert(const K &key, const V &value) {

    std::scoped_lock writer{m_mtx};
    auto [_, is_new_insert] = m_map.insert_or_assign(key, value);

    return is_new_insert;
  }

  std::optional<V> get(const K &key) const {
    std::shared_lock reader{m_mtx};
    auto value_iter = m_map.find(key);
    if (value_iter == m_map.end()) {
      return std::nullopt;
    }

    return std::optional{value_iter->second};
  }

  bool contains(const K &key) const {
    std::shared_lock reader{m_mtx};
    return m_map.contains(key);
  }

  bool erase(const K &key) {
    std::scoped_lock writer{m_mtx};
    size_t removed_count = m_map.erase(key);

    return removed_count > 0;
  }

  size_t size() const {
    std::shared_lock reader{m_mtx};
    return m_map.size();
  }
};

static_assert(map_interface_c<BlockingMap<int, int>, int, int>);

#endif

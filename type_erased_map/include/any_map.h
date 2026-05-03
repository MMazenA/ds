#ifndef ANY_MAP_H
#define ANY_MAP_H

#include <any>
#include <expected>
#include <string>
#include <unordered_map>
#include <utility>

namespace ds {
template <typename Key> class AnyMap {
  std::unordered_map<Key, std::any> m_data;

  /**
   * Conversion proxy that provides compile time generated conversions to
   * std::expected<T, std::string>
   *
   * This provides left hand side type deduction and on errored types gives
   * a string
   *
   * std::expected<int, std::string> will generate an any -> int conversion
   * on compile and compute the cast at runtime. Failed conversions result
   * in unexpected return std::string
   *
   * All callers are forced into lhs std::expected, T gets deducted through
   * implict conversion by compiler. I think this is mostly used in converting
   * classes into stl printable types, so for example, if i want to `cout <<` my
   * class, I can provide a direct conversion to some primitive type
   *
   *
   * std::any_cast is tracking some kind of typeid, and requires
   * EXACT type matches, the lhs conversion has to be exactly the same
   * as the stored type
   *
   *
   * The conversion operator is ref qualified &&, i basically want to enforce
   * the fact that this object must be ephemeral, m_proxy_item may dangle
   * its only intended use is to generate a typed copy, and then die
   *
   * consume once view, deleted everything except for the expected
   * ctor
   */
  class ConversionProxy {
    const std::any &m_proxy_item;

  public:
    explicit ConversionProxy(const std::any &proxy_item)
        : m_proxy_item(proxy_item) {}
    ConversionProxy(const ConversionProxy &) = delete;
    ConversionProxy(ConversionProxy &&) = delete;
    ConversionProxy &operator=(const ConversionProxy &) = delete;
    ConversionProxy &operator=(ConversionProxy &&) = delete;
    ~ConversionProxy() = default;

    template <typename T>
    [[nodiscard]] operator std::expected<T, std::string>() const && {
      if (auto *p = std::any_cast<T>(&m_proxy_item)) {
        return std::expected<T, std::string>{std::in_place, *p};
      }
      return std::expected<T, std::string>{std::unexpect,
                                           "lhs-value incorrect type"};
    }
  };

public:
  AnyMap() = default;

  void set(const Key &key, std::any &&value) { m_data[key] = std::move(value); }

  ConversionProxy get(const Key &key) const {
    return ConversionProxy{m_data.at(key)};
  }

  bool contains(const Key &key) const { return m_data.contains(key); }

  [[nodiscard]] size_t size() const { return m_data.size(); }
};

// bare implementation that above builds off
class AnyMapMinimum {
  std::unordered_map<std::string, std::any> m_data;

  struct ConversionType {
    std::any entity;
    explicit ConversionType(std::any ent) : entity(std::move(ent)) {}
    template <typename T> operator T() { return std::any_cast<T>(entity); }
  };

public:
  AnyMapMinimum() = default;
  void set(const std::string &key, std::any value) {
    m_data[key] = std::move(value);
  }

  [[nodiscard]] ConversionType get(const std::string &key) const {
    return ConversionType{m_data.at(key)};
  }
};

} // namespace ds

#endif

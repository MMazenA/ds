#ifndef ANY_MAP_H
#define ANY_MAP_H

#include <unordered_map>
#include <any>
#include <optional>
#include <expected>
#include <string>

namespace ds {
    template<typename Key>
    class AnyMap{
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
         * implict conversion
         * 
         */
        struct ConversionProxy{
            const std::any& m_proxy_item;
            ConversionProxy(const std::any& proxy_item) : m_proxy_item(proxy_item){}
    
            template <typename T>
            operator std::expected<T, std::string>() const {
                if(auto *p =  std::any_cast<T>(&m_proxy_item)){
                    return std::expected<T, std::string>{std::in_place, *p};
                }
                return std::expected<T, std::string>{std::unexpect, "l-value incorrect type"};
            }
           };
    public:
        AnyMap<Key>() = default;

        void set(const Key& key, std::any &&value){
            m_data[key] = std::move(value);

        }
        
        ConversionProxy get(const Key &key) const {
            return ConversionProxy{m_data.at(key)};
        }
    };

} // namespace ds

#endif

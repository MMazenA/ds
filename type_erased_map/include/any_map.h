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
         * implict conversion by compiler, teh same thing it does when it sees
         * float x = int + float, 2 different types get to still interact with each other
         * cuz int can promote up to float, same concept being used here
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
        class ConversionProxy{
            const std::any& m_proxy_item;
            public:
            explicit (const std::any& proxy_item) : m_proxy_item(proxy_item){}
            ConversionProxy(const ConversionProxy&) = delete;
            ConversionProxy(ConversionProxy&&) = delete;
            ConversionProxy& operator=(const ConversionProxy&) = delete;
            ConversionProxy& operator=(ConversionProxy&&) = delete;
            
            template <typename T>
            [[nodiscard]] operator std::expected<T, std::string>() const &&{
                if(auto *p =  std::any_cast<T>(&m_proxy_item)){
                    return std::expected<T, std::string>{std::in_place, *p};
                }
                return std::expected<T, std::string>{std::unexpect, "lhs-value incorrect type"};
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

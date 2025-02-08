#pragma once

/* C includes */
#include <time.h>
#include <stdint.h>

/* C++ includes */
#include <string>
#include <vector>
#include <functional>
#include <initializer_list>
#include <filesystem>
#include <map>
#include <type_traits>

/* uva includes */
#include <uva.hpp>
#include <format.hpp>

#define VAR_THROW_UNDEFINED_METHOD_FOR_TYPE(__type) throw std::runtime_error(std::format("undefined method '{}' for {}", UVA_FUNCTION_NAME, __type));
#define VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE() VAR_THROW_UNDEFINED_METHOD_FOR_TYPE(type)

#define VAR_THROW_CANT_CAST_TO_TYPE(left, right) throw std::runtime_error(std::format("{}: {} can't be casted to {}", UVA_FUNCTION_NAME, left, right));
#define VAR_THROW_CANT_CAST_TO_THIS_TYPE(left) VAR_THROW_CANT_CAST_TO_TYPE(left, type)

#undef max

namespace uva
{
    namespace core
    {
        // A simple dictionary class. It will be optimized later.
        template<typename ktype, typename vtype>
        class basic_dictionary
        {
        protected:
            std::vector<std::pair<ktype, vtype>> m_data;
        public:
            void insert(std::pair<ktype, vtype> __pair)
            {
                m_data.push_back(std::move(__pair));
            }

            size_t size() const
            {
                return m_data.size();
            }

            const vtype& operator[](const ktype& key) const
            {
                auto it = find(key);
                
                if(it == m_data.end())
                {
                    throw std::runtime_error(std::format("key not found in constant dictionary. It cannot be auto inserted."));
                }

                return it->second;
            }

            vtype& operator[](const ktype& key)
            {
                auto it = find(key);

                if(it == m_data.end())
                {
                    m_data.push_back({ key, vtype() });
                    return m_data.back().second;
                }

                return it->second;
            }

            template<typename T>
            const vtype& operator[](const T& key)
            {
                auto it = std::find_if(m_data.begin(), m_data.end(), [&key](const std::pair<ktype, vtype>& __pair) {
                    return __pair.first == key;
                });

                if(it == m_data.end())
                {
                    m_data.push_back({ ktype(key), vtype() });
                    return m_data.back().second;
                }

                return it->second;
            }

            auto begin()
            {
                return m_data.begin();
            }

            auto end()
            {
                return m_data.end();
            }

            const auto begin() const
            {
                return m_data.begin();
            }

            const auto end() const
            {
                return m_data.end();
            }

            auto find(const ktype& key)
            {
                return std::find_if(m_data.begin(), m_data.end(), [&key](const std::pair<ktype, vtype>& __pair) {
                    return __pair.first == key;
                });
            }

            auto find(const ktype& key) const
            {
                return std::find_if(m_data.begin(), m_data.end(), [&key](const std::pair<ktype, vtype>& __pair) {
                    return __pair.first == key;
                });
            }

            bool operator== (const basic_dictionary<ktype, vtype>& __dict) const
            {
                return m_data == __dict.m_data;
            }

            bool operator<(const basic_dictionary<ktype, vtype>& __dict) const
            {
                return m_data < __dict.m_data;
            }
        };
        class var
        {
        public:
            using integer_type = int64_t;
            using bool_type = int64_t;
            using real_type = double;
            using char_type = char;

            using string_type = std::basic_string<char_type>;

            using array_type = std::vector<var>;
            using array_iterator = array_type::iterator;
            using array_const_iterator = array_type::const_iterator;

            using map_type = std::map<var, var>;
            using map_iterator = map_type::iterator;
            using map_const_iterator = map_type::const_iterator;

            using dictionary_type           = basic_dictionary<var, var>;
        public:
            enum class var_type
            {
                undefined = -1,
                null_type = 0,
                integer,
                real,
                string,
                array,
                map,
                dictionary,
                color,
                max
            };
            //Function declarations MUST match this order:

            //null

            var() = default;

            //integer

            // Constructor enabled if T is an integral_type
            template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
            var(T __integer)
            {
                as<var::integer>() = __integer;
                type = var_type::integer;
            }

            //real

            var(const double& d);

            //string

            var(const char* str);
#ifdef __UVA_CPP20__
            var(const char8_t* __str);
#endif
#ifdef __UVA_CPP17__
            var(std::string_view __str);
#endif
            var(const char* str, size_t i);
            var(const string_type& _str);
            var(string_type&& _str);
            

            //array

            var(array_type&& __array);
            var(const array_type& __array);
            var(const std::vector<int>& __array);
            var(const std::vector<std::string>& __array);

            //map
            var(map_type&& __map);
            var(const map_type& __map);

            //dictionary
            var(dictionary_type&& __dictionary);
            var(const dictionary_type& __dictionary);
            
            //var

            /// @brief Copy constructor.
            /// @param other The var to copy
            var(const var& other);

            /// @brief Move constructor.
            /// @param other The var to move
            var(var&& other);

            //var_type

            var(const var_type& __array);
            ~var();

            //initializers
            
            /// @brief Creates a var from an integer-like initialization syntax
            /// @param __integer Anything which can be used to create an integer
            /// @return An var equivalent to an integer created with the same arguments
            static var integer(integer_type&& __integer);

            /// @brief Creates a var from a real-like initialization syntax
            /// @param __real Anything which can be used to create a real
            /// @return An var equivalent to a real created with the same arguments
            static var real(real_type&& i);

            /// @brief Creates a var from a string-like initialization syntax
            /// @param __string Anything which can be used to create a string
            /// @return An var equivalent to a string created with the same arguments
            static var string(string_type&& _string = string_type());

            /// @brief Creates a var from an array-like initialization syntax
            /// @param __array Anything which can be used to create an array
            /// @return An var equivalent to an array created with the same arguments
            static var array(array_type&& __array = array_type());

            /// @brief Creates a var from a map-like initialization syntax
            /// @param __map Anything which can be used to create a map
            /// @return An var equivalent to a map created with the same arguments
            static var map(map_type&& __map = map_type());
            /// @brief Creates a var from a dictionary-like initialization syntax
            /// @param __dictionary Anything which can be used to create a dictionary
            /// @return An var equivalent to a dictionary created with the same arguments
            static var dictionary(dictionary_type&& __dictionary = dictionary_type());
        public:
            var_type type = var_type::null_type;
            static constexpr size_t size_for_buffer = std::max({   
                sizeof(string_type),
                sizeof(integer_type),
                sizeof(real_type),
                sizeof(array_type),
                sizeof(map_type),
                sizeof(dictionary_type),
            });
            uint8_t m_value[size_for_buffer] = {0};
            //for debugging
#if __UVA_DEBUG_LEVEL__ >= 1
            integer_type*    m_integer_ptr    = (integer_type*)m_value;
            real_type*       m_real_ptr       = (real_type*)m_value;
            string_type*     m_string_ptr     = (string_type*)m_value;
            array_type*      m_array_ptr      = (array_type*)m_value;
            map_type*        m_map_ptr        = (map_type*)m_value;
            dictionary_type* m_dictionary_ptr = (dictionary_type*)m_value;
#endif
        private:
            void reconstruct(const var& var);
            void reconstruct(var&& var);
            void reconstruct(const var_type& __type);

            template<typename type>
            void reconstruct(type __val)
            {
                destruct();
                new(m_value) type(std::move(__val));
            }

            void destruct();
        protected:
            template<typename type>
            type& cast_to() const
            {
                return *((type*)m_value);
            }
        public:
            template<auto __type>
            const auto& as() const;

            template<auto __type>
            auto& as();
        public:
            template<auto __type>
            bool is_a() const;

            template<auto __type>
            auto&& move()
            {
                return std::move(this->as<__type>());
            }
        public:
            bool is_null() const;
            std::string to_s() const;
            std::string to_typed_s(char array_open = '{', char array_close = '}', bool double_quote = true) const;
            int64_t to_i() const;
            real_type to_f() const;
            /// @brief The size of the var.
            /// @return The number of elements in array, map, dictionary, or the size of the string.
            size_t size() const;
            /// @brief Index operator for arrays, maps, and dictionaries.
            /// @return The a element in array, map or dictionary
            const var& operator[](size_t i) const;
            /// @brief Index operator for arrays, maps, and dictionaries.
            /// @return The a element in array, map or dictionary
            const var& operator[](const std::string_view& key) const;
            /// @brief Index operator for arrays, maps, and dictionaries.
            /// @return The a element in array, map or dictionary
            const var& operator[](const char* key) const { return operator[](std::string_view(key)); }
        public:
            operator int() const;
            operator uint64_t() const;
            operator int64_t() const;
            operator std::string() const;
            operator bool() const;
            operator double() const;
            operator std::vector<int>() const;

            var& operator=(const var& other);
            var& operator=(var&& other);
            var& operator=(const bool& b);
            var& operator=(const int& i);
            var& operator=(const int64_t& i);
            var& operator=(const uint64_t& i);
            var& operator=(const double& d);
            var& operator=(const char* c);
#ifdef __UVA_CPP20__
            var& operator=(const char8_t* c);
            var& operator=(const std::u8string& s);
#endif
            var& operator=(const unsigned char* c);
            var& operator=(const std::string& s);
            var& operator=(array_type&& __array);
            var& operator=(const map_type& __map);
            var& operator=(const var_type& __type);

            template<auto __type>
            bool typed_compare(const var& other) const
            {
                return as<__type>() == other.as<__type>();
            }

            bool operator==(const var& v) const;
            bool operator==(std::string_view s) const;
            bool operator!=(const var& v) const;

            bool operator<(const var& v) const;

            template<var_type out_type>
            auto move()
            {
                return std::move(as<out_type>());
            }
//FRIEND OPERATORS BEGIN
            friend std::ostream& operator<<(std::ostream& stream, const var& holder)
            {
                stream << holder.to_s();
                return stream;
            }
        };
    };
};

using namespace uva::core;

#ifdef USE_FMT_FORMT
    template<>
    struct std::formatter<var>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx);

        template<typename FormatContext>
        auto format(var const& v, FormatContext& ctx);

    };

    template<typename ParseContext>
    constexpr auto std::formatter<var>::parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto std::formatter<var>::format(var const& v, FormatContext& ctx)
    {
        return std::format_to(ctx.out(), "{}", v.to_s());
    }

    template<>
    struct std::formatter<var::var_type>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx);

        template<typename FormatContext>
        auto format(var::var_type const& v, FormatContext& ctx) const;

    };

    template<typename ParseContext>
    constexpr auto std::formatter<var::var_type>::parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto std::formatter<var::var_type>::format(var::var_type const& type, FormatContext& ctx) const
    {
        switch(type)
        {
            case var::var_type::string:
                return std::format_to(ctx.out(), "{}", "string");
            break;
            case var::var_type::integer:
                return std::format_to(ctx.out(), "{}", "integer");
            break;
            case var::var_type::real:
                return std::format_to(ctx.out(), "{}", "real");
            break;
            case var::var_type::array:
                return std::format_to(ctx.out(), "{}", "array");
            break;
            case var::var_type::null_type:
                return std::format_to(ctx.out(), "{}", "null");
            break;
            case var::var_type::map:
                return std::format_to(ctx.out(), "{}", "map");
            break;
            case var::var_type::undefined:
                return std::format_to(ctx.out(), "{}", "undefined");
            break;
            default:
                throw std::runtime_error(std::format("invalid value of var::var_type: {}", (int)type));
            break;
        };
    }
#else
    template <>
    struct std::formatter<var> : std::formatter<std::string> {
        auto format(const var& v, format_context& ctx) {
            return std::format_to(ctx.out(), "{}", v.to_s());
        }
    };

    template <>
    struct std::formatter<var::var_type> : std::formatter<std::string> {
        auto format(var::var_type type, format_context& ctx) const {
            switch(type)
            {
                case var::var_type::null_type:
                    return std::format_to(ctx.out(), "{}", "null");
                break;
                case var::var_type::string:
                    return std::format_to(ctx.out(), "{}", "string");
                break;
                case var::var_type::integer:
                    return std::format_to(ctx.out(), "{}", "integer");
                break;
                case var::var_type::real:
                    return std::format_to(ctx.out(), "{}", "real");
                break;
                case var::var_type::array:
                    return std::format_to(ctx.out(), "{}", "array");
                break;
                case var::var_type::map:
                    return std::format_to(ctx.out(), "{}", "map");
                break;
                case var::var_type::color:
                    return std::format_to(ctx.out(), "{}", "color");
                break;
                case var::var_type::undefined:
                    return std::format_to(ctx.out(), "{}", "undefined");
                break;
                default:
                    throw std::runtime_error(std::format("invalid value of var::var_type: {}", (int)type));
                break;
            }
        }
    };
#endif

//methods that call std::format on var_type must be defined after the formatter. As templates need to be in header, define them here.
//Note: VAR_THROW_UNDEFINED_METHOD_FOR_TYPE does uses var_type formatter.
template<auto __type>
const auto& uva::core::var::as() const
{
    //GCC < 12 has a bug in the constexpr operator== for function pointers. Instead of returning false,
    //it throws compilation error. The function_is_same was created to allow this to work.

    if constexpr (uva::function_is_same<__type, var::integer>()) {

        return cast_to<integer_type>();

    }
    else if constexpr (uva::function_is_same<__type, var::real>()) {

        return cast_to<real_type>();

    }
    else if constexpr (uva::function_is_same<__type, var::string>()) {

        return cast_to<string_type>();

    }
    else if constexpr (uva::function_is_same<__type, var::array>()) {

        return cast_to<array_type>();

    }
    else if constexpr (uva::function_is_same<__type, var::map>()) {

        return cast_to<map_type>();

    }
    else if constexpr (uva::function_is_same<__type, var::dictionary>()) {

        return cast_to<dictionary_type>();
        
    }

    VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
}

template<auto __type>
auto& uva::core::var::as()
{
    //GCC < 12 has a bug in the constexpr operator== for function pointers. Instead of returning false,
    //it throws compilation error. The function_is_same was created to allow this to work.

    if constexpr (uva::function_is_same<__type, var::integer>()) {

        return cast_to<integer_type>();

    }
    else if constexpr (uva::function_is_same<__type, var::real>()) {

        return cast_to<real_type>();

    }
    else if constexpr (uva::function_is_same<__type, var::string>()) {

        return cast_to<string_type>();

    }
    else if constexpr (uva::function_is_same<__type, var::array>()) {

        return cast_to<array_type>();

    }
    else if constexpr (uva::function_is_same<__type, var::map>()) {

        return cast_to<map_type>();

    }
    else if constexpr (uva::function_is_same<__type, var::dictionary>()) {

        return cast_to<dictionary_type>();
        
    }

    VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
}

template<auto __type>
bool uva::core::var::is_a() const
{
    //GCC < 12 has a bug in the constexpr operator== for function pointers. Instead of returning false,
    //it throws compilation error. The function_is_same was created to allow this to work.

    if constexpr (uva::function_is_same<__type, var::integer>()) {

        return type == var_type::integer;

    }
    else if constexpr (uva::function_is_same<__type, var::real>()) {

        return type == var_type::real;

    }
    else if constexpr (uva::function_is_same<__type, var::string>()) {

        return type == var_type::string;

    }
    else if constexpr (uva::function_is_same<__type, var::array>()) {

        return type == var_type::array;

    }
    else if constexpr (uva::function_is_same<__type, var::map>()) {

        return type == var_type::map;

    }
    else if constexpr (uva::function_is_same<__type, var::dictionary>()) {

        return type == var_type::dictionary;
        
    }

    VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();

    return false;
}

#define var var
#define null var::var_type::null_type
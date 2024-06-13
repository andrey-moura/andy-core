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
#include <unordered_map>

/* uva includes */
#include <uva.hpp>
#include <format.hpp>

#define VAR_THROW_UNDEFINED_METHOD_FOR_TYPE(__type) throw std::runtime_error(std::format("undefined method '{}' for {}", UVA_FUNCTION_NAME, __type));
#define VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE() VAR_THROW_UNDEFINED_METHOD_FOR_TYPE(type)

#define VAR_THROW_CANT_CAST_TO_TYPE(left, right) throw std::runtime_error(std::format("{}: {} can't be casted to {}", UVA_FUNCTION_NAME, left, right));
#define VAR_THROW_CANT_CAST_TO_THIS_TYPE(left) VAR_THROW_CANT_CAST_TO_TYPE(left, type)

#undef max

#if __UVA_WIN__
    #define UVA_HAS_DICTIONARY
#endif

namespace uva
{
    namespace core
    {
        class var;
        struct times_helper
        {
            times_helper(size_t __times);
            size_t times;
            void operator()(std::function<void()> f) const;
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
#if         UVA_HAS_DICTIONARY
            using dictionary_type = std::unordered_map<std::string, var>;
            using dictionary_iterator = dictionary_type::iterator;
            using dictionary_const_iterator = dictionary_type::const_iterator;
#endif
            using color_type = color;
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

            var(const bool& boolean);
            var(const int& _integer);
            var(const uint64_t& _integer);
            var(const time_t& _integer);

            //real

            var(const double& d);

            //color

            var(const color_type& __color);

            //string

            var(const char* str);
#ifdef __UVA_CPP20__
            var(const char8_t* __str);
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
            var(const std::map<std::string, std::string>& __map);
            var(const std::map<std::string, var>& __map);

            //dictionary

            var(dictionary_type&& __dictionary);
            var(const dictionary_type& __dictionary);

            //var

            var(const var& other);
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
            void* m_value_ptr = nullptr;
            //for debugging
#if __UVA_DEBUG_LEVEL__ >= 1
            integer_type* m_integer_ptr = nullptr;
            real_type* m_real_ptr = nullptr;
            string_type* m_string_ptr = nullptr;
            array_type* m_array_ptr = nullptr;
            map_type* m_map_ptr = nullptr;
            dictionary_type* m_dictionary_ptr = nullptr;
            color_type* m_color_ptr = nullptr;
#endif
        private:
            void construct();
            void construct(void* __ptr);
            void* __construct();

            void reconstruct(const var& var);
            void reconstruct(var&& var);
            void reconstruct(const var_type& __type);

            template<typename type>
            void reconstruct(const type& __val)
            {
                destruct();
                construct();
                new(m_value_ptr) type(__val);
            }

            template<typename type>
            void reconstruct(const type&& __val)
            {
                destruct();
                construct();
                new(m_value_ptr) type(std::move(__val));
            }

            void destruct();
            void __delete();
            static constexpr size_t size_for_buffer = std::max({   
                sizeof(string_type),
                sizeof(integer_type),
                sizeof(real_type),
                sizeof(array_type),
                sizeof(map_type),
                sizeof(dictionary_type),
            });
        protected:
            template<typename type>
            type& cast_to() const
            {
                return *((type*)m_value_ptr);
            }
        public:
            template<auto __type>
            const auto& as() const;
            private:
            template<typename T>
            T& cast_to_non_const(const T& t)
            {
                return const_cast<T&>(t);
            }
            public:
            template<auto __type>
            auto& as()
            {
                return cast_to_non_const(const_cast<const var*>(this)->as<__type>());
            }
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
        public:
            operator int() const;
            operator uint64_t() const;
            operator int64_t() const;
            operator std::string() const;
            operator bool() const;
            operator double() const;
            operator std::vector<int>() const;
            operator uva::color() const;

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

            var operator+(const char* s) const;
            var operator+(const std::string& s) const;
            var operator+(const var& c) const;

            var& operator++();

            var& operator+=(const std::string& s);

            template<auto __type>
            bool typed_compare(const var& other) const
            {
                return as<__type>() == other.as<__type>();
            }
            bool operator==(std::string_view other) const;
            bool operator==(const var& other) const;
            bool operator==(const long& l) const;
            bool operator==(const double& d) const;
            bool operator==(const std::string& s) const;
            bool operator==(const bool& b) const;
            bool operator==(const int& other) const;
            bool operator==(const array_type& other) const;
            template<size_t N>
            bool operator==(const char(&other)[N]) const;
            bool operator!=(const var& v) const;
            bool operator!=(const double& d) const;
            bool operator!=(const char* s) const;
#ifdef __UVA_CPP20__
            bool operator!=(const char8_t* s) const;
#endif
            bool operator!=(const std::string& s) const;
            bool operator!=(const var_type& __type) const;

            bool operator<(const int& i) const;
            bool operator<(const time_t& i) const;
            bool operator<(const double& d) const;
            bool operator<(const var& other) const;
            template<typename T>
            bool operator>(const T& other) const
            {
                return ((T)*this) > other;
            }

            var operator/(const var& other) const;
            template<var_type out_type>
            auto move()
            {
                if(out_type != type)
                {
                    throw std::runtime_error(std::format("cannot move var of type '{}' into {}", type, out_type));
                } 

                return std::move(as<out_type>());
            }
//FRIEND OPERATORS BEGIN
            friend std::ostream& operator<<(std::ostream& stream, const var& holder)
            {
                stream << holder.to_s();
                return stream;
            }
            friend bool operator<(const double& d, const var& __other)
            {
                //return d < __other.as<var::real>();
                return false;
            }
            friend std::filesystem::path operator/(const std::filesystem::path& path, const var& __other)
            {
                switch (__other.type)
                {
                case var::var_type::null_type:
                        throw std::runtime_error("undefined method 'friend operator/(std::filesystem::path, var)' for null");
                    break;
                default:
                    return path / __other.to_s();
                    break;
                }
            }
//FRIEND OPERATORS END

//ARRAY FUNCTIONS
            /**
             *  @brief  Subscript access to the data contained in the %vector.
             *  @param __n The index of the element for which data should be
             *  accessed.
             *  @return  Read-only (constant) reference to data.
             *
             *  This operator allows for easy, array-style, data access.
             *  Note that data access with this operator is unchecked and
             *  out_of_range lookups are not defined. (For checked lookups
             *  see at().)
             */
            const var& operator[](const size_t& __n) const;

            /**
             *  @brief  Subscript access to the data contained in the %vector.
             *  @param __n The index of the element for which data should be
             *  accessed.
             *  @return  Read/write reference to data.
             *
             *  This operator allows for easy, array-style, data access.
             *  Note that data access with this operator is unchecked and
             *  out_of_range lookups are not defined. (For checked lookups
             *  see at().)
             */
            var& operator[](const size_t& __n);
            /**
             *  @brief  Subscript access to the data contained in the %vector.
             *  @param __n The index of the element for which data should be
             *  accessed.
             *  @return  Read-only (constant) reference to data.
             *
             *  This operator allows for easy, array-style, data access.
             *  Note that data access with this operator is unchecked and
             *  out_of_range lookups are not defined. (For checked lookups
             *  see at().)
             */
            const var& operator[](const int& __n) const;

            /**
             *  @brief  Subscript access to the data contained in the %vector.
             *  @param __n The index of the element for which data should be
             *  accessed.
             *  @return  Read/write reference to data.
             *
             *  This operator allows for easy, array-style, data access.
             *  Note that data access with this operator is unchecked and
             *  out_of_range lookups are not defined. (For checked lookups
             *  see at().)
             */
            var& operator[](const int& __n);

            /**
             *  @return         A read-only (constant) iterator that points to the first element in the %vector.
             */
            array_const_iterator begin() const;

            /**
             *  @return         A read/write iterator that points to the first element in the %vector.
             */
            array_iterator begin();
            
            /**
             *  @return         A read-only (constant) iterator that points one past the last element in the %vector.
             */
            array_const_iterator end() const;
            /**
             *  @return         A read/write iterator that points one past the last element in the %vector.
             */
            array_iterator end();

            /**
             *  @brief  Add data to the end of the %vector.
             *  @param  __x  Data to be added.
             *
             *  This is a typical stack operation.  The function creates an
             *  element at the end of the %vector and assigns the given data
             *  to it.  Due to the nature of a %vector this operation can be
             *  done in constant time if the %vector has preallocated space
             *  available.
             */
            void push_back(const var& __x);

            /**
             *  @brief  Move data to the end of the %vector.
             *  @param  __x  Data to be moved.
             *
             *  The function moves an element to the end of the %vector.
             *  Due to the nature of a %vector this operation can be
             *  done in constant time if the %vector has preallocated
             *  space available.
             */
            void push_back(var&& __x);
            /**
             *  @brief  Inserts given rvalue into %vector before specified iterator.
             *  @param  __position  A const_iterator into the %vector.
             *  @param  __x  Data to be inserted.
             *  @return  An iterator that points to the inserted data.
             *
             *  This function will insert a copy of the given rvalue before
             *  the specified location.  Note that this kind of operation
             *  could be expensive for a %vector and if it is frequently
             *  used the user should consider using std::list.
             */
            array_iterator insert(array_const_iterator __position, var&& __x);
            /**
             *  @brief  Inserts given rvalue into %vector before specified iterator.
             *  @param  __position  A const_iterator into the %vector.
             *  @param  __x  Data to be inserted.
             *  @return  An iterator that points to the inserted data.
             *
             *  This function will insert a copy of the given rvalue before
             *  the specified location.  Note that this kind of operation
             *  could be expensive for a %vector and if it is frequently
             *  used the user should consider using std::list.
             */
            array_iterator insert(array_iterator __position, var&& __x);
            /**
             *  @return true if the %vector is empty.
             */
            bool empty() const;
            /// @brief Access last element in the array
            /// @return a reference to the last element
            var& back();
            /**
             *  @brief Finds the first position in which @a val could be inserted
             *         without changing the ordering.
             *  @param  __val     The search term.
             *  @return         A read-only (constant) iterator pointing to the first element <em>not less
             *                  than</em> @a val, or end() if every element is less than
             *                  @a val.
             */
            array_const_iterator lower_bound(const uva::core::var& __val) const;

            /**
             *  @brief Finds the first position in which @a val could be inserted
             *         without changing the ordering.
             *  @param  __val     The search term.
             *  @return         An iterator pointing to the first element <em>not less
             *                  than</em> @a val, or end() if every element is less than
             *                  @a val.
             */
            array_iterator lower_bound(const uva::core::var& __val);

            array_iterator upper_bound(const uva::core::var& __val);

            /**
             *  @brief Finds the first position in which @a val could be inserted
             *         without changing the ordering.
             *  @param  __val     The search term.
             *  @param __comp     A functor to use for comparisons.
             *  @return         A read-only (constant) iterator pointing to the first element <em>not less
             *                  than</em> @a val, or end() if every element is less than
             *                  @a val.
             */
            template<typename _Compare>
            array_const_iterator lower_bound(const uva::core::var& __val, _Compare __comp) const
            {
                return std::lower_bound(begin(), end(), __val, __comp);
            }
            /**
             *  @brief Finds the first position in which @a val could be inserted
             *         without changing the ordering.
             *  @param  __val     The search term.
             *  @param __comp     A functor to use for comparisons.
             *  @return         An iterator pointing to the first element <em>not less
             *                  than</em> @a val, or end() if every element is less than
             *                  @a val.
             */
            template<typename _Compare>
            array_iterator lower_bound(const uva::core::var& __val, _Compare __comp)
            {
                return std::lower_bound(begin(), end(), __val, __comp);
            }
            /**
             *  @brief Apply a function to every element of a sequence.
             *  @param  __f      A unary function object.
             *
             *  Applies the function object.
             */
            void each(std::function<void(const uva::core::var& value)> __f) const;
            void each(std::function<void(uva::core::var& value)> __f);
            void each(void (*__f)(const char&)) const;
            private:
            void each_array(std::function<void(const uva::core::var& value)> __f) const;
            void each_array(std::function<void(uva::core::var& value)> __f);
            void each_string(std::function<void(const uva::core::var& value)> __f) const;
            void each_string(std::function<void(uva::core::var& value)> __f);
            public:
                template<typename T>
                void for_each(void(*function)(T&));
                template<typename T>
                void for_each(void(*function)(T&, void*), void* data = nullptr);
//END ARRAY FUNCTIONS
                bool includes(const var& value) const;
//ARRAY/MAP FUNCTIONS
            private:
                var join_array(const char& __separator) const;
                var join_map(const char& __separator) const;
            public:
            /**
             *  @brief Join all elements of self in string
             *  @param  __separator To separe joined elements
             *  @return A string containing the elements formated to string, separeted with @a __separator
             */
            var join(const char& __separator) const;
//END ARRAY/MAP FUNCTIONS

//ARRAY/STRING/MAP/DICTIONARY FUNCTIONS
            /**
             *  Erases all the elements.  Note that this function only erases the
             *  elements, and that if the elements themselves are pointers, the
             *  pointed-to memory is not touched in any way.  Managing the pointer is
             *  the user's responsibility.
             */
            void clear();
            /**
             *   Attempt to preallocate enough memory for specified number of elements.
            */
            void reserve(size_t __n);
            bool binary_search(const var& other) const;
            array_iterator insert_sorted(const var& item, bool distinct = true);
            public:
            /**
             *  @return The number of elements contained by %var if it is a container
             *  otherwise sizeof it.
             */
            size_t size() const;

            var fetch(const var& __value, const var& __default = var_type::null_type) const;

            /**
             *  @brief Fetches the value in the map corresponding to the key in __key. If key doesn’t exist then return __default.
             *  @param __key The key to find.
             *  @param __default The default value to return if key doesn't exist.
             *  @return The value corresponding to the key or __default if key doesn’t exist
             */
            var fetch_path(std::string_view __key, const var& __default = var_type::null_type) const;


//END ARRAY/STRING/MAP/DICTIONARY

//DATE TIME FUNCTIONS
            /**
             *  @brief Formats date according to the directives in given format.
             *  @param  __format The output format.
             *  @return The time represented in a string according to @a __format
             */
            var strftime(std::string_view __format);

//END DATE TIME FUNCTIONS

//STRING FUNCTIONS
            /**
             *  @brief Converts the starting letters of all words of this string to it's uppercase representation.
             *  @return A uppercased copy of self.
             */
            var capitalize();
            /**
             *  @brief Add suffix of plural to the word in self.
             *  @return A pluralized copy of self.
             */
            var to_downcase() const;
            /**
             *  @brief Converts all characters in the string to it's lowercase.
             *  @return A downcased copy of self.
             */
            var pluralize();
            /**
             *  @brief Formats @a __args according to the format in self.
             *  @param  __args The objects to be formated.
             *  @return The format in self with @a __args formating applied.
             */
            template<class... Args>
            var format(Args... __args);

            /// @brief Checks if string has a prefix
            /// @param sv The string which this should starts with
            /// @return True if this starts with sv, false otherwise
            bool starts_with(std::string_view sv) const;
            /// @brief Checks if string has a sufix
            /// @param sv The string which this should ends with
            /// @return True if this ends with sv, false otherwise
            bool ends_with(std::string_view sv) const;

            bool ends_with(const var& sufix) const;
//END STRING FUNCTIONS

//MAP FUNCTIONS
            const var& operator[](const var& __k) const;
            const var& operator[](const char* __k) const;

            var& operator[](const var& __k);
            var& operator[](const char* __k);
            var& operator[](const std::string& __k);
#ifdef __UVA_CPP20__
            const var& operator[](const char8_t* __k) const;
            var& operator[](const char8_t* __k);
#endif
            /**
             *  @brief Gives the key value corresponding to the value in __v. If value doesn’t exist then return null.
             *  @param __v value to find.
             *  @return key corresponding to the value or nil if value doesn’t exist
             */
            var key(const var& __v);

            /// @brief Append a string representation of this var into __str.
            /// @param __str The string which will receive the string representation of this.
            void append_to(std::string& __str, bool typed = false) const;

            var select(bool(*selector)(const var&));
//END MAP FUNCTIONS
        };

        var now();
        var parse_argument_list(const std::string& argument_list);
    };
};

using namespace uva::core;

times_helper operator ""_times(unsigned long long times);
var          operator ""_var(char const* str, std::size_t i);
var          operator ""_percent(unsigned long long d);


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
        auto format(var::var_type const& v, FormatContext& ctx);

    };

    template<typename ParseContext>
    constexpr auto std::formatter<var::var_type>::parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto std::formatter<var::var_type>::format(var::var_type const& type, FormatContext& ctx)
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
        auto format(var::var_type type, format_context& ctx) {
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
        
    } else {
        return *this;
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

template<class... Args>
var uva::core::var::format(Args... __args)
{
    switch (type)
    {
    case var::var_type::string:
        return std::vformat(as<var::string>(), std::make_format_args(__args...));
    break;
    default:
        VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
    break;
    }
}

template<size_t N>
bool uva::core::var::operator==(const char(&other)[N]) const
{
    switch(type)
    {
        case var_type::null_type:
            return false;
        break;
        case var_type::string:
            return as<var::string>() == other;
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    return false;
}

template<typename T>
void uva::core::var::for_each(void(*function)(T&))
{   
    switch(type)
    {
        case var_type::array:
        {
            auto& array = as<var::array>();
            for(size_t i = 0; i < array.size(); ++i) {
                function(array[i]);
            }
        }
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

template<typename T>
void uva::core::var::for_each(void(*function)(T&, void*), void* data)
{   
    switch(type)
    {
        case var_type::array:
        {
            auto& array = as<var::array>();
            for(size_t i = 0; i < array.size(); ++i) {
                function(array[i], data);
            }
        }
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

#define var var
#define null var::var_type::null_type
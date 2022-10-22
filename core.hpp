#pragma once

#include <string>
#include <vector>
#include <functional>
#include <initializer_list>
#include <format>
#include <time.h>
#include <filesystem>

#include <string.hpp>

namespace uva
{
    namespace core
    {
        struct times_helper
        {
            times_helper(size_t __times);
            size_t times;
            void operator()(std::function<void()> f) const;
        };
        class var
        {
//ARRAY DEFINITIONS
            using array_type = std::vector<var>;
            using array_iterator = array_type::iterator;
            using array_const_iterator = array_type::const_iterator;
//END ARRAY DEFINITIONS
//MAP DEFINITIONS
            using map_type = std::map<var, var>;
            using map_iterator = map_type::iterator;
            using map_const_iterator = map_type::const_iterator;
//ND MAP DEFINITIONS
        public:
            enum class var_type
            {
                null_type,
                string,
                integer,
                real,
                array,
                map
            };
            var(std::initializer_list<var> l);
            var();
            var(const var& other) = default;
            var(var&& other);
            var(const uint64_t& _integer);
            var(const int& _integer);
            var(const std::string& _str);
            var(const time_t& _integer);
            var(const char* str);
            var(const char* str, size_t i);
            var(const bool& boolean);
            var(const double& d);
            var(const array_type& __array);
            var(array_type&& __array);
            var(const var_type& __array);
        private:
            void construct(const var_type& __type);
            void construct(var&& var);
            void construct(array_type&& __array);
        public:

            var_type type;

            std::string str;
            int64_t integer;
            double real;
            array_type array;
            map_type map;
        public:
            bool is_null() const;
            std::string to_s() const;
            int64_t to_i() const;
        public:
            operator int() const;
            operator uint64_t() const;
            operator int64_t() const;
            operator std::string() const;
            operator bool() const;
            operator double() const;

            var& operator=(std::initializer_list<var> __array);
            var& operator=(const var& other);
            var& operator=(var&& other);
            var& operator=(const bool& b);
            var& operator=(const int& i);
            var& operator=(const int64_t& i);
            var& operator=(const uint64_t& i);
            var& operator=(const double& d);
            var& operator=(const char* c);
            var& operator=(const unsigned char* c);
            var& operator=(const std::string& s);
            var& operator=(array_type&& __array);
            var& operator=(const var_type& __type);

            var operator+(const std::string& s) const;
            var& operator+=(const std::string& s);

            bool operator==(const var& other) const;
            bool operator==(const long& l) const;
            bool operator==(const double& d) const;
            bool operator==(const std::string& s) const;
            bool operator==(const bool& b) const;
            bool operator==(const int& other) const;

            bool operator!=(const double& d) const;
            bool operator!=(const std::string& s) const;

            bool operator<(const int& i) const;
            bool operator<(const time_t& i) const;
            bool operator<(const double& d) const;
            bool operator<(const var& other) const;
            template<typename T>
            bool operator>(const T& other) const
            {
                return ((T)*this) > other;
            }
//FRIEND OPERATORS BEGIN
            friend std::ostream& operator<<(std::ostream& stream, const var& holder)
            {
                stream << holder.to_s();
                return stream;
            }
            friend bool operator<(const double& d, const var& h)
            {
                return d < h.real;
            }
            friend std::filesystem::path operator/(const std::filesystem::path& path, const var& var)
            {
                switch (var.type)
                {
                case var::var_type::null_type:
                        throw std::runtime_error("undefined method 'friend operator/(std::filesystem::path, var)' for null");
                    break;
                default:
                    return path / var.to_s();
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
            private:
            void each_array(std::function<void(const uva::core::var& value)> __f) const;
            void each_array(std::function<void(uva::core::var& value)> __f);
            void each_string(std::function<void(const uva::core::var& value)> __f) const;
            void each_string(std::function<void(uva::core::var& value)> __f);
            public:
//END ARRAY FUNCTIONS

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

//ARRAY/STRING/MAP FUNCTIONS
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
//END ARRAY/STRING/MAP
            /**
             *  @return The number of elements contained by %var if it is a container
             *  otherwise sizeof it.
             */
            size_t size() const;

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
            var downcase();
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
            var format(Args... __args)
            {
                switch (type)
                {
                case var::var_type::string:
                    #ifdef USE_FMT_FORMT
                        return vformat(str, std::make_format_args(__args...));
                    #else
                        return std::format(str, std::forward<Args>(__args)...);
                    #endif       
                break;
                default:
                    throw std::runtime_error(std::format("undefined method 'format' for {}", type));
                break;
                }
            }
//END STRING FUNCTIONS
        };

        var now();
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
            default:
                throw std::runtime_error(std::format("invalid value of var::var_type: {}", (int)type));
            break;
        };
    }

#endif

#define var var
#define null var::var_type::null_type
#define empty_map var::var_type::map
#define empty_array var::var_type::array
#define self (*this)
#define VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE() throw std::runtime_error(std::format("undefined method '{}' for {}", __PRETTY_FUNCTION__, type))
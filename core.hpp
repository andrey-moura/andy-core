#pragma once

#include <string>
#include <vector>
#include <functional>
#include <initializer_list>
#include <format>
#include <time.h>

namespace uva
{
    namespace core
    {
        class var
        {
//ARRAY DEFINITIONS
            using array_type = std::vector<var>;
            using iterator = array_type::iterator;
            using const_iterator = array_type::const_iterator;
//END ARRAY DEFINITIONS
        public:
            var();
            var(const var& other) = default;
            var(var&& other);
            var(const uint64_t& _integer);
            var(const int& _integer);
            var(const std::string& _str);
            var(const time_t& _integer);
            var(const char* str);
            var(const bool& boolean);
            var(const double& d);
            var(std::initializer_list<var>&& l);

            enum class var_type
            {
                null_type,
                string,
                integer,
                real,
                array
            };

            var_type type;

            std::string str;
            int64_t integer;
            double real;
            std::vector<var> array;
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

            var& operator=(const var& other);
            var& operator=(const bool& b);
            var& operator=(const int& i);
            var& operator=(const int64_t& i);
            var& operator=(const uint64_t& i);
            var& operator=(const double& d);
            var& operator=(const char* c);
            var& operator=(const unsigned char* c);
            var& operator=(const std::string& s);


            bool operator==(const var& other) const;
            bool operator==(const long& l) const;
            bool operator==(const double& d) const;
            bool operator==(const std::string& s) const;
            bool operator==(const bool& b) const;

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
                        throw std::runtime_error("no such operator/(std::filesystem::path, var) which takes var(null)");
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
            const_iterator begin() const;

            /**
             *  @return         A read/write iterator that points to the first element in the %vector.
             */
            iterator begin();
            
            /**
             *  @return         A read-only (constant) iterator that points one past the last element in the %vector.
             */
            const_iterator end() const;
            /**
             *  @return         A read/write iterator that points one past the last element in the %vector.
             */
            iterator end();

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
            iterator insert(const_iterator __position, var&& __x);
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
            iterator insert(iterator __position, var&& __x);

            /**
             *  Erases all the elements.  Note that this function only erases the
             *  elements, and that if the elements themselves are pointers, the
             *  pointed-to memory is not touched in any way.  Managing the pointer is
             *  the user's responsibility.
             */
            void clear();

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
            const_iterator lower_bound(const uva::core::var& __val) const;

            /**
             *  @brief Finds the first position in which @a val could be inserted
             *         without changing the ordering.
             *  @param  __val     The search term.
             *  @return         An iterator pointing to the first element <em>not less
             *                  than</em> @a val, or end() if every element is less than
             *                  @a val.
             */
            iterator lower_bound(const uva::core::var& __val);

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
            const_iterator lower_bound(const uva::core::var& __val, _Compare __comp) const
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
            iterator lower_bound(const uva::core::var& __val, _Compare __comp)
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
        };

        var now() { return time(nullptr); }
    };
};

using namespace uva::core;

#define var var
#define null var()

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
            case var::var_type::null_type:
                return std::format_to(ctx.out(), "{}", "null");
            break;
        };
    }

#endif
#include <uva/core.hpp>
#include "core.hpp"

static char s_buffer[100];

times_helper::times_helper(size_t __times)
    : times(__times)
{

}

void times_helper::operator()(std::function<void()> f) const
{
    for(size_t i = 0; i < times; ++i)
    {
        f();
    }
}

times_helper operator ""_times(unsigned long long times)
{
    return times_helper(times);
}

var operator ""_var(char const* str, std::size_t i)
{
    return var(str, i);
}

var operator ""_percent(unsigned long long d)
{
    return var((double)d / 100.0);
}

//VAR

//INTEGER CONSTRUCTORS

var::var(const bool& __boolean)
{
    construct();
    as<var::integer>() = __boolean;
    type = var_type::integer;
}

var::var(const int& __integer)
{
    construct();
    as<var::integer>() = __integer;
    type = var_type::integer;
}

var::var(const uint64_t& __integer)
{
    construct();
    as<var::integer>() = __integer;
    type = var_type::integer;
}

var::var(const time_t& __integer)
{
    construct();
    as<var::integer>() = __integer;
    type = var_type::integer;
}

//REAL CONSTRUCTORS

var::var(const double& d)
{
    construct();
    as<var::real>() = d;
    type = var_type::real;
}

//COLOR CONSTRUCTORS

// uva::core::var::var(const color_type &__color)
// {
//     construct();
//     new(m_value_ptr) color_type(__color);
//     type = var_type::color;
// }

//STRING CONSTRUCTORS

var::var(const char* __str)
{
    construct();
    new(m_value_ptr) string_type(__str);
    type = var_type::string;
}

uva::core::var::var(const char8_t *__str)
{
    construct();
    new(m_value_ptr) string_type((const char*)__str);
    type = var_type::string;
}

var::var(const char* __str, size_t __len)
{
    construct();
    new(m_value_ptr) string_type(__str, __len);
    type = var_type::string;
}

var::var(const std::string& __str)
{
    construct();
    new(m_value_ptr) string_type(__str);
    type = var_type::string;
}

uva::core::var::var(string_type && __string)
{
    construct();
    new(m_value_ptr) string_type(std::move(__string));
    type = var_type::string;    
}

//ARRAY CONSTRUCTORS

var::var(array_type&& __array)
{
    construct();
    new(m_value_ptr) array_type(std::move(__array));
    type = var_type::array;
}

var::var(const array_type& __array)
{
    construct();
    new(m_value_ptr) array_type(__array);
    type = var_type::array;
}

uva::core::var::var(const std::vector<int> &__array)
{
    construct();
    new(m_value_ptr) array_type();
    type = var_type::array;

    size_t size = __array.size();

    as<var::array>().reserve(size);

    for(size_t i = 0; i < size; ++i)
    {
        as<var::array>().push_back(var(__array[i]));
    }
}

uva::core::var::var(const std::vector<std::string> &__array)
{
    construct();
    new(m_value_ptr) array_type();
    type = var_type::array;

    size_t size = __array.size();

    as<var::array>().reserve(size);

    for(size_t i = 0; i < size; ++i)
    {
        as<var::array>().push_back(var(__array[i]));
    }
}

//MAP CONSTRUCTORS

var::var(map_type&& __map)
{
    construct();
    new(m_value_ptr) map_type(std::move(__map));
    type = var_type::map;
}

uva::core::var::var(const map_type & __map)
{
    construct();
    new(m_value_ptr) map_type(__map);
    type = var_type::map;
}

uva::core::var::var(const std::map<std::string, std::string> &__map)
{
    construct();
    new(m_value_ptr) map_type();
    type = var_type::map;

    for(const auto& pair : __map)
    {
        as<var::map>().insert({var(std::move(pair.first)), var(std::move(pair.second))});
    }
}

uva::core::var::var(const std::map<std::string, uva::core::var> &__map)
{
    construct();
    new(m_value_ptr) map_type();
    type = var_type::map;

    for(const auto& pair : __map)
    {
        as<var::map>().insert({var(std::move(pair.first)), var(std::move(pair.second))});
    }
}

//DICIONARY CONSTRUCTORS

uva::core::var::var(dictionary_type &&__dictionary)
{
    construct();
    new(m_value_ptr) dictionary_type(std::move(__dictionary));
    type = var_type::dictionary;
}

uva::core::var::var(const dictionary_type &__dictionary)
{
    construct();
    new(m_value_ptr) dictionary_type(__dictionary);
    type = var_type::dictionary;
}

//VAR CONSTRUCTORS

var::var(var&& other)
{
    reconstruct(std::move(other));
}

var::var(const var &other)
{
    reconstruct(other);
}

//VAR_TYPE CONSTRUCTORS

var::var(const var_type& __type)
{
    reconstruct(__type);
}

//CONTRUCTORS HELPERS

void* var::__construct()
{
    return new uint8_t[size_for_buffer];
}

void var::construct(void* __ptr)
{
    m_value_ptr = __ptr;

#if __UVA_DEBUG_LEVEL__ > 0
    m_integer_ptr    = (integer_type*)m_value_ptr;
    m_real_ptr       = (real_type*)m_value_ptr;
    m_string_ptr     = (string_type*)m_value_ptr;
    m_array_ptr      = (array_type*)m_value_ptr;
    m_map_ptr        = (map_type*)m_value_ptr;
    m_dictionary_ptr = (dictionary_type*)m_value_ptr;
#endif
}

void var::construct()
{
    if(!m_value_ptr) {
        construct(__construct());
    }
}

void var::reconstruct(const var& other)
{
    destruct();
    construct();

    switch(other.type)
    {
        case var_type::null_type:
        break;
        case var_type::integer:
            //does not need constructor
            as<var::integer>() = other.as<var::integer>();
        break;
        case var_type::real:
            //does not need constructor
            as<var::real>() = other.as<var::real>();
        break;
        case var_type::string:
            new(m_value_ptr) string_type(other.as<var::string>());
        break;
        case var_type::array:
            new(m_value_ptr) array_type(other.as<var::array>());
        break;
        case var_type::map:
            new(m_value_ptr) map_type(other.as<var::map>());
        break;
        case var_type::dictionary:
            new(m_value_ptr) dictionary_type(other.as<var::dictionary>());
        break;
        case var_type::undefined:
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    type = other.type;
}

void var::reconstruct(var&& __var)
{
    destruct();

    construct(__var.m_value_ptr);

    type = __var.type;

    __var.m_value_ptr = nullptr;
    __var.type = var_type::null_type;
}

void var::reconstruct(const var_type& __type)
{
    destruct();
    construct();

    switch (__type)
    {
    case var_type::integer:
    case var_type::real:
    break;
    case var_type::string:
        new(m_value_ptr) string_type();
    break;
    case var_type::array:
        new(m_value_ptr) array_type();
    break;
    case var_type::map:
        new(m_value_ptr) map_type();
    break;
    case var_type::dictionary:
        new(m_value_ptr) dictionary_type();
    break;
    // case var_type::color:
    //     as<var::color>().~color();
    // break;
    case var_type::null_type:
        break;
    case var_type::undefined:
    break;
    default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    type = __type;
}

var::~var()
{
    __delete();
}

var uva::core::var::integer(integer_type &&__integer)
{
    return var(std::move(__integer));
}

var uva::core::var::real(real_type&& i)
{
    return var(std::move(i));
}

var uva::core::var::string(string_type &&__string)
{
    return var(std::move(__string));
}

var uva::core::var::array(array_type &&__array)
{
    return var(std::move(__array));
}

var uva::core::var::map(map_type && __map)
{
    return var(std::move(__map));
}

var uva::core::var::dictionary(dictionary_type &&__dictionary)
{
    return var(std::move(__dictionary));
}

void var::__delete()
{
    destruct();
    delete[] (uint8_t*)m_value_ptr;
}

void var::destruct()
{
    if(m_value_ptr)
    {
        switch (type)
        {
            case var_type::null_type:
            break;
            case var_type::integer:
            case var_type::real:
            break;
            case var_type::string:
                as<var::string>().~basic_string();
            break;
            case var_type::array:
                as<var::array>().~vector();
            break;
            case var_type::map:
                as<var::map>().~map();
            break;
            case var_type::dictionary:
                as<var::dictionary>().~unordered_map();
            break;
            // case var_type::color:
            //     as<var::color>().~color();
            // break;
            case var_type::undefined:
            break;
            default:
                VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
            break;
        }
    }
}

bool var::is_null() const
{
    return type == var_type::null_type;
}

std::string var::to_s() const
{
    switch(type)
    {
        case var_type::null_type:
            return "";
        break;
        case var_type::string:
            return as<var::string>();
        break;
        case var_type::integer:
            return std::to_string(as<var::integer>());
        case var_type::real:
            return std::format("{}", as<var::real>());
        case var_type::array: {
            std::string s = "{";
            size_t reserved = as<var::array>().size()*64;
            s.reserve(reserved);
            for(const auto& p : as<var::array>())
            {
                s+=p.to_s();
                s.push_back(',');
            }
            if(s.size()) {
                s.pop_back();
            }
            s += " }";

            UVA_CHECK_RESERVED_BUFFER(s, reserved);
            return s;
        }
        break;
        case var_type::map:
            {
                std::string s = "{";

                for(const auto& p : as<var::map>())
                {
                    s += " {";

                    if(p.first.type == var_type::string) {
                        s.push_back('\"');
                    }

                    s += p.first.to_s();

                    if(p.first.type == var_type::string) {
                        s.push_back('\"');
                    }

                    s+= ", ";

                    if(p.second.type == var_type::string) {
                        s.push_back('\"');
                    }

                    s += p.second.to_s();

                    if(p.second.type == var_type::string) {
                        s.push_back('\"');
                    }

                    s += " },";
                }

                s += " }";

                return s;
            }
        break;
    }

    VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
}

std::string uva::core::var::to_typed_s(char array_open, char array_close, bool double_quote) const
{
    char quote;
    char other_quote;

    if(double_quote) {
        quote = '"';
        other_quote = '\'';
    } else {
        quote = '\'';
        other_quote = '"';
    }

    switch(type)
    {
        case var_type::null_type:
            return "null";
        break;
        case var_type::string:
            {
                std::string ret;
                ret.reserve(as<var::string>().size()+15);

                ret.push_back(quote);
                
                std::string_view sv = as<var::string>();

                while(sv.size()) {
                    const char& c = sv.front();
                    switch(c)
                    {
                        case '\t':
                            ret.push_back('\\');
                            ret.push_back('t');
                        break;
                        case '\n':
                            ret.push_back('\\');
                            ret.push_back('n');
                        break;
                        case '\r':
                            ret.push_back('\\');
                            ret.push_back('r');
                        break;
                        case '"':
                            if(double_quote) {
                                ret.push_back('\\');
                            }
                            
                            ret.push_back('\"');
                        break;
                        case '\'':
                            if(double_quote) {
                                ret.push_back('\\');
                            }

                            ret.push_back('\'');
                        break;
                        case '\\':
                            ret.push_back('\\');
                            ret.push_back('\\');
                        break;
                        default:
                            ret.push_back(c);
                        break;
                    }

                    sv.remove_prefix(1);
                }

                ret.push_back(quote);

                return ret;
            }
        break;
        case var_type::integer:
            return std::to_string(as<var::integer>());
        case var_type::real:
            return std::format("{}", as<var::real>());
        case var_type::array: {
            std::string s;
            size_t reserved = as<var::array>().size()*64;
            s.reserve(reserved);
            
            if(array_open) {
                s.push_back(array_open);
                s.push_back(' ');
            }

            for(const auto& p : as<var::array>())
            {
                s+=p.to_typed_s(array_open, array_close, double_quote);
                s.push_back(',');
            }

            if(s.size()) {
                s.pop_back();
                s.push_back(' ');
            }

            if(array_close) {
                s.push_back(array_close);
                s.push_back(' ');
            }

            UVA_CHECK_RESERVED_BUFFER(s, reserved);
            return s;
        }
        break;
        case var_type::map:
            {
                std::string map_buffer = "{";

                for(const auto& p : as<var::map>())
                {
                    map_buffer += p.first.to_typed_s();

                    map_buffer.push_back(':');
                    map_buffer.push_back(' ');

                    map_buffer += p.second.to_typed_s();

                    map_buffer.push_back(',');
                    map_buffer.push_back(' ');
                }

                map_buffer.pop_back();
                map_buffer.pop_back();

                map_buffer += " }";

                return map_buffer;
            }
        break;
    }

    VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
}

int64_t var::to_i() const
{
    switch(type)
    {
        case var_type::null_type:
            return 0;
        break;
        case var_type::string:
        {
            std::string string = as<var::string>();
            bool negative = false;

            if(string.starts_with('-')) {
                negative = true;
                string.erase(0);
            }

            for(const char& c : string) {
                if(!isdigit(c)) {
                    throw std::runtime_error(std::format("invalid character '{}' in string \"{}\" while converting to integer", c, as<var::real>()));
                }
            }

            int64_t _integer = std::stoll(string);
            if(negative) {
                _integer *= -1;
            }

            return _integer;
        }
        break;
        case var_type::real:
            return (int)as<var::real>();
        break;
        case var_type::integer:
            return as<var::integer>();
        break;
    }

    throw std::runtime_error(std::format("failed to convert from (var_type){} to integer", (size_t)type));
    return -1;
}

var::real_type var::to_f() const
{
    switch(type)
    {
        case var_type::null_type:
            return 0;
        break;
        case var_type::real:
            return as<var::real>();
        break;
        case var_type::integer:
            return (real_type)as<var::integer>();
        break;
    }

    VAR_THROW_CANT_CAST_TO_TYPE(type, var_type::real);
}

var::operator int() const
{
    return (int)as<var::integer>();
}

var::operator uint64_t() const
{
    return (uint64_t)as<var::integer>();
}

var::operator int64_t() const
{
    return (int64_t)as<var::integer>();
}

var::operator std::string() const
{
    switch (type)
    {
    case var_type::null_type:
        return "";
        break;
    case var_type::string:
        return as<var::string>();
    break;
    default:
        VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

var::operator bool() const
{
    switch (type)
    {
    case var_type::null_type:
        return false;
        break;
    case var_type::integer:
        return (bool)as<var::integer>();
    break;
    case var_type::string:
        return as<var::string>().size();
    break;
    case var_type::array:
        return as<var::array>().size();
    break;
    case var_type::map:
        return as<var::map>().size();
    break;
    case var_type::dictionary:
        return as<var::dictionary>().size();
    break;
    default:
        VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

var::operator double() const
{
    return as<var::real>();
}

var::operator std::vector<int>() const
{
    if(type == var_type::array)
    {
        const array_type& array = as<var::array>();

        std::vector<int> __arr;
        __arr.reserve(array.size());
        
        for(int i = 0; i < array.size(); ++i)
        {
            __arr.push_back((int)array[i].as<var::integer>());
        }

        return __arr;
    } else {
        VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
    }
}

uva::core::var::operator uva::color() const
{
    switch (type)
    {
    // case var_type::color:
    //     return as<var::color>();
    //     break;
    default:
        VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

var& var::operator=(const uint64_t& i)
{
    if(type == var_type::integer)
    {
        as<var::integer>() = i;
    } else {
        reconstruct<integer_type>(i);
        type = var::var_type::integer;
    }

    return *this;
}

var& var::operator=(const int64_t& i)
{
    if(type == var_type::integer)
    {
        as<var::integer>() = i;
    } else {
        reconstruct<integer_type>(i);
        type = var::var_type::integer;
    }

    return *this;
}

var& var::operator=(const bool& b)
{
    constexpr var_type __type = var_type::integer;
    int64_t i = (int64_t)(b);

    if(type == __type)
    {
        as<var::integer>() = i;
    } else {
        reconstruct<integer_type>(i);
        type =__type;
    }

    return *this;
}

var &uva::core::var::operator=(const int &i)
{
    constexpr var_type __type = var_type::integer;

    if(type == __type)
    {
        as<var::integer>() = i;
    } else {
        reconstruct<integer_type>(i);
        type =__type;
    }

    return *this;
}

var& var::operator=(const double& d)
{
    constexpr var_type __type = var_type::real;

    if(type == __type)
    {
        as<var::real>() = d;
    } else {
        reconstruct<real_type>(d);
        type =__type;
    }

    return *this;
}

var& var::operator=(const char* str)
{
    if(type == var_type::string)
    {
        as<var::string>() = str;
    } else {
        reconstruct<string_type>(str);
        type = var::var_type::string;
    }

    return *this;
}

var &uva::core::var::operator=(const char8_t *c)
{
    return operator=((const char*)c);
}

var& var::operator=(const unsigned char* ustr)
{
    const char* str = (const char*)ustr;

    if(type == var_type::string)
    {
        as<var::string>() = str;
    } else {
        reconstruct<string_type>(str);
        type = var::var_type::string;
    }

    return *this;
}

var& var::operator=(const std::string& str)
{
    if(type == var_type::string)
    {
        as<var::string>() = str;
    } else {
        reconstruct<string_type>(str);
        type = var::var_type::string;
    }

    return *this;
}

var &uva::core::var::operator=(const std::u8string &s)
{
    if(type == var_type::string)
    {
        as<var::string>() = std::string((const char*)s.data(), s.size());
    } else {
        reconstruct<string_type>(std::string((const char*)s.data(), s.size()));
        type = var::var_type::string;
    }

    return *this;
}

var& var::operator=(array_type&& __array)
{
    constexpr var_type __type = var_type::array;

    if(type == __type)
    {
        as<var::array>() = __array;
    } else {
        reconstruct<array_type>(__array);
        type = __type;
    }

    return *this;
}

var &uva::core::var::operator=(const map_type & __map)
{
    constexpr var_type __type = var_type::map;

    if(type == __type)
    {
        as<var::map>() = __map;
    } else {
        reconstruct<map_type>(__map);
        type = __type;
    }

    return *this;
}

var& var::operator=(const var& other)
{
    reconstruct(other);

    type = other.type;
    return *this;
}

var& var::operator=(var&& other)
{
    reconstruct(std::move(other));
    return *this;
}

var& var::operator=(const var_type& __type)
{
    reconstruct(__type);
    return *this;
}

var uva::core::var::operator+(const char *s) const
{
    switch (type)
    {
        case var::var_type::string:{
            var new_var = *this;
            new_var += s;
            return new_var;
        }
        break;
    }

    VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
}

var var::operator+(const std::string &s) const
{
    switch (type)
    {
    case var::var_type::string:{
        var new_var = *this;
        new_var += s;
        return new_var;
    }
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'operator+(std::string)' for {}", type));
        break;
    }
}

var uva::core::var::operator+(const var &v) const
{
    switch (type)
    {
        case var_type::string:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
        case var_type::integer:
            switch(v.type)
            {
                case var_type::integer:
                    return as<var::integer>() + v.as<var::integer>();
                break;
            };
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

var &uva::core::var::operator++()
{
    switch (type)
    {
        case var_type::integer:
            as<var::integer>()++;
        break;
        case var_type::real:
            as<var::real>()++;
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    return *this;
}

var& var::operator+=(const std::string& s)
{
    switch (type)
    {
    case var::var_type::string:{
        as<var::string>() += s;
        return *this;
    }
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'operator+=(std::string)' for {}", type));
        break;
    }
}

bool var::operator==(const var& other) const
{
    if(other.type == var::var_type::null_type) {
        return type == var::var_type::null_type;
    }

    switch(type)
    {
        case var_type::null_type:
            return other.type == var_type::null_type;
        break;
        case var_type::string:
            return other.type == var_type::string && typed_compare<var::string>(other); 
        break;
        case var_type::integer:
            if(other.type == var_type::integer) {
                return typed_compare<var::integer>(other);
            } 
            else if(other.type == var_type::real)
            {
                return (real_type)as<var::integer>() == other.as<var::real>();
            }
            return false;
        break;
        case var_type::real:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
        case var_type::array:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
        case var_type::map:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
        case var_type::dictionary:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    return false;
}

bool var::operator==(const long& l) const
{
    return as<var::integer>() == l;
}

bool var::operator==(const double& d) const
{
    return d == as<var::real>();
}

bool var::operator==(const std::string& s) const
{
    return as<var::string>() == s;
}

bool var::operator==(const bool& b) const
{
    return b == (bool)as<var::integer>();
}

bool var::operator==(const int& other) const
{
    switch (type)
    {
        case var_type::null_type:
        case var_type::string:
        case var_type::array:
        case var_type::map:
        case var_type::dictionary:
            return false;
        break;
        case var_type::integer:
            return as<var::integer>() == other;
        break;
        case var_type::real:
            return ((int)as<var::real>()) == other;
        break;
    default:
        break;
    }

    VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
}

bool uva::core::var::operator==(const array_type &other) const
{
    switch (type)
    {
        case var_type::array:{
            array_type self = as<var::array>();

            if(self.size() != other.size()) {
                return false;
            }

            for(size_t i = 0; i < other.size(); ++i) {
                if(self[i] != other[i]) {
                    return false;
                }
            }

            return true;
        }
        break;
        default:
            return false;
        break;
    }
}

bool var::operator!=(const var& v) const
{
    return !(*this == v);
}

bool var::operator!=(const double& d) const
{
    return as<var::real>() != d;
}

bool uva::core::var::operator!=(const char *s) const
{
    switch (type)
    {
    case var_type::null_type:
        return true;
        break;
    case var_type::string:
        return as<var::string>() != s;
        break;
    default:
        VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

bool uva::core::var::operator!=(const char8_t *s) const
{
    return as<var::string>() != (const char *)s;
}

bool var::operator!=(const std::string& s) const
{
    return as<var::string>() != s;
}

bool var::operator!=(const var_type& __type) const
{
    return type != __type;
}

bool var::operator<(const int& i) const
{
    return as<var::integer>() < i;
}

bool var::operator<(const time_t& i) const
{
    return as<var::integer>() < i;
}

bool var::operator<(const double& d) const
{
    return as<var::real>() < d;
}

bool var::operator<(const var& other) const
{
    if(type != other.type) {
        return false;
    }

    switch(type)
    {
        case var_type::string:
            return as<var::string>() < other.as<var::string>();
        case var_type::real:
            return as<var::real>() < other.as<var::real>();
        case var_type::integer:
            return as<var::integer>() < other.as<var::integer>();
    }

    throw std::runtime_error(std::format("cannot compare (var_type){} to (var_type){}", (size_t)type, (size_t)other.type));
    return false;
}

var uva::core::var::operator/(const var &other) const
{
    switch (type)
    {
        case var_type::integer:
            switch (other.type)
            {
                case var_type::integer:
                    return as<var::integer>() / other.as<var::integer>();
                break;
                case var_type::real:
                    return as<var::integer>() / other.as<var::real>();
                break;
                default:
                    VAR_THROW_CANT_CAST_TO_THIS_TYPE(other.type);
                break;
            }
        break;
        case var_type::real:
            switch (other.type)
            {
                case var_type::integer:
                    return as<var::real>() / other.as<var::integer>();
                break;
                case var_type::real:
                    return as<var::real>() / other.as<var::real>();
                break;
                default:
                    VAR_THROW_CANT_CAST_TO_THIS_TYPE(other.type);
                break;
            }
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

const var& var::operator[](const size_t& i) const
{
    auto& array = as<var::array>();
    return array[i];
}

var& var::operator[](const size_t& i)
{
    switch(type)
    {
        case var_type::map:{
            return as<var::map>()[i];
        }
        case var_type::array:{
            return as<var::array>()[i];
        }
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

const var& var::operator[](const int& __n) const
{
    switch(type)
    {
        case var_type::map:{
            const map_type& map = as<var::map>();

            auto it = map.find(__n);
            if(it == map.end()) {
                throw "trying to access var by non-existent key in const map.";
            }

            return it->second;
        }
        case var_type::array:{
            return as<var::array>()[__n];
        }
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

var& var::operator[](const int& __n)
{
    switch(type)
    {
        case var_type::map:{
            return as<var::map>()[__n];
        }
        case var_type::array:{
            return as<var::array>()[__n];
        }
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

const var &var::operator[](const var &__k) const
{
    switch(type)
    {
        case var_type::map:{
            const map_type& map = as<var::map>();

            auto it = map.find(__k);
            if(it == map.end()) {
                throw "trying to access var by non-existent key in const map.";
            }

            return it->second;
        }
        case var_type::dictionary:{
            if(!__k.is_a<var_type::string>()) {
                throw "trying to read dictionary by non string key.";
            }

            const dictionary_type& dic = as<var::dictionary>();

            auto it = dic.find(__k.as<var::string>());

            if(it == dic.end()) {
                throw "trying to access var by non-existent key in const dictionary.";
            }

            return it->second;
        }
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

var& var::operator[](const var& __k)
{
    switch(type)
    {
        case var_type::map:
            return as<var::map>()[__k];
        break;
        case var_type::dictionary:{
            if(!__k.is_a<var_type::string>()) {
                throw "trying to read dictionary by non string key.";
            }

            return as<var::dictionary>()[__k.as<var::string>()];
        }
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

const var& var::operator[](const char* __k) const
{
    switch(type)
    {
        case var_type::map:{
            const map_type& map = as<var::map>();

            auto it = map.find(__k);
            if(it == map.end()) {
                throw "trying to access var by non-existent key in const map.";
            }

            return it->second;
        }
        case var_type::dictionary:{
            const dictionary_type& dic = as<var::dictionary>();

            auto it = dic.find(__k);

            if(it == dic.end()) {
                throw "trying to access var by non-existent key in const dictionary.";
            }

            return it->second;
        }
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

const var &uva::core::var::operator[](const char8_t *__k) const
{
    return (*this)[(const char*)__k];
}

var& var::operator[](const char* __k)
{
    switch(type)
    {
        case var_type::map:
            return as<var::map>()[var(__k)];
        break;
        case var_type::dictionary:{
            return as<var::dictionary>()[__k];
        }
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

var &uva::core::var::operator[](const char8_t *__k)
{
    return (*this)[(const char*)__k];
}

var &uva::core::var::operator[](const std::string &__k)
{
    switch(type)
    {
        case var_type::map:
            return as<var::map>()[var(__k)];
        break;
        case var_type::dictionary:{
            return as<var::dictionary>()[__k];
        }
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

var uva::core::var::key(const var &__v)
{
    switch(type)
    {
        case var_type::map: {
            for(const auto & pair : as<var::map>())
            {
                if(pair.second == __v) {
                    return pair.first;
                }
            }
        }
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    return null;
}

void uva::core::var::append_to(std::string &__str, bool typed) const
{
    switch (type)
    {
    case var::var_type::string:
        __str.push_back('\'');
        __str.append(as<var::string>());
        __str.push_back('\'');
    break;
    default:
        if(typed) {
            __str.append(to_typed_s());
        } else {
            __str.append(to_s());
        }
        break;
    }
}

std::vector<var> select_array(bool (*selector)(const var&), std::vector<var>& array)
{
    std::vector<var> new_array;
    new_array.reserve(array.size());

    for(size_t i = 0; i < array.size(); ++i)
    {
        if(selector(array[i])) {
            new_array.push_back(array[i]);
        }
    }

    return new_array;
}

var uva::core::var::select(bool (*selector)(const var&))
{
    switch (type)
    {
    case var::var_type::array:
        return select_array(selector, as<var::array>());
    break;
    default:
        VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

var::array_const_iterator var::begin() const
{
    switch (type)
    {
    case var::var_type::array:
            return as<var::array>().begin();
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'push' for (var_type){}", (size_t)type));
        break;
    }
}

var::array_iterator var::begin()
{
    return as<var::array>().begin();
}

var::array_const_iterator var::end() const
{
    return as<var::array>().end();
}

var::array_iterator var::end()
{
    return as<var::array>().end();
}

void var::push_back(const var& v)
{
    switch (type)
    {
    case var::var_type::array:
        as<var::array>().push_back(v);
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'push' for (var_type){}", (size_t)type));
        break;
    }
}

void var::push_back(var&& v)
{
    switch (type)
    {
    case var::var_type::array:
        as<var::array>().push_back(std::forward<var&&>(v));
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'push' for (var_type){}", (size_t)type));
        break;
    }
}

var::array_iterator var::insert(var::array_const_iterator __position, var&& __x)
{
    return as<var::array>().insert(__position, std::move(__x));
}

var::array_iterator var::insert(var::array_iterator __position, var&& __x)
{
    return as<var::array>().insert(__position, std::move(__x));
}

void var::clear()
{
    switch(type)
    {
        case var_type::string:
            as<var::string>().clear();
        break;
        case var_type::array:
            as<var::array>().clear();
        break;
        case var_type::map:
            as<var::map>().clear();
        break;
        default:
            throw std::runtime_error(std::format("undefined method 'clear' for {}", type));
        break;
    } 
}

void var::reserve(size_t __n)
{
    switch(type)
    {
        case var_type::string:
            as<var::string>().reserve(__n);
        break;
        case var_type::array:
            as<var::array>().reserve(__n);
        break;
        default:
            throw std::runtime_error(std::format("undefined method 'reserve' for {}", type));
        break;
    } 
}

bool var::binary_search(const var& other) const
{
    switch(type)
    {
        case var_type::array:
            return std::binary_search(as<var::array>().begin(), as<var::array>().end(), other);
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

var::array_iterator var::insert_sorted(const var& item, bool distinct)
{
    switch(type)
    {
        case var_type::array: {
            auto it = upper_bound(item);

            if(!distinct || it == end() || *it != item)
            {
                return as<var::array>().insert
                ( 
                    it,
                    item 
                );
            }

            return as<var::array>().end();
        }
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

bool var::empty() const
{
    switch(type)
    {
        case var_type::string:
            return as<var::string>().empty();
        break;
        case var_type::array:
            return as<var::array>().empty();
        break;
        case var_type::map:
            return as<var::map>().empty();
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    } 

}

var &uva::core::var::back()
{
    switch(type)
    {
        case var_type::array:
            return as<var::array>().back();
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    } 
}

var::array_const_iterator var::lower_bound(const var& value) const
{
    switch (type)
    {
    case var::var_type::array:
        return std::lower_bound(begin(), end(), value, [](const var& left, const var& right){
            return left < right;
        });
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'lower_bound' for const (var_type){}", (size_t)type));
        break;
    }
}

var::array_iterator var::lower_bound(const var& value)
{
    switch (type)
    {
    case var::var_type::array:
        return std::lower_bound(begin(), end(), value, [](const var& left, const var& right){
            return left < right;
        });
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'lower_bound' for (var_type){}", (size_t)type));
        break;
    }
}

var::array_iterator var::upper_bound(const uva::core::var& __val)
{
    switch (type)
    {
    case var::var_type::array:
        return std::upper_bound(begin(), end(), __val);
        break;
    default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

void var::each_array(std::function<void(const var& value)> __f) const
{
    for(const auto& v : as<var::array>())
    {
        __f(v);
    }
}

void var::each_array(std::function<void(var& value)> __f)
{
    for(auto& v : as<var::array>())
    {
        __f(v);
    }
}

void var::each_string(std::function<void(const var& value)> __f) const
{
    for(const auto& c : as<var::string>())
    {
        __f(c);
    }
}

void var::each_string(std::function<void(var& value)> __f)
{
    throw std::logic_error("not implemented");
    for(auto& c : as<var::string>())
    {
        //__f(c);
    }
}

void var::each(std::function<void(const var& value)> __f) const
{
    switch(type)
    {
        case var_type::string:
            each_string(__f);
        break;
        case var_type::array:
            each_array(__f);
        break;
        default:
            throw std::runtime_error(std::format("undefined method 'each' for (var_type){}", (size_t)type));
        break;
    }
}

void var::each(std::function<void(var& value)> __f)
{
    switch(type)
    {
        case var_type::string:
            each_string(__f);
        break;
        case var_type::array:
            each_array(__f);
        break;
        default:
            throw std::runtime_error(std::format("undefined method 'each' for (var_type){}", (size_t)type));
        break;
    }
}

void var::each(void (*__f)(const char&)) const
{
    switch(type)
    {
        case var_type::string:{
            const std::string& string = as<var::string>();

            for(size_t i = 0; i < string.size(); ++i) {
                __f(string[i]);
            }
        }
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

bool uva::core::var::includes(const var & value) const
{
    switch(type)
    {
        case var_type::array:{
            const array_type& array = as<var::array>();

            for(size_t i = 0; i < array.size(); ++i) {
                if(array[i] == value) {
                    return true;
                }
            }

            return false;
        }
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    return false;
}

var var::join_array(const char& __separator) const
{ 
    return uva::string::join(as<var::array>(), __separator);
}
var var::join_map(const char& __separator) const
{
    //return uva::string::join(map, __separator);
    return "";
}

var var::join(const char& __separator) const
{
    switch(type)
    {
        case var_type::array:
            return join_array(__separator);
        break;
        case var_type::map:
            return join_map(__separator);
        break;
        default:
            throw std::runtime_error(std::format("undefined method 'join' for {}", type));
        break;
    }
}

var var::fetch(const var& __value, const var& __default) const
{
    switch(type)
    {
        case var_type::map:
        {
            auto it = as<var::map>().find(__value);
            if(it != as<var::map>().end())
            {
                return it->second;
            }

            return __default;
            break;
        }
        case var_type::array:
        {
            auto& array = as<var::array>();
            auto it = std::find(array.begin(), array.end(), __value);
            if(it != array.end())
            {
                return *it;
            }

            return __default;
            break;
        }
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

size_t var::size() const
{
    switch(type)
    {
        case var_type::string:
            return as<var::string>().size();
        break;
        case var_type::real:
            return sizeof(double);
        break;
        case var_type::integer:
            return sizeof(int64_t);
        case var_type::array:
            return as<var::array>().size();
        break;
        case var_type::map:
            return as<var::map>().size();
        break;
        case var_type::dictionary:
            return as<var::map>().size();
        break;
        default:
            throw std::runtime_error(std::format("undefined method 'size' for {}", type));
        break;
    }
}

var var::strftime(std::string_view __format)
{
    switch(type)
    {
        case var_type::integer: {
            time_t t = as<var::integer>();

            std::tm* tm = gmtime(&t);
            if(!std::strftime(s_buffer, 100, __format.data(), tm)) {
                throw std::runtime_error("strftime: error");
            }

            return (std::string)s_buffer;
            break;
        }
        default:
            throw std::runtime_error(std::format("undefined method 'strftime' for {}", type));
        break;
    }
}

var var::capitalize()
{
    if(type != var_type::string)
    {
        throw std::runtime_error(std::format("undefined method 'capitalize' for {}", type));
    }

    return uva::string::capitalize(as<var::string>());
}

var var::to_downcase() const
{
    if(type != var_type::string)
    {
        VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
    }

    return uva::string::tolower(as<var::string>());
}

var var::pluralize()
{
    if(type != var_type::string)
    {
        throw std::runtime_error(std::format("undefined method 'pluralize' for {}", type));
    }

    return uva::string::pluralize(as<var::string>());
}

bool uva::core::var::starts_with(std::string_view sv) const
{
    switch(type)
    {
        case var_type::string:
            return as<var::string>().starts_with(sv);
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

bool uva::core::var::ends_with(std::string_view sv) const
{
    switch(type)
    {
        case var_type::string:
            return as<var::string>().ends_with(sv);
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

bool uva::core::var::ends_with(const var& sufix) const
{
    switch(type)
    {
        case var_type::string:
            switch(sufix.type)
            {
                case var_type::array: {
                    const array_type& array = sufix.as<var::array>();

                    for(size_t i = 0; i < array.size(); ++i) {
                        if(ends_with(array[i])) {
                            return true;
                        }
                    }

                    return false;
                }
                break;
                case var_type::string:
                    return as<var::string>().ends_with(sufix.as<var::string>());
                break;
            }
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

//END VAR

//CORE
var uva::core::now()
{
    return time(nullptr);
}

var find_best_type(std::string str)
{
    if(str.starts_with('\'') || str.starts_with('"')) {
        if(str.ends_with('\'') || str.ends_with('"')) {
            return str.substr(1, str.size()-2);
        } else {
            throw std::runtime_error("error: missing terminating quoetes");
        }
    } else {
        bool is_negative = false;
        bool is_double = false;

        if(str.starts_with('-')) {
            is_negative = true;
            str = str.substr(1);
        }

        for(size_t i = 0; i < str.size(); ++i)
        {
            if(str[i] == '.') {
                if(is_double) {
                    throw std::runtime_error("error: unexpected '.'");
                } else {
                    is_double = true;
                }
            }

            if(!isdigit(str[i])) {
                throw std::runtime_error(std::format("error: unexpected '{}'", str[i]));
            }
        }

        if(is_double) {
            double d = std::stod(str);

            return is_negative ? (d * -1) : d;
        } else {
            size_t i = std::stol(str);

            return is_negative ? (i * -1) : i;
        }
    }
}

var uva::core::parse_argument_list(const std::string &argument_list)
{
    std::string_view sv = argument_list;
    var arguments = var::array();

    std::string extracted_var;
    char is_inside_quotes = '\0';

    while(sv.size()) {
        if(!is_inside_quotes && extracted_var.empty())
        {
            if(!isspace(sv[0])) {
                if(sv.starts_with('\'') || sv.starts_with('"')) {
                    is_inside_quotes = sv[0];
                } else if(isalpha(sv[0]))
                {
                    throw std::runtime_error("error: unexpected alphabet");
                } else if(sv[0] == '-' || isdigit(sv[0]))
                {
                    extracted_var.push_back(sv[0]);
                }
            }
        } else if(is_inside_quotes){
            if(sv.starts_with(is_inside_quotes)) {
                is_inside_quotes = 0;
                arguments.push_back(extracted_var);
                extracted_var.clear();
            } else {
                extracted_var.push_back(sv[0]);
            }
        } else
        {
            if(sv.starts_with(',')) {
                arguments.push_back(find_best_type(extracted_var));
                extracted_var.clear();
            } else {
                extracted_var.push_back(sv[0]);
            }
        }
        
        sv.remove_prefix(1);
    }
    
    if(extracted_var.size()) {
        arguments.push_back(find_best_type(extracted_var));
    }
    
    return arguments;
}
// END CORE
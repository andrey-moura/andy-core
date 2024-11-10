#include <uva/var.hpp>
#include <uva/string.hpp>

//VAR

//REAL CONSTRUCTORS

var::var(const double& d)
{
    set_debug_pointers();
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
    set_debug_pointers();
    new(m_value) string_type(__str);
    type = var_type::string;
}

#ifdef __UVA_CPP20__
uva::core::var::var(const char8_t *__str)
{
    set_debug_pointers();
    new(m_value_ptr) string_type((const char*)__str);
    type = var_type::string;
}
#endif

#ifdef __UVA_CPP17__
uva::core::var::var(std::string_view __str)
{
    set_debug_pointers();
    new(m_value) string_type(__str);
    type = var_type::string;
}
#endif

var::var(const char *__str, size_t __len)
{
    set_debug_pointers();
    new(m_value) string_type(__str, __len);
    type = var_type::string;
}

var::var(const std::string& __str)
{
    set_debug_pointers();
    new(m_value) string_type(__str);
    type = var_type::string;
}

uva::core::var::var(string_type && __string)
{
    set_debug_pointers();
    new(m_value) string_type(std::move(__string));
    type = var_type::string;    
}

//ARRAY CONSTRUCTORS

var::var(array_type&& __array)
{
    set_debug_pointers();
    new(m_value) array_type(std::move(__array));
    type = var_type::array;
}

var::var(const array_type& __array)
{
    set_debug_pointers();
    new(m_value) array_type(__array);
    type = var_type::array;
}

uva::core::var::var(const std::vector<int> &__array)
{
    set_debug_pointers();
    new(m_value) array_type();
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
    set_debug_pointers();
    new(m_value) array_type();
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
    set_debug_pointers();
    new(m_value) map_type(std::move(__map));
    type = var_type::map;
}

uva::core::var::var(const map_type & __map)
{
    set_debug_pointers();
    new(m_value) map_type(__map);
    type = var_type::map;
}

//DICIONARY CONSTRUCTORS
uva::core::var::var(dictionary_type &&__dictionary)
{
    set_debug_pointers();
    new(m_value) dictionary_type(std::move(__dictionary));
    type = var_type::dictionary;
}

uva::core::var::var(const dictionary_type &__dictionary)
{
    set_debug_pointers();
    new(m_value) dictionary_type(__dictionary);
    type = var_type::dictionary;
}

//VAR CONSTRUCTORS

var::var(const var& other)
{
    reconstruct(other);
}

var::var(var&& other)
{
    reconstruct(std::move(other));
}

//VAR_TYPE CONSTRUCTORS

var::var(const var_type& __type)
{
    reconstruct(__type);
}

//CONTRUCTORS HELPERS

void var::set_debug_pointers()
{
#if __UVA_DEBUG_LEVEL__ > 0
    m_integer_ptr    = (integer_type*)m_value;
    m_real_ptr       = (real_type*)m_value;
    m_string_ptr     = (string_type*)m_value;
    m_array_ptr      = (array_type*)m_value;
    m_map_ptr        = (map_type*)m_value;
    m_dictionary_ptr = (dictionary_type*)m_value;
#endif
}

void var::reconstruct(const var& other)
{
    destruct();

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
            new(m_value) string_type(other.as<var::string>());
        break;
        case var_type::array:
            new(m_value) array_type(other.as<var::array>());
        break;
        case var_type::map:
            new(m_value) map_type(other.as<var::map>());
        break;
        case var_type::dictionary:
            new(m_value) dictionary_type(other.as<var::dictionary>());
        break;
        case var_type::undefined:
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    type = other.type;
}

void var::reconstruct(var&& other)
{
    destruct();

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
            new(m_value) string_type(std::move(other.as<var::string>()));
        break;
        case var_type::array:
            new(m_value) array_type(std::move(other.as<var::array>()));
        break;
        case var_type::map:
            new(m_value) map_type(std::move(other.as<var::map>()));
        break;
        case var_type::dictionary:
            new(m_value) dictionary_type(std::move(other.as<var::dictionary>()));
        break;
        case var_type::undefined:
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    type = other.type;
}

void var::reconstruct(const var_type& __type)
{
    destruct();

    switch (__type)
    {
    case var_type::integer:
    case var_type::real:
    break;
    case var_type::string:
        new(m_value) string_type();
    break;
    case var_type::array:
        new(m_value) array_type();
    break;
    case var_type::map:
        new(m_value) map_type();
    break;
    case var_type::dictionary:
        new(m_value) dictionary_type();
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
}

var uva::core::var::integer(integer_type &&__integer)
{
    return var(__integer);
}

var uva::core::var::real(real_type&& i)
{
    return var(i);
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

void var::destruct()
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
            as<var::dictionary>().~basic_dictionary();
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

size_t uva::core::var::size() const
{
    switch(type)
    {
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

const var& uva::core::var::operator[](size_t i) const
{
    switch(type)
    {
        case var_type::array:
            return as<var::array>()[i];
        break;
        case var_type::map: {
            // The [] operator for map inserts a new element if the key does not exist.
            // Since we are in const context, we can't insert a new element.
            // So we will raise an exception if the key does not exist.
            
            const map_type& map = as<var::map>();

            auto it = map.find(var(i));

            if(it == map.end()) {
                throw std::runtime_error(std::format("key {} not found in constant map. It cannot be auto inserted.", i));
            }

            return it->second;
        }
        break;
        case var_type::dictionary:
            //return as<var::dictionary>()[var(i)];
        break;
    }

    VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
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

#ifdef __UVA_CPP20__
var &uva::core::var::operator=(const char8_t *c)
{
    return operator=((const char*)c);
}
#endif

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
#ifdef __UVA_CPP20__
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
#endif

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
    reconstruct(other);
    return *this;
}

var& var::operator=(const var_type& __type)
{
    reconstruct(__type);
    return *this;
}

bool var::operator==(const var& other) const
{
    if(other.type != type) {
        return false;
    }

    switch(type)
    {
        case var_type::null_type:
            return true;
        break;
        case var_type::string:
            return as<var::string>() == other.as<var::string>();
        break;
        case var_type::integer:
            return as<var::integer>() == other.as<var::integer>();
        break;
        case var_type::real:
            return as<var::real>() == other.as<var::real>();
        break;
        case var_type::array:
            return as<var::array>() == other.as<var::array>();
        break;
        case var_type::map:
            return as<var::map>() == other.as<var::map>();
        break;
        case var_type::dictionary:
            return as<var::dictionary>() == other.as<var::dictionary>();
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    return false;
}

bool var::operator!=(const var& other) const
{
    return *this != other;
}

bool uva::core::var::operator<(const var &v) const
{
    if(type != v.type) {
        return type < v.type;
    }

    switch(type)
    {
        case var_type::null_type:
            return false;
        break;
        case var_type::string:
            return as<var::string>() < v.as<var::string>();
        break;
        case var_type::integer:
            return as<var::integer>() < v.as<var::integer>();
        break;
        case var_type::real:
            return as<var::real>() < v.as<var::real>();
        break;
        case var_type::array:
            return as<var::array>() < v.as<var::array>();
        break;
        case var_type::map:
            return as<var::map>() < v.as<var::map>();
        break;
        case var_type::dictionary:
            return as<var::dictionary>() < v.as<var::dictionary>();
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}
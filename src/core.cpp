#include <core.hpp>

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

var::var()
{

}

var::var(const var& other)
{
    reconstruct(other);
}

var::var(var&& other)
{
    reconstruct(std::move(other));
}

var::var(const uint64_t& __integer)
{
    construct();
    as<var_type::integer>() = __integer;
    type = var_type::integer;
}

var::var(const int& __integer)
{
    construct();
    as<var_type::integer>() = __integer;
    type = var_type::integer;
}

var::var(const time_t& __integer)
{
    construct();
    as<var_type::integer>() = __integer;
    type = var_type::integer;
}

var::var(const bool& boolean)
{
    construct();
    as<var_type::integer>() = boolean;
    type = var_type::integer;
}

var::var(const double& d)
{
    construct();
    as<var_type::real>() = d;
    type = var_type::real;
}

var::var(const std::string& __str)
{
    construct();
    new(m_value_ptr) string_type(__str);
    type = var_type::string;
}

var::var(const char* __str)
{
    construct();
    new(m_value_ptr) string_type(__str);
    type = var_type::string;
}

var::var(const char* __str, size_t __len)
{
    construct();
    new(m_value_ptr) string_type(__str, __len);
    type = var_type::string;
}

var::var(std::initializer_list<var> l)
{
    construct();
    new(m_value_ptr) array_type(std::move(l));
    type = var_type::array;
}

var::var(const array_type& __array)
{
    construct();
    new(m_value_ptr) array_type(__array);
    type = var_type::array;
}

var::var(array_type&& __array)
{
    construct();
    new(m_value_ptr) array_type(std::move(__array));
    type = var_type::array;
}

var::var(const var_type& __type)
{
    reconstruct(__type);
}

var::var(map_type&& __map)
{
    construct();
    new(m_value_ptr) map_type(std::move(__map));
    type = var_type::map;
}

void* var::__construct()
{
    return new uint8_t[size_for_buffer];
}

void var::construct(void* __ptr)
{
    m_value_ptr = __ptr;

    m_integer_ptr = (integer_type*)m_value_ptr;
    m_real_ptr    = (real_type*)m_value_ptr;
    m_string_ptr  = (string_type*)m_value_ptr;
    m_array_ptr   = (array_type*)m_value_ptr;
    m_map_ptr     = (map_type*)m_value_ptr;
}

void var::construct()
{
    construct(__construct());
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
            as<var_type::integer>() = other.as<var_type::integer>();
        break;
        case var_type::real:
            //does not need constructor
            as<var_type::real>() = other.as<var_type::real>();
        break;
        case var_type::string:
            new(m_value_ptr) string_type(other.as<var_type::string>());
        break;
        case var_type::array:
            new(m_value_ptr) array_type(other.as<var_type::array>());
        break;
        case var_type::map:
            new(m_value_ptr) map_type(other.as<var_type::map>());
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
                as<var_type::string>().~basic_string();
            break;
            case var_type::array:
                as<var_type::array>().~vector();
            break;
            case var_type::map:
                as<var_type::map>().~map();
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
        case var_type::string:
            return as<var_type::string>();
        break;
        case var_type::integer:
            return std::to_string(as<var_type::integer>());
        case var_type::real:
            return std::format("{}", as<var_type::real>());
        break;
    }

    throw std::runtime_error(std::format("failed to convert from (var_type){} to string", (size_t)type));
    return "";
}

int64_t var::to_i() const
{
    switch(type)
    {
        case var_type::string:
        {
            std::string string = as<var_type::string>();
            bool negative = false;

            if(string.starts_with('-')) {
                negative = true;
                string.erase(0);
            }

            for(const char& c : string) {
                if(!isdigit(c)) {
                    throw std::runtime_error(std::format("invalid character '{}' in string \"{}\" while converting to integer", c, as<var_type::real>()));
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
            return (int)as<var_type::real>();
        break;
        case var_type::integer:
            return as<var_type::integer>();
        break;
    }

    throw std::runtime_error(std::format("failed to convert from (var_type){} to integer", (size_t)type));
    return -1;
}

var::operator int() const
{
    return (int)as<var_type::integer>();
}

var::operator uint64_t() const
{
    return (uint64_t)as<var_type::integer>();
}

var::operator int64_t() const
{
    return (int64_t)as<var_type::integer>();
}

var::operator std::string() const
{
    return as<var_type::string>();
}

var::operator bool() const
{
    return (bool)as<var_type::integer>();
}

var::operator double() const
{
    return as<var_type::real>();
}

var::operator std::vector<int>() const
{
    if(type == var_type::array)
    {
        const array_type& array = as<var_type::array>();

        std::vector<int> __arr;
        __arr.reserve(array.size());
        
        for(int i = 0; i < array.size(); ++i)
        {
            __arr.push_back(array[i].as<var_type::integer>());
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
        as<var_type::integer>() = i;
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
        as<var_type::integer>() = i;
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
        as<__type>() = i;
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
        as<__type>() = d;
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
        as<var_type::string>() = str;
    } else {
        reconstruct<string_type>(str);
        type = var::var_type::string;
    }

    return *this;
}

var& var::operator=(const unsigned char* ustr)
{
    const char* str = (const char*)ustr;

    if(type == var_type::string)
    {
        as<var_type::string>() = str;
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
        as<var_type::string>() = str;
    } else {
        reconstruct<string_type>(str);
        type = var::var_type::string;
    }

    return *this;
}

var& var::operator=(array_type&& __array)
{
    constexpr var_type __type = var_type::array;

    if(type == __type)
    {
        as<__type>() = __array;
    } else {
        reconstruct<array_type>(__array);
        type = __type;
    }

    return *this;
}

var& var::operator=(std::initializer_list<var> __array)
{
    constexpr var_type __type = var_type::array;

    if(type == __type)
    {
        as<__type>() = __array;
    } else {
        reconstruct<array_type>(std::move(__array));
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

var var::operator+(const std::string& s) const
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

var& var::operator+=(const std::string& s)
{
    switch (type)
    {
    case var::var_type::string:{
        as<var_type::string>() += s;
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
    switch(type)
    {
        case var_type::null_type:
            return other.type == var_type::null_type;
        break;
        case var_type::string:
            return other.type == var_type::string && typed_compare<var_type::string>(other); 
        break;
        case var_type::integer:
            if(other.type == var_type::integer) {
                return typed_compare<var_type::integer>(other);
            } 
            else if(other.type == var_type::real)
            {
                return (real_type)as<var_type::integer>() == other.as<var_type::real>();
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
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    return false;
}

bool var::operator==(const long& l) const
{
    return as<var_type::integer>() == l;
}

bool var::operator==(const double& d) const
{
    return d == as<var_type::real>();
}

bool var::operator==(const std::string& s) const
{
    return as<var_type::string>() == s;
}

bool var::operator==(const bool& b) const
{
    return as<var_type::integer>() == (bool)as<var_type::integer>();
}

bool var::operator==(const int& other) const
{
    switch (type)
    {
        case var_type::null_type:
        case var_type::string:
        case var_type::array:
        case var_type::map:    
            return false;
        break;
        case var_type::integer:
            return as<var_type::integer>() == other;
        break;
        case var_type::real:
            return ((int)as<var_type::real>()) == other;
        break;
    default:
        break;
    }

    VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
}

bool var::operator!=(const double& d) const
{
    return as<var_type::real>() != d;
}

bool var::operator!=(const std::string& s) const
{
    return as<var_type::string>() != s;
}

bool var::operator!=(const var_type& __type) const
{
    return type != __type;
}

bool var::operator<(const int& i) const
{
    return as<var_type::integer>() < i;
}

bool var::operator<(const time_t& i) const
{
    return as<var_type::integer>() < i;
}

bool var::operator<(const double& d) const
{
    return as<var_type::real>() < d;
}

bool var::operator<(const var& other) const
{
    if(type != other.type) {
        return false;
    }

    switch(type)
    {
        case var_type::string:
            return as<var_type::string>() < other.as<var_type::string>();
        case var_type::real:
            return as<var_type::real>() < other.as<var_type::real>();
        case var_type::integer:
            return as<var_type::integer>() < other.as<var_type::integer>();
    }

    throw std::runtime_error(std::format("cannot compare (var_type){} to (var_type){}", (size_t)type, (size_t)other.type));
    return -1;
}

const var& var::operator[](const size_t& i) const
{
    return as<var_type::array>()[i];
}

var& var::operator[](const size_t& i)
{
    return as<var_type::array>()[i];
}

const var& var::operator[](const int& __n) const
{
    return as<var_type::array>()[(size_t)__n];
}

var& var::operator[](const int& __n)
{
    return as<var_type::array>()[(size_t)__n];
}


var::array_const_iterator var::begin() const
{
    switch (type)
    {
    case var::var_type::array:
            return as<var_type::array>().begin();
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'push' for (var_type){}", (size_t)type));
        break;
    }
}

var::array_iterator var::begin()
{
    return as<var_type::array>().begin();
}

var::array_const_iterator var::end() const
{
    return as<var_type::array>().end();
}

var::array_iterator var::end()
{
    return as<var_type::array>().end();
}

void var::push_back(const var& v)
{
    switch (type)
    {
    case var::var_type::array:
        as<var_type::array>().push_back(v);
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
        as<var_type::array>().push_back(std::forward<var&&>(v));
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'push' for (var_type){}", (size_t)type));
        break;
    }
}

var::array_iterator var::insert(var::array_const_iterator __position, var&& __x)
{
    return as<var_type::array>().insert(__position, std::move(__x));
}

var::array_iterator var::insert(var::array_iterator __position, var&& __x)
{
    return as<var_type::array>().insert(__position, std::move(__x));
}

void var::clear()
{
    switch(type)
    {
        case var_type::string:
            as<var_type::string>().clear();
        break;
        case var_type::array:
            as<var_type::array>().clear();
        break;
        case var_type::map:
            as<var_type::map>().clear();
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
            as<var_type::string>().reserve(__n);
        break;
        case var_type::array:
            as<var_type::array>().reserve(__n);
        break;
        default:
            throw std::runtime_error(std::format("undefined method 'reserve' for {}", type));
        break;
    } 
}

bool var::empty() const
{
    switch(type)
    {
        case var_type::string:
            return as<var_type::string>().empty();
        break;
        case var_type::array:
            return as<var_type::array>().empty();
        break;
        default:
            throw std::runtime_error(std::format("undefined method 'empty' for (var_type){}", (size_t)type));
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

void var::each_array(std::function<void(const var& value)> __f) const
{
    for(const auto& v : as<var_type::array>())
    {
        __f(v);
    }
}

void var::each_array(std::function<void(var& value)> __f)
{
    for(auto& v : as<var_type::array>())
    {
        __f(v);
    }
}

void var::each_string(std::function<void(const var& value)> __f) const
{
    for(const auto& c : as<var_type::string>())
    {
        __f(c);
    }
}

void var::each_string(std::function<void(var& value)> __f)
{
    throw std::logic_error("not implemented");
    for(auto& c : as<var_type::string>())
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
            const std::string& string = as<var_type::string>();

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

var var::join_array(const char& __separator) const
{ 
    return uva::string::join(as<var_type::array>(), __separator);
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

size_t var::size() const
{
    switch(type)
    {
        case var_type::string:
            return as<var_type::string>().size();
        break;
        case var_type::real:
            return sizeof(double);
        break;
        case var_type::integer:
            return sizeof(int64_t);
        case var_type::array:
            return as<var_type::array>().size();
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
            time_t t = as<var_type::integer>();

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

    return uva::string::capitalize(as<var_type::string>());
}

var var::downcase()
{
    if(type != var_type::string)
    {
        throw std::runtime_error(std::format("undefined method 'downcase' for {}", type));
    }

    return uva::string::tolower(as<var_type::string>());
}

var var::pluralize()
{
    if(type != var_type::string)
    {
        throw std::runtime_error(std::format("undefined method 'pluralize' for {}", type));
    }

    return uva::string::pluralize(as<var_type::string>());
}

const var& var::operator[](const var& __k) const
{
    switch(type)
    {
        case var_type::map:{
            const map_type& map = as<var_type::map>();

            auto it = map.find(__k);
            if(it == map.end()) {
                throw "trying to access var by non-existent key in const map.";
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
            return as<var_type::map>()[__k];
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }
}

const var& var::operator[](const char* __k) const
{
    return self[var(__k)];
}

var& var::operator[](const char* __k)
{
    return self[var(__k)];
}

//END VAR

//CORE
var uva::core::now()
{
    return time(nullptr);
}
//END CORE
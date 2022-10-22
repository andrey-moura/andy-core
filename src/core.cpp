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
    : type(var_type::null_type)
{

}

var::var(var&& other)
{
    construct(std::forward<var&&>(other));
}

var::var(const uint64_t& _integer)
    : integer(_integer), type(var::var_type::integer)
{

}

var::var(const int& _integer)
    : integer(_integer), type(var::var_type::integer)
{

}

var::var(const time_t& _integer)
    : integer(_integer), type(var::var_type::integer)
{

}


var::var(const std::string& _str)
    : str(_str), type(var::var_type::string)
{

}

var::var(const char* _str)
    : str(_str), type(var::var_type::string)
{

}

var::var(const char* str, size_t i)
    : str(str, i), type(var::var_type::string)
{

}

var::var(const bool& boolean)
    : integer(boolean), type(var::var_type::integer)
{

}

var::var(const double& d)
    : real(d), type(var::var_type::real)
{
    
}

var::var(std::initializer_list<var> l)
    : array(std::move(l)), type(var::var_type::array)
{
    
}

var::var(const array_type& __array)
    : array(__array), type(var::var_type::array)
{

}

var::var(array_type&& __array)
    : array(std::forward<array_type&&>(__array)), type(var::var_type::array)
{

}

var::var(const var_type& __type)
{
    construct(__type);
}

void var::construct(const var_type& __type)
{
    if(__type == var_type::map || __type == var_type::array || __type == var_type::null_type)
    {
        type = __type;
    }
    else
    {
        throw std::runtime_error(std::format("failed to create a var from var_type::{}", __type));
    }
}

void var::construct(var&& __var)
{
    switch (__var.type)
    {
        case var_type::null_type:
            type = null;
        break;
        case var_type::string:
            str = std::move(__var.str);
        break;
        case var_type::integer:
            integer = std::move(__var.integer);
        break;
        case var_type::real:
            real = std::move(__var.real);
        break;
        case var_type::array:
            array = std::move(__var.array);
        break;
        case var_type::map:
            map = std::move(__var.map);
        break;
    default:
            throw std::runtime_error(std::format("failed to construct a var from a var with typee {}", __var.type));
        break;
    }

    type = __var.type;
    __var = null;
}

void var::construct(array_type&& __array)
{
    self.array = std::forward<array_type&&>(__array);
    (*this).type = var_type::array;
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
            return str;
        break;
        case var_type::integer:
            return std::to_string(integer);
        case var_type::real:
            return std::format("{}", real);
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
            std::string string = str;
            bool negative = false;

            if(string.starts_with('-')) {
                negative = true;
                string.erase(0);
            }

            for(const char& c : string) {
                if(!isdigit(c)) {
                    throw std::runtime_error(std::format("invalid character '{}' in string \"{}\" while converting to integer", c, str));
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
            return (int)real;
        break;
        case var_type::integer:
            return integer;
        break;
    }

    throw std::runtime_error(std::format("failed to convert from (var_type){} to integer", (size_t)type));
    return -1;
}

var::operator int() const
{
    return (int)integer;
}

var::operator uint64_t() const
{
    return (uint64_t)integer;
}

var::operator int64_t() const
{
    return (int64_t)integer;
}

var::operator std::string() const
{
    return str;
}

var::operator bool() const
{
    return (bool)integer;
}

var::operator double() const
{
    return real;
}

var& var::operator=(const char* c)
{
    str = c;
    type = var::var_type::string;
    return *this;
}

var& var::operator=(const unsigned char* c)
{
    str = (char*)c;
    type = var::var_type::string;
    return *this;
}

var& var::operator=(const uint64_t& i)
{
    integer = (int64_t)i;
    type = var::var_type::integer;
    return *this;
}

var& var::operator=(const int64_t& i)
{
    integer = i;
    type = var::var_type::integer;
    return *this;
}

var& var::operator=(const std::string& s)
{
    str = s;
    type = var::var_type::string;
    return *this;
}

var& var::operator=(array_type&& __array)
{
    construct(std::forward<array_type&&>(__array));
    return *this;
}

var& var::operator=(const bool& b)
{
    integer = (int64_t)b;
    type = var::var_type::integer;
    return *this;
}

var& var::operator=(const double& d)
{
    real = d;
    type = var::var_type::real;
    return *this;
}

var& var::operator=(const var& other)
{
    switch(other.type) {
        case var_type::integer:
            integer = other.integer;
            break;
        case var_type::string:
            str = other.str;
            break;
        default:
            throw std::runtime_error(std::format("undefined method 'operator=(const var&)' for {}", type));
        break;
    }
    type = other.type;
    return *this;
}

var& var::operator=(var&& other)
{
    construct(std::forward<var&&>(other));
    return *this;
}

var& var::operator=(std::initializer_list<var> __array)
{
    array = std::move(__array);
    type = var_type::array;
    return *this;
}

var& var::operator=(const var_type& __type)
{
    construct(__type);
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
        str += s;
        return *this;
    }
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'operator+=(std::string)' for {}", type));
        break;
    }
}

bool var::operator==(const long& l) const
{
    return integer == l;
}

bool var::operator==(const double& d) const
{
    return d == real;
}

bool var::operator==(const std::string& s) const
{
    return str == s;
}

bool var::operator==(const bool& b) const
{
    return b == (bool)integer;
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
            return integer == other;
        break;
        case var_type::real:
            return ((int)real) == other;
        break;
    default:
        break;
    }

    VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
}

bool var::operator!=(const double& d) const
{
    return real != d;
}

bool var::operator!=(const std::string& s) const
{
    return str != s;
}

bool var::operator<(const int& i) const
{
    return integer < i;
}

bool var::operator<(const time_t& i) const
{
    return integer < i;
}

bool var::operator<(const double& d) const
{
    return real < d;
}

bool var::operator<(const var& other) const
{
    if(type != other.type) {
        return false;
    }

    switch(type)
    {
        case var_type::string:
            return str < other.str;
        case var_type::real:
            return real < other.real;
        case var_type::integer:
            return integer < other.integer;
    }

    throw std::runtime_error(std::format("cannot compare (var_type){} to (var_type){}", (size_t)type, (size_t)other.type));
    return -1;
}

const var& var::operator[](const size_t& i) const
{
    return array[i];
}

var& var::operator[](const size_t& i)
{
    return array[i];
}

var::array_const_iterator var::begin() const
{
    switch (type)
    {
    case var::var_type::array:
            return array.begin();
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'push' for (var_type){}", (size_t)type));
        break;
    }
}

var::array_iterator var::begin()
{
    return array.begin();
}

var::array_const_iterator var::end() const
{
    return array.end();
}

var::array_iterator var::end()
{
    return array.end();
}

void var::push_back(const var& v)
{
    switch (type)
    {
    case var::var_type::array:
        array.push_back(v);
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
        array.push_back(std::move(v));
        break;
    default:
        throw std::runtime_error(std::format("undefined method 'push' for (var_type){}", (size_t)type));
        break;
    }
}

var::array_iterator var::insert(var::array_const_iterator __position, var&& __x)
{
    return array.insert(__position, std::move(__x));
}

var::array_iterator var::insert(var::array_iterator __position, var&& __x)
{
    return array.insert(__position, std::move(__x));
}

void var::clear()
{
    switch(type)
    {
        case var_type::string:
            str.clear();
        break;
        case var_type::array:
            array.clear();
        break;
        case var_type::map:
            map.clear();
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
            str.reserve(__n);
        break;
        case var_type::array:
            array.reserve(__n);
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
            return str.empty();
        break;
        case var_type::array:
            return array.empty();
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
    for(const auto& v : array)
    {
        __f(v);
    }
}

void var::each_array(std::function<void(var& value)> __f)
{
    for(auto& v : array)
    {
        __f(v);
    }
}

void var::each_string(std::function<void(const var& value)> __f) const
{
    for(const auto& c : str)
    {
        __f(c);
    }
}

void var::each_string(std::function<void(var& value)> __f)
{
    throw std::logic_error("not implemented");
    for(auto& c : str)
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

var var::join_array(const char& __separator) const
{ 
    return uva::string::join(array, __separator);
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
            return str.size();
        break;
        case var_type::real:
            return sizeof(real);
        break;
        case var_type::integer:
            return sizeof(integer);
        case var_type::array:
            return array.size();
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
            time_t t = integer;

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

    return uva::string::capitalize(str);
}

var var::downcase()
{
    if(type != var_type::string)
    {
        throw std::runtime_error(std::format("undefined method 'downcase' for {}", type));
    }

    return uva::string::tolower(str);
}

var var::pluralize()
{
    if(type != var_type::string)
    {
        throw std::runtime_error(std::format("undefined method 'pluralize' for {}", type));
    }

    return uva::string::pluralize(str);
}

//END VAR

//CORE
var uva::core::now()
{
    return time(nullptr);
}
//END CORE
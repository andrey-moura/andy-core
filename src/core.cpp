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

var::var(const var& other)
{
    switch(other.type)
    {
        case var_type::null_type:
        break;
        case var_type::string:
            as<var_type::string>() = other.as<var_type::string>();
        break;
        case var_type::integer:
            as<var_type::integer>() = other.as<var_type::integer>();
        break;
        case var_type::real:
            as<var_type::real>() = other.as<var_type::real>();
        break;
        case var_type::array:
            as<var_type::array>() = other.as<var_type::array>();
        break;
        case var_type::map:
            as<var_type::map>() = other.as<var_type::map>();
        break;
        default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
            break;
    }

    type = other.type;
}

var::var(var&& other)
{
    construct(std::forward<var&&>(other));
}

var::var(const uint64_t& __integer)
    : type(var::var_type::integer)
{
    m_value_ptr = new int64_t(__integer);
}

var::var(const int& __integer)
    : type(var::var_type::integer)
{
    m_value_ptr = new int64_t(__integer);
}

var::var(const time_t& __integer)
    : type(var::var_type::integer)
{
    m_value_ptr = new int64_t(__integer);
}

var::var(const std::string& __str)
    : type(var::var_type::string)
{
    m_value_ptr = new std::string(__str);
}

var::var(const char* __str)
    : type(var::var_type::string)
{
    m_value_ptr = new std::string(__str);
}

var::var(const char* __str, size_t __len)
    : type(var::var_type::string)
{
    m_value_ptr = new std::string(__str, __len);
}

var::var(const bool& boolean)
    : type(var::var_type::integer)
{
    m_value_ptr = new int64_t(boolean);
}

var::var(const double& d)
    : type(var::var_type::real)
{
    m_value_ptr = new double(d);
}

var::var(std::initializer_list<var> l)
    : type(var::var_type::array)
{
    m_value_ptr = new array_type(std::move(l));
}

var::var(const array_type& __array)
    : type(var::var_type::array)
{
    m_value_ptr = new array_type(__array);
}

var::var(array_type&& __array)
    : type(var::var_type::array)
{
    m_value_ptr = new array_type(__array);
}

var::var(const var_type& __type)
{
    construct(__type);
}

var::~var()
{
    if(m_value_ptr)
    {
        switch (type)
        {
            case var_type::string:
                delete &as<var_type::string>();
            break;
            case var_type::integer:
                delete &as<var_type::integer>();
            break;
            case var_type::real:
                delete &as<var_type::real>();
            break;
            case var_type::array:
                delete &as<var_type::array>();
            break;
            case var_type::map:
                delete &as<var_type::map>();
            break;
        }
    }
}

void var::construct(const var_type& __type)
{
    switch (__type)
    {
    case var_type::map:
            m_value_ptr = new map_type();
        break;
    case var_type::array:
            m_value_ptr = new array_type();
        break;
    case var_type::null_type:
            m_value_ptr = nullptr;
        break;
    default:
            VAR_THROW_UNDEFINED_METHOD_FOR_THIS_TYPE();
        break;
    }

    type = __type;
}

void var::construct(var&& __var)
{
    m_value_ptr = __var.m_value_ptr;
    __var.m_value_ptr = nullptr;
    __var.type = var_type::null_type;

    type = __var.type;
}

void var::construct(array_type&& __array)
{
    as<var_type::array>() = std::forward<array_type&&>(__array);
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

var& var::operator=(const char* c)
{
    as<var_type::string>() = c;
    type = var::var_type::string;
    return *this;
}

var& var::operator=(const unsigned char* c)
{
    as<var_type::string>() = (char*)c;
    type = var::var_type::string;
    return *this;
}

var& var::operator=(const uint64_t& i)
{
    as<var_type::integer>() = (int64_t)i;
    type = var::var_type::integer;
    return *this;
}

var& var::operator=(const int64_t& i)
{
    as<var_type::integer>() = i;
    type = var::var_type::integer;
    return *this;
}

var& var::operator=(const std::string& s)
{
    as<var_type::string>() = s;
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
    as<var_type::integer>() = (int64_t)b;
    type = var::var_type::integer;
    return *this;
}

var& var::operator=(const double& d)
{
    as<var_type::real>() = d;
    type = var::var_type::real;
    return *this;
}

var& var::operator=(const var& other)
{
    switch(other.type) {
        case var_type::integer:
            as<var_type::integer>() = other.as<var_type::integer>();
            break;
        case var_type::string:
            as<var_type::string>() = other.as<var_type::string>();
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
    as<var_type::array>() = std::move(__array);
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
        as<var_type::string>() += s;
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

//END VAR

//CORE
var uva::core::now()
{
    return time(nullptr);
}
//END CORE
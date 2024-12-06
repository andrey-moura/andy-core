#pragma once

#include <uva/size.hpp>
#include <uva/point.hpp>

namespace uva
{
    template<typename T>
    struct basic_rect
    {
        basic_rect() = default;
        basic_rect(T x, T y, T w, T h)
            : x(x), y(y), w(w), h(h)
        {
        }
        basic_rect(const basic_point<T>& position, const basic_size<T>& size)
            : position(position), size(size)
        {
        }
        union {
            struct {
            T x;
            T y;
            T w;
            T h;
            };
            struct {
                uva::basic_point<T> position;
                uva::basic_size<T>  size;
            };
        };
    };

    typedef basic_rect<int> rect;
    typedef basic_rect<float> rectf;
}
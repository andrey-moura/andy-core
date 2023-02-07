#pragma once

#include <stdint.h>

namespace uva
{
    template<typename T>
    struct basic_color
    {
        basic_color(const basic_color& other)
            : r(other.r), g(other.g), b(other.b), a(other.a)
        {

        }

        basic_color(const T& r, const T& g, const T& b, const T& a = 255)
            : r(r), g(g), b(b), a(a)
        {

        }

        T r;
        T g;
        T b;
        T a;
    };

    using color = basic_color<uint8_t>;
};
#pragma once

#include <stdint.h>

namespace uva
{
    struct color24
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };
    template<typename T>
    struct basic_color
    {
        basic_color() = default;

        basic_color(const basic_color& other)
            : r(other.r), g(other.g), b(other.b), a(other.a)
        {

        }

        basic_color(const T& r, const T& g, const T& b, const T& a = 255)
            : r(r), g(g), b(b), a(a)
        {

        }

        union {
            uint32_t rgba;

            struct {
                T r;
                T g;
                T b;
                T a;
            };
        };

        bool operator==(const basic_color<T>& other) const {
            return rgba == other.rgba;
        }
        bool operator!=(const basic_color<T>& other) const {
            return rgba != other.rgba;
        }
        basic_color<T>& operator=(const basic_color<T>& other) {
            rgba = other.rgba;
            return *this;
        }
        bool operator<(const basic_color<T>& c) const
        {
            return rgba < c.rgba;
        }
    };

    using color = basic_color<uint8_t>;
    using color32 = basic_color<uint8_t>;
};
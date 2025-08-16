#pragma once

namespace andy {

    template<typename T>
    struct basic_size
    {
        T w;
        T h;

        basic_size()
            : w((T)0), h((T)0)
        {

        }

        basic_size(const T& __w, const T& __h)
            : w(__w), h(__h)
        {

        }

        T area() const { return w * h; }
        template<typename OtherT>
        basic_size<T>& operator=(const basic_size<OtherT>& other) {
            w = (T)other.w;
            h = (T)other.h;
            return *this;
        }
        basic_size<T>& operator+=(const basic_size<T>& other) {
            w += other.w;
            h += other.h;
            return *this;
        }

        basic_size<T>& operator-=(const basic_size<T>& other) {
            w -= other.w;
            h -= other.h;
            return *this;
        }
        basic_size<T>& operator*=(int x) {
            w *= x;
            h *= x;
            return *this;
        }
        basic_size<T> operator*(int x) {
            return { w*(T)x, h*(T)x };
        }
        basic_size<T>& operator=(const basic_size<T>& other) {
            w = other.w;
            h = other.h;
            return *this;
        }
        bool operator==(const basic_size<T>& other) const {
            return other.w == w && other.h == h;
        }
        bool operator!=(const basic_size<T>& other) const {
            return other.w != w || other.h != h;
        }
        bool operator<(const basic_size<T>& other) const {
            return w < other.w && h < other.h;
        }
        bool operator<=(const basic_size<T>& other) const {
            return w <= other.w && h <= other.h;
        }
    };

    typedef basic_size<int> size;
    typedef basic_size<float> sizef;
}
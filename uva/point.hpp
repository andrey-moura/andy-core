#pragma once

namespace uva {

template<typename T>
    struct basic_point
    {
        T x;
        T y;

        basic_point<T> ()
            : x((T)0), y((T)0) {
                
            }

        basic_point<T>(const T& x, const T& y) 
            : x(x), y(y) {
                
            }
        template<typename OtherT>
        basic_point<T>(const OtherT& x, const OtherT& y)
            : x((T)x), y((T)y) {

        }
        template<typename OtherT>
        basic_point<T>(const basic_point<OtherT> other)
            : x((T)other.x), y((T)other.y) {

        }

        basic_point<T>& operator+=(const basic_point<T>& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        basic_point<T>& operator-=(const basic_point<T>& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        basic_point<T> operator+(const basic_point<T> other) const {
            return { x+other.x, y+other.y };
        }
        basic_point<T> operator-(const basic_point<T> other) const {
            return { x+other.x, y+other.y };
        }
        bool operator==(const basic_point<T>& other) const {
            return x == other.x && y == other.y;
        }
    };
    typedef basic_point<int> point;
    typedef basic_point<float> pointf;
}
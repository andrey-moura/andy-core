#pragma once

namespace andy
{
    template<typename T>
    class basic_vector_3
    {
        public:
            T x;
            T y;
            T z;
        public:
            basic_vector_3(T x, T y, T z)
                : x(x), y(y), z(z)
            {
            }
            static basic_vector_3<T> up()
            {
                return basic_vector_3<T>(0.0f, 1.0f, 0.0f);
            }

            static basic_vector_3<T> right()
            {
                return basic_vector_3<T>(1.0f, 0.0f, 0.0f);
            }

            static basic_vector_3<T> down()
            {
                return basic_vector_3<T>(0.0f, -1.0f, 0.0f);
            }

            static basic_vector_3<T> left()
            {
                return basic_vector_3<T>(-1.0f, 0.0f, 0.0f);
            }
        private:
        public:
            const T& operator[](unsigned int n) const
            {
                return *(((float*)this) + n);
            }
            T& operator[](unsigned int n)
            {
                return *(((float*)this) + n);
            }
        private:
    };

    template<typename T>
    class basic_vector_2
    {
        public:
            T x;
            T y;
        private:
        public:
        private:
    };

    using vector3f = basic_vector_3<float>;
    using vector = vector3f;

    using vector2f = basic_vector_2<float>;
}; // namespace andy
#pragma once

#include <cmath>
#include <iostream>

namespace mathf
{
    inline
    static float mix(const float v0, const float v1, const float value) noexcept
    {
        return v0 * (1.f - value) + (v1 * value);
    }


    struct vec4
    {
    public:
        float x;
        float y;
        float z;
        float w;

        vec4()
        {
            vec4(0.f);
        }

        vec4(const float& value)
        {
            this->x = value;
            this->y = value;
            this->z = value;
            this->w = value;
        }

        vec4(const float& x, const float& y, const float& z, const float& w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        vec4(const vec4& vec)
        {
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;
            this->w = vec.w;
        }

        void print(std::string arg)
        {
            std::cout<<arg<<" vec4: "<<this->x<< " " << this->y<< " " << this->z << " " << this->w <<std::endl;
        }

        void print()
        {
           std::cout<<"vec4: "<<this->x<< " " << this->y<< " " << this->z << " " << this->w<<std::endl;
        }

        vec4& operator=(const vec4& vec)
        {
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;
            this->w = vec.w;

            return *this;
        }

        vec4& operator+=(const vec4& right)
        {
            this->x += right.x;
            this->y += right.y;
            this->z += right.z;
            this->w += right.w;
            
            return *this;
        }

        vec4& operator-=(const vec4& right)
        {
            this->x -= right.x;
            this->y -= right.y;
            this->z -= right.z;
            this->w -= right.w;
            
            return *this;
        }

        vec4& operator*=(const float& value)
        {
            this->x *= value;
            this->y *= value;
            this->z *= value;
            this->w *= value;

            return *this;
        }

        vec4& operator/=(const float& value)
        {
            this->x /= value;
            this->y /= value;
            this->z /= value;
            this->w /= value;

            return *this;
        }

        vec4 operator+(const vec4& right) const
        {
            vec4 res;

            res.x = this->x + right.x;
            res.y = this->y + right.y;
            res.z = this->z + right.z;
            res.w = this->w + right.w;

            return res;
        }

        vec4 operator-(const vec4& right) const
        {
            vec4 res;

            res.x = this->x - right.x;
            res.y = this->y - right.y;
            res.z = this->z - right.z;
            res.w = this->w - right.w;

            return res;
        }

        vec4 operator*(const float& value) const
        {
            vec4 res = *this;

            res *= value;

            return res;
        }

        vec4 operator/(const float& value) const
        {
            vec4 res = *this;

            res /= value;

            return res;
        }

        float operator[](unsigned int i) const
        {
            if (i == 0)
                return x;
            else if (i == 1)
                return y;
            else
                return z;
        }

        bool operator==(const vec4& vec) const
        {
            return (this->x == vec.x && this->y == vec.y && this->z == vec.z && this->w == vec.w);
        }

        bool operator!=(const vec4& vec) const
        {
            return (this->x != vec.x || this->y != vec.y || this->z != vec.z || this->w != vec.w);
        }

        inline
        static float dot(const vec4& a, const vec4& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        inline
        float dot(const vec4& b) const
        {
            return dot(*this, b);
        }

        inline
        static float	length(const vec4 v1)
        {
            return sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z + v1.w * v1.w);
        }

        inline
        float	length() const
        {
            return length(*this);
        }

        inline
        static vec4		normalize(const vec4& v1)
        {
            vec4	res;
            float	vec_length;

            vec_length = length(v1);
            res.x = v1.x / vec_length;
            res.y = v1.y / vec_length;
            res.z = v1.z / vec_length;
            res.w = v1.w / vec_length;
            return (res);
        }

        inline
        vec4		normalize() const
        {
            return normalize(*this);
        }

        inline
        static vec4		normalize_safe(const vec4& v1)
        {
            vec4	res;
            float	vec_length;

            if (v1.x == 0.f && v1.y == 0.f && v1.z == 0.f && v1.w == 0.f)
            {
                return (vec4){0.f, 0.f, 0.f, 1.f};
            }

            vec_length = length(v1);
            res.x = v1.x / vec_length;
            res.y = v1.y / vec_length;
            res.z = v1.z / vec_length;
            res.w = v1.w / vec_length;

            return res;
        }

        inline
        vec4		normalize_safe() const
        {
            return normalize_safe(*this);
        }

        inline
        static vec4 lerp(const vec4& a, const vec4& b, const float& value)
        {
            vec4 res;

            res.x =  mix(a.x, b.x, value);
            res.y =  mix(a.y, b.y, value);
            res.z =  mix(a.z, b.z, value);
            res.w =  mix(a.z, b.z, value);

            return res;
        }

        inline
        vec4 lerp(const vec4& b, const float& value) const
        {
            return lerp(*this, b, value);
        }

        inline
        static float distance(const vec4& from, const vec4& to);
    };

    inline
    float vec4::distance(const vec4& from, const vec4& to)
    {
        return length(to - from);
    }
}

namespace mathf
{
    struct vec3
    {
        float x;
        float y;
        float z;

        vec3()
        {
            vec3(0.f);
        }

        vec3(const float& value)
        {
            this->x = value;
            this->y = value;
            this->z = value;
        }

        vec3(const float& x, const float y, const float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        vec3(const vec3& vec)
        {
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;
        }

        vec3(const mathf::vec4& vec)
        {
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;
        }

        void print(std::string arg)
        {
            std::cout<<arg<<" vec3: "<<this->x<< " " << this->y<< " " << this->z <<std::endl;
        }

        void print()
        {
           std::cout<<"vec3: "<<this->x<< " " << this->y<< " " << this->z <<std::endl;
        }

        vec3& operator=(const vec3& vec)
        {
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;

            return *this;
        }

        vec3& operator+=(const vec3& right)
        {
            this->x += right.x;
            this->y += right.y;
            this->z += right.z;
            
            return *this;
        }

        vec3& operator-=(const vec3& right)
        {
            this->x -= right.x;
            this->y -= right.y;
            this->z -= right.z;
            
            return *this;
        }

        vec3& operator*=(const float& value)
        {
            this->x *= value;
            this->y *= value;
            this->z *= value;

            return *this;
        }

        vec3& operator/=(const float& value)
        {
            this->x /= value;
            this->y /= value;
            this->z /= value;

            return *this;
        }

        vec3 operator+(const vec3& right) const
        {
            vec3 res;

            res.x = this->x + right.x;
            res.y = this->y + right.y;
            res.z = this->z + right.z;

            return res;
        }

        vec3 operator-(const vec3& right) const
        {
            vec3 res;

            res.x = this->x - right.x;
            res.y = this->y - right.y;
            res.z = this->z - right.z;

            return res;
        }

        vec3 operator-() const
        {
            vec3 res;

            res.x = -this->x;
            res.y = -this->y;
            res.z = -this->z;

            return res;
        }

        vec3 operator*(const float& value) const
        {
            vec3 res = *this;

            res *= value;

            return res;
        }

        vec3 operator/(const float& value) const
        {
            vec3 res = *this;

            res /= value;

            return res;
        }
    
        float operator[](unsigned int i) const
        {
            if (i == 0)
                return x;
            else if (i == 1)
                return y;
            else
                return z;
        }

        bool operator==(const vec3& vec) const
        {
            return (this->x == vec.x && this->y == vec.y && this->z == vec.z);
        }

        bool operator!=(const vec3& vec) const
        {
            return (this->x != vec.x || this->y != vec.y || this->z != vec.z);
        }

        inline
        static float dot(const vec3& a, const vec3& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        inline
        float dot(const vec3& b) const
        {
            return dot(*this, b);
        }

        inline
        static vec3 cross(const vec3& a, const vec3& b)
        {
            vec3 res;
            
            res.x = a.y * b.z - a.z * b.y;
            res.y = a.z * b.x - a.x * b.z;
            res.z = a.x * b.y - a.y * b.x;

            return res;
        }

        inline
        vec3 cross(const vec3& b) const
        {
            return cross(*this, b);
        }

        inline
        static float	length(const vec3& v1)
        {
            return (sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z));
        }

        inline
        float	length() const
        {
            return length(*this);
        }
    
        inline
        static vec3		normalize(const vec3& v1)
        {
            vec3	res;
            float	vec_length;

            vec_length = length(v1);
            res.x = v1.x / vec_length;
            res.y = v1.y / vec_length;
            res.z = v1.z / vec_length;

            return res;
        }
    
        inline
        vec3		normalize() const
        {
            return normalize(*this);
        }

        inline
        static vec3		normalize_safe(const vec3& v1)
        {
            vec3	res;
            float	vec_length;

            if (v1.x == 0.f && v1.y == 0.f && v1.z == 0.f)
            {
                return (vec3){0.f, 0.f, 0.f};
            }

            vec_length = length(v1);
            res.x = v1.x / vec_length;
            res.y = v1.y / vec_length;
            res.z = v1.z / vec_length;

            return res;
        }

        inline
        vec3		normalize_safe() const
        {
            return normalize_safe(*this);
        }

        inline
        static vec3 lerp(const vec3& a, const vec3& b, const float& value)
        {
            vec3 res;

            res.x =  mix(a.x, b.x, value);
            res.y =  mix(a.y, b.y, value);
            res.z =  mix(a.z, b.z, value);

            return res;
        }

        inline
        vec3 lerp(const vec3& b, const float& value) const
        {
            return lerp(*this, b, value);
        }

        inline
        static float distance(const vec3& from, const vec3& to);
    };

    inline
    float vec3::distance(const vec3& from, const vec3& to)
    {
        return length(to - from);
    }
}

namespace mathf
{
    struct vec2
    {
        float x;
        float y;

        vec2()
        {
            vec2(0.f);
        }

        vec2(const float& value)
        {
            this->x = value;
            this->y = value;
        }

        vec2(const float& x, const float& y)
        {
            this->x = x;
            this->y = y;
        }

        vec2(const vec2& vec)
        {
            this->x = vec.x;
            this->y = vec.y;
        }

        void print(std::string arg)
        {
            std::cout<<arg<<" vec2: "<<this->x<< " " << this->y<<std::endl;
        }

        void print()
        {
            std::cout<<"vec2: "<<this->x<< " " << this->y<<std::endl;
        }

        vec2& operator=(const vec2& vec)
        {
            this->x = vec.x;
            this->y = vec.y;

            return *this;
        }

        vec2& operator+=(const vec2& right)
        {
            this->x += right.x;
            this->y += right.y;
            
            return *this;
        }

        vec2& operator-=(const vec2& right)
        {
            this->x -= right.x;
            this->y -= right.y;
            
            return *this;
        }

        vec2& operator*=(const float& value)
        {
            this->x *= value;
            this->y *= value;

            return *this;
        }

        vec2& operator/=(const float& value)
        {
            this->x /= value;
            this->y /= value;

            return *this;
        }

        vec2 operator+(const vec2& right)
        {
            vec2 res;

            res.x = this->x + right.x;
            res.y = this->y + right.y;

            return res;
        }

        vec2 operator-(const vec2& right) const
        {
            vec2 res;

            res.x = this->x - right.x;
            res.y = this->y - right.y;

            return res;
        }

        vec2 operator*(const float& value) const
        {
            vec2 res = *this;

            res *= value;

            return res;
        }

        vec2 operator/(const float& value) const
        {
            vec2 res = *this;

            res /= value;

            return res;
        }
        bool operator==(const vec2& vec) const
        {
            return (this->x == vec.x && this->y == vec.y);
        }

        bool operator!=(const vec2& vec) const
        {
            return (this->x != vec.x || this->y != vec.y);
        }

        inline
        static float dot(const vec2& a, const vec2& b)
        {
            return a.x * b.x + a.y * b.y;
        }

        inline
        float dot(const vec2& b) const
        {
            return dot(*this, b); 
        }

        inline
        static vec2 cross(const vec2& a, const vec2& b)
        {
            vec2 res;

            res.x = 0.f;
            res.y = a.x * b.y - a.y * b.x;

            return res;
        }

        inline
        vec2 cross(const vec2& b) const
        {
            return cross(*this, b);
        }

        inline
        static float length(const vec2 v1)
        {
            return sqrtf(v1.x * v1.x + v1.y * v1.y);
        }

        inline
        float length() const
        {
            return length(*this);
        }

        inline
        static vec2 normalize(const vec2& v1)
        {
            vec2	res;
            float	vec_length;

            vec_length = length(v1);
            res.x = v1.x / vec_length;
            res.y = v1.y / vec_length;

            return res;
        }

        inline
        vec2 normalize() const
        {
            return normalize(*this);
        }

        inline
        static vec2 normalize_safe(const vec2& v1)
        {
            vec2	res;
            float	vec_length;

            if (v1.x == 0.f && v1.y == 0.f)
            {
                return (vec2){0.f, 0.f};
            }

            vec_length = length(v1);
            res.x = v1.x / vec_length;
            res.y = v1.y / vec_length;
            
            return res;
        }

        inline
        vec2 normalize_safe() const
        {
            return normalize_safe(*this);
        }

        static float distance(const vec2& from, const vec2& to);
    };


    inline
    float vec2::distance(const vec2& from, const vec2& to)
    {
        return length(to - from);
    }
}

namespace mathf
{
    struct ivec2
    {
        int x;
        int y;

        ivec2()
        {
            ivec2(0.f);
        }

        explicit ivec2(const int& value)
        {
            this->x = value;
            this->y = value;
        }

        ivec2(const int& x, const int& y)
        {
            this->x = x;
            this->y = y;
        }

        ivec2(const ivec2& vec)
        {
            this->x = vec.x;
            this->y = vec.y;
        }

        void print(std::string arg)
        {
            std::cout<<arg<<" ivec2: "<<this->x<< " " << this->y<<std::endl;
        }

        void print()
        {
            std::cout<<"ivec2: "<<this->x<< " " << this->y<<std::endl;
        }

        ivec2& operator=(const ivec2& vec)
        {
            this->x = vec.x;
            this->y = vec.y;

            return *this;
        }

        ivec2& operator+=(const ivec2& right)
        {
            this->x += right.x;
            this->y += right.y;
            
            return *this;
        }

        ivec2& operator-=(const ivec2& right)
        {
            this->x -= right.x;
            this->y -= right.y;
            
            return *this;
        }

        ivec2& operator*=(const float& value)
        {
            this->x *= value;
            this->y *= value;

            return *this;
        }

        ivec2& operator/=(const float& value)
        {
            this->x /= value;
            this->y /= value;

            return *this;
        }

        ivec2 operator+(const ivec2& right) const
        {
            ivec2 res;

            res.x = this->x + right.x;
            res.y = this->y + right.y;

            return res;
        }

        ivec2 operator-(const ivec2& right) const
        {
            ivec2 res;

            res.x = this->x - right.x;
            res.y = this->y - right.y;

            return res;
        }

        ivec2 operator*(const float& value) const
        {
            ivec2 res = *this;

            res *= value;

            return res;
        }

        ivec2 operator/(const float& value) const
        {
            ivec2 res = *this;

            res /= value;

            return res;
        }

        bool operator==(const ivec2& vec) const
        {
            return (this->x == vec.x && this->y == vec.y);
        }

        bool operator!=(const ivec2& vec) const
        {
            return (this->x != vec.x || this->y != vec.y);
        }

        inline
        static float dot(const ivec2& a, const ivec2& b)
        {
            return a.x * b.x + a.y * b.y;
        }

        inline
        float dot(const ivec2& b) const
        {
            return dot(*this, b); 
        }

        inline
        static ivec2 cross(const ivec2& a, const ivec2& b)
        {
            ivec2 res;

            res.x = 0.f;
            res.y = a.x * b.y - a.y * b.x;

            return res;
        }

        inline
        ivec2 cross(const ivec2& b) const
        {
            return cross(*this, b);
        }

        inline
        static float length(const ivec2 v1)
        {
            return sqrtf(v1.x * v1.x + v1.y * v1.y);
        }

        inline
        float length() const
        {
            return length(*this);
        }

        inline
        static ivec2 normalize(const ivec2& v1)
        {
            ivec2	res;
            float	vec_length;

            vec_length = length(v1);
            res.x = v1.x / vec_length;
            res.y = v1.y / vec_length;

            return res;
        }

        inline
        ivec2 normalize() const
        {
            return normalize(*this);
        }

        inline
        static ivec2 normalize_safe(const ivec2& v1)
        {
            ivec2	res;
            float	vec_length;

            if (v1.x == 0.f && v1.y == 0.f)
            {
                return (ivec2){0.f, 0.f};
            }

            vec_length = length(v1);
            res.x = v1.x / vec_length;
            res.y = v1.y / vec_length;
            
            return res;
        }

        inline
        ivec2 normalize_safe() const
        {
            return normalize_safe(*this);
        }

        static float distance(const ivec2& from, const ivec2& to);
    };


    inline
    float ivec2::distance(const ivec2& from, const ivec2& to)
    {
        return length(to - from);
    }
}

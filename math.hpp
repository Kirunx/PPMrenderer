#pragma once
#include <math.h>

struct vec4 {
    float x, y, z, w;
    vec4(float x = 0, float y = 0, float z = 0, float w = 0)
        : x(x)
        , y(y)
        , z(z)
        , w(w) { }
};
struct vec2 {
    float x, y;
    vec2(float x = 0, float y = 0)
        : x(x)
        , y(y) { }
    vec2 yx() const { return vec2(y, x); }
    vec4 xyyx() const { return vec4(x, y, y, x); }
};
struct mat4 {
    float m[4][4];
    //Конструктор единичной матрицы
    mat4() {
        m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
        m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
    }

    //Статические фабричные методы для перемещения поворта и масштаба
    static mat4 translation(const vec4& v) {
        mat4 res; 
        res.m[0][3] = v.x;
        res.m[1][3] = v.y;
        res.m[2][3] = v.z;
        return res;
    }
    static mat4 scale(const vec4& v) {
        mat4 res;
        res.m[0][0] = v.x;
        res.m[1][1] = v.y;
        res.m[2][2] = v.z;
        return res;
    }
    static mat4 rotate_y(float angle_radians);

    // Матрицы камеры/зрителя
    static mat4 look_at(vec4 eye, vec4 center, vec4 up);
    static mat4 perspective(float fov_y, float aspect, float near_z, float far_z);
};
inline mat4 operator*(const mat4& a, const mat4& b) {
    mat4 result;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.m[row][col] = a.m[row][0] * b.m[0][col] +
                                 a.m[row][1] * b.m[1][col] +
                                 a.m[row][2] * b.m[2][col] +
                                 a.m[row][3] * b.m[3][col];
        }
    }
    return result;
}
inline vec4 operator*(const mat4& mat, const vec4& vec) {
    return vec4(
        mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3] * vec.w,
        mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3] * vec.w,
        mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3] * vec.w,
        mat.m[3][0] * vec.x + mat.m[3][1] * vec.y + mat.m[3][2] * vec.z + mat.m[3][3] * vec.w
    );
}
inline vec2 operator*(const vec2& a, float s) { return vec2(a.x * s, a.y * s); }
inline vec2 operator+(const vec2& a, float s) { return vec2(a.x + s, a.y + s); }
inline vec2 operator*(float s, const vec2& a) { return a * s; }
inline vec2 operator-(const vec2& a, const vec2& b) { return vec2(a.x - b.x, a.y - b.y); }
inline vec2 operator+(const vec2& a, const vec2& b) { return vec2(a.x + b.x, a.y + b.y); }
inline vec2 operator*(const vec2& a, const vec2& b) { return vec2(a.x * b.x, a.y * b.y); }
inline vec2 operator/(const vec2& a, float s) { return vec2(a.x / s, a.y / s); }
inline float dot(const vec2& a, const vec2& b) { return a.x * b.x + a.y * b.y; }
inline vec2 abs(const vec2& a) { return vec2(fabsf(a.x), fabsf(a.y)); }
inline vec2& operator+=(vec2& a, const vec2& b) {
    a = a + b;
    return a;
}
inline vec2& operator+=(vec2& a, float s) {
    a = a + s;
    return a;
}
inline vec2 cos(const vec2& a) { return vec2(cosf(a.x), cosf(a.y)); }
inline vec4 sin(const vec4& a) { return vec4(sinf(a.x), sinf(a.y), sinf(a.z), sinf(a.w)); }
inline vec4 exp(const vec4& a) { return vec4(expf(a.x), expf(a.y), expf(a.z), expf(a.w)); }
inline vec4 tanh(const vec4& a) { return vec4(tanhf(a.x), tanhf(a.y), tanhf(a.z), tanhf(a.w)); }
inline vec4 operator+(const vec4& a, float s) { return vec4(a.x + s, a.y + s, a.z + s, a.w + s); }
inline vec4 operator*(const vec4& a, float s) { return vec4(a.x * s, a.y * s, a.z * s, a.w * s); }
inline vec4 operator*(float s, const vec4& a) { return a * s; }
inline vec4 operator+(const vec4& a, const vec4& b) { return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
inline vec4& operator+=(vec4& a, const vec4& b) {
    a = a + b;
    return a;
}
inline vec4 operator-(float s, const vec4& a) { return vec4(s - a.x, s - a.y, s - a.z, s - a.w); }
inline vec4 operator/(const vec4& a, const vec4& b) { return vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
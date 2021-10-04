#pragma once

#include <cmath>

#pragma pack(push, 1)
template <class T>
class Vector2 {
public:
    T x, y;

    Vector2() : x(0), y(0) { }
    Vector2(T x, T y) : x(x), y(y) { }

    template<class U>
    explicit Vector2(Vector2<U> v) : x(v.x), y(v.y) { }

    Vector2<T> operator-() const { return {-x, -y}; }
    Vector2<T> operator+(const Vector2<T> &a) const { return {x + a.x, y + a.y}; }
    Vector2<T> operator-(const Vector2<T> &a) const { return {x - a.x, y - a.y}; }
    Vector2<T> operator*(const Vector2<T> &a) const { return (x * a.x, y * a.y); }
    Vector2<T> operator/(const Vector2<T> &a) const { return {x / a.x, y / a.y}; }
    Vector2<T> operator*(T a) const { return {x * a, y * a}; }
    Vector2<T> operator/(T a) const { return {x / a, y / a}; }

    Vector2<T> lerp(const Vector2<T> &a, T t) const { return *this + (a - *this) * t; }
    Vector2<T> unit() const { return *this / length(); }
    T dot(const Vector2<T> &a) const { return x * a.x + y * a.y; }
    T length() const { return sqrt(dot(*this)); }

    template<class U>
    Vector2<U> to() { return { (U)x, (U)y }; }

    static constexpr Vector2<T> origin = {0, 0};
};
#pragma pack(pop)

typedef Vector2<int> ivec2;
typedef Vector2<float> vec2;
typedef Vector2<double> dvec2;

#pragma pack(push, 1)
template <class T>
class Vector3 {
public:
    T x, y, z;

    Vector3() : x(0), y(0), z(0) { }
    Vector3(T x, T y, T z) : x(x), y(y), z(z) { }

    template<class U>
    explicit Vector3(Vector3<U> v) : x(v.x), y(v.y), z(v.z) { }

    Vector3<T> operator-() const { return {-x, -y, -z}; }
    Vector3<T> operator+(const Vector3<T> &a) const { return {x + a.x, y + a.y, z + a.z}; }
    Vector3<T> operator-(const Vector3<T> &a) const { return {x - a.x, y - a.y, z - a.z}; }
    Vector3<T> operator*(const Vector3<T> &a) const { return {x * a.x, y * a.y, z * a.z}; }
    Vector3<T> operator/(const Vector3<T> &a) const { return {x / a.x, y / a.y, z / a.z}; }
    Vector3<T> operator*(T a) const { return {x * a, y * a, z * a}; }
    Vector3<T> operator/(T a) const { return {x / a, y / a, z / a}; }

    Vector3<T> cross(const Vector3<T> &a) const { return {y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x}; }
    Vector3<T> lerp(const Vector3<T> &a, T t) const { return *this + (a - *this) * t; }
    Vector3<T> unit() const { return *this / length(); }
    T dot(const Vector3<T> &a) const { return x * a.x + y * a.y + z * a.z; }
    T length() const { return sqrt(dot(*this)); }

    template<class U>
    Vector3<U> to() { return { (U)x, (U)y, (U)z }; }

    static constexpr Vector3<T> origin = {0, 0, 0};
};
#pragma pack(pop)

typedef Vector3<int> ivec3;
typedef Vector3<float> vec3;
typedef Vector3<double> dvec3;

#pragma pack(push, 1)
template <class T>
class Vector4 {
public:
    T x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(0) { }
    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }

    template<class U>
    explicit Vector4(Vector4<U> v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

    Vector4<T> operator-() const { return {-x, -y, -z, -w}; }
    Vector4<T> operator+(const Vector4<T> &a) const { return {x + a.x, y + a.y, z + a.z, w + a.w}; }
    Vector4<T> operator-(const Vector4<T> &a) const { return {x - a.x, y - a.y, z - a.z, w - a.w}; }
    Vector4<T> operator*(const Vector4<T> &a) const { return {x * a.x, y * a.y, z * a.z, w * a.w}; }
    Vector4<T> operator/(const Vector4<T> &a) const { return {x / a.x, y / a.y, z / a.z, w / a.w}; }
    Vector4<T> operator*(T a) const { return {x * a, y * a, z * a, w * a}; }
    Vector4<T> operator/(T a) const { return {x / a, y / a, z / a, w / a}; }

    Vector4<T> lerp(const Vector4<T> &a, T t) const { return *this + (a - *this) * t; }
    Vector4<T> unit() const { return *this / length(); }
    T dot(const Vector4<T> &a) const { return x * a.x + y * a.y + z * a.z + w * a.w; }
    T length() const { return sqrt(dot(*this)); }

    template<class U>
    Vector4<U> to() { return { (U)x, (U)y, (U)z, (U)w }; }

    static constexpr Vector4<T> origin = {0, 0, 0, 0};
};
#pragma pack(pop)

typedef Vector4<int> ivec4;
typedef Vector4<float> vec4;
typedef Vector4<double> dvec4;

class Matrix4 {
public:
    float m[16]{};

    Matrix4() = default;
    Matrix4(float a00, float a01, float a02, float a03,
            float a04, float a05, float a06, float a07,
            float a08, float a09, float a10, float a11,
            float a12, float a13, float a14, float a15) {
        m[0] = a00; m[1] = a01; m[2] = a02; m[3] = a03;
        m[4] = a04; m[5] = a05; m[6] = a06; m[7] = a07;
        m[8] = a08; m[9] = a09; m[10] = a10; m[11] = a11;
        m[12] = a12; m[13] = a13; m[14] = a14; m[15] = a15;
    }

    float &operator[](int i) { return m[i]; }
    const float &operator[](int i) const { return m[i]; }

    Matrix4 operator*(const Matrix4 &a) const {
        Matrix4 res{};
        for (int i = 0; i<4; i++)
        {
            auto x = m[4 * i + 0];
            auto y = m[4 * i + 1];
            auto z = m[4 * i + 2];
            auto w = m[4 * i + 3];

            res.m[4 * i + 0] = x * a[0] + y * a[4] + z * a[8] + w * a[12];
            res.m[4 * i + 1] = x * a[1] + y * a[5] + z * a[9] + w * a[13];
            res.m[4 * i + 2] = x * a[2] + y * a[6] + z * a[10] + w * a[14];
            res.m[4 * i + 3] = x * a[3] + y * a[7] + z * a[11] + w * a[15];
        }

        return res;
    }

    vec4 operator*(const vec4 &v) const {
        return {v.x*m[0] + v.y*m[1] + v.z*m[2] + v.w*m[3],
                v.x*m[4] + v.y*m[5] + v.z*m[6] + v.w*m[7],
                v.x*m[8] + v.y*m[9] + v.z*m[10] + v.w*m[11],
                v.x*m[12] + v.y*m[13] + v.z*m[14] + v.w*m[15]};
    }

    vec3 operator*(const vec3 &v) const {
        return {v.x*m[0] + v.y*m[1] + v.z*m[2] + m[3],
                v.x*m[4] + v.y*m[5] + v.z*m[6] + m[7],
                v.x*m[8] + v.y*m[9] + v.z*m[10] + m[11]};
    }

    Matrix4 transpose() const {
        return {m[0], m[4], m[8], m[12],
                m[1], m[5], m[9], m[13],
                m[2], m[6], m[10], m[14],
                m[3], m[7], m[11], m[15]};
    }

    Matrix4 invert() const {
        Matrix4 inv = Matrix4(
                m[5] * m[10] * m[15] -
                m[5] * m[11] * m[14] -
                m[9] * m[6] * m[15] +
                m[9] * m[7] * m[14] +
                m[13] * m[6] * m[11] -
                m[13] * m[7] * m[10],

                -m[1] * m[10] * m[15] +
                m[1] * m[11] * m[14] +
                m[9] * m[2] * m[15] -
                m[9] * m[3] * m[14] -
                m[13] * m[2] * m[11] +
                m[13] * m[3] * m[10],

                m[1] * m[6] * m[15] -
                m[1] * m[7] * m[14] -
                m[5] * m[2] * m[15] +
                m[5] * m[3] * m[14] +
                m[13] * m[2] * m[7] -
                m[13] * m[3] * m[6],

                -m[1] * m[6] * m[11] +
                m[1] * m[7] * m[10] +
                m[5] * m[2] * m[11] -
                m[5] * m[3] * m[10] -
                m[9] * m[2] * m[7] +
                m[9] * m[3] * m[6],

                -m[4] * m[10] * m[15] +
                m[4] * m[11] * m[14] +
                m[8] * m[6] * m[15] -
                m[8] * m[7] * m[14] -
                m[12] * m[6] * m[11] +
                m[12] * m[7] * m[10],

                m[0] * m[10] * m[15] -
                m[0] * m[11] * m[14] -
                m[8] * m[2] * m[15] +
                m[8] * m[3] * m[14] +
                m[12] * m[2] * m[11] -
                m[12] * m[3] * m[10],

                -m[0] * m[6] * m[15] +
                m[0] * m[7] * m[14] +
                m[4] * m[2] * m[15] -
                m[4] * m[3] * m[14] -
                m[12] * m[2] * m[7] +
                m[12] * m[3] * m[6],


                m[0] * m[6] * m[11] -
                m[0] * m[7] * m[10] -
                m[4] * m[2] * m[11] +
                m[4] * m[3] * m[10] +
                m[8] * m[2] * m[7] -
                m[8] * m[3] * m[6],


                m[4] * m[9] * m[15] -
                m[4] * m[11] * m[13] -
                m[8] * m[5] * m[15] +
                m[8] * m[7] * m[13] +
                m[12] * m[5] * m[11] -
                m[12] * m[7] * m[9],



                -m[0] * m[9] * m[15] +
                m[0] * m[11] * m[13] +
                m[8] * m[1] * m[15] -
                m[8] * m[3] * m[13] -
                m[12] * m[1] * m[11] +
                m[12] * m[3] * m[9],

                m[0] * m[5] * m[15] -
                m[0] * m[7] * m[13] -
                m[4] * m[1] * m[15] +
                m[4] * m[3] * m[13] +
                m[12] * m[1] * m[7] -
                m[12] * m[3] * m[5],

                -m[0] * m[5] * m[11] +
                m[0] * m[7] * m[9] +
                m[4] * m[1] * m[11] -
                m[4] * m[3] * m[9] -
                m[8] * m[1] * m[7] +
                m[8] * m[3] * m[5],

                -m[4] * m[9] * m[14] +
                m[4] * m[10] * m[13] +
                m[8] * m[5] * m[14] -
                m[8] * m[6] * m[13] -
                m[12] * m[5] * m[10] +
                m[12] * m[6] * m[9],

                m[0] * m[9] * m[14] -
                m[0] * m[10] * m[13] -
                m[8] * m[1] * m[14] +
                m[8] * m[2] * m[13] +
                m[12] * m[1] * m[10] -
                m[12] * m[2] * m[9],

                -m[0] * m[5] * m[14] +
                m[0] * m[6] * m[13] +
                m[4] * m[1] * m[14] -
                m[4] * m[2] * m[13] -
                m[12] * m[1] * m[6] +
                m[12] * m[2] * m[5],

                m[0] * m[5] * m[10] -
                m[0] * m[6] * m[9] -
                m[4] * m[1] * m[10] +
                m[4] * m[2] * m[9] +
                m[8] * m[1] * m[6] -
                m[8] * m[2] * m[5]
        );

        auto det = m[0] * inv.m[0] + m[1] * inv.m[4] + m[2] * inv.m[8] + m[3] * inv.m[12];
        det = 1.0f / det;
        for (float &i : inv.m) i = i * det;
        return inv;
    }

    static Matrix4 rotate(const vec3 &a, float t) {
        auto sint = sinf(t);
        auto cost = cosf(t);
        auto icost = 1 - cost;

        return {a.x * a.x * icost + cost,
                a.y * a.x * icost - sint * a.z,
                a.z * a.x * icost + sint * a.y,
                0,
                a.x * a.y * icost + sint * a.z,
                a.y * a.y * icost + cost,
                a.z * a.y * icost - sint * a.x,
                0,
                a.x * a.z * icost - sint * a.y,
                a.y * a.z * icost + sint * a.x,
                a.z * a.z * icost + cost,
                0,
                0, 0, 0, 1};
    }

    static Matrix4 rotateX(float t) {
        auto sint = sinf(t);
        auto cost = cosf(t);

        return {1, 0, 0, 0,
                0, cost, sint, 0,
                0, -sint, cost, 0,
                0, 0, 0, 1};
    }

    static Matrix4 rotateY(float t) {
        auto sint = sinf(t);
        auto cost = cosf(t);

        return {cost, 0, -sint, 0,
                0, 1, 0, 0,
                sint, 0, cost, 0,
                0, 0, 0, 1};
    }

    static Matrix4 rotateZ(float t) {
        auto sint = sinf(t);
        auto cost = cosf(t);

        return {cost, -sint, 0, 0,
                sint, cost, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1};
    }

    static Matrix4 shearX(float y, float z) {
        return {1, 0, 0, 0,
                y, 1, 0, 0,
                z, 0, 1, 0,
                0, 0, 0, 1};
    }

    static Matrix4 shearY(float x, float z) {
        return {1, x, 0, 0,
                0, 1, 0, 0,
                0, z, 1, 0,
                0, 0, 0, 1};
    }

    static Matrix4 shearZ(float x, float y) {
        return {1, 0, x, 0,
                0, 1, y, 0,
                0, 0, 1, 0,
                0, 0, 0, 1};
    }

    static Matrix4 move(const vec3 &p) {
        return {1, 0, 0, p.x,
                0, 1, 0, p.y,
                0, 0, 1, p.z,
                0, 0, 0, 1};
    }

    static Matrix4 scale(const vec3 &s) {
        return {s.x, 0, 0, 0,
                0, s.y, 0, 0,
                0, 0, s.z, 0,
                0, 0, 0, 1};
    }

    static Matrix4 getPerspective(float fovy, float aspect, float znear, float zfar) {
        auto tanv = tanf(fovy * (float)M_PI / 180);
        auto x = 1 / (tanv * aspect);
        auto y = 1 / tanv;
        auto c = -(zfar + znear) / (zfar - znear);
        auto d = -(2 * zfar * znear) / (zfar - znear);

        return {x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, c, d,
                0, 0, -1, 0};
    }

    static Matrix4 getOrtho(vec2 xClip, vec2 yClip, vec2 zClip) {
        auto x = 2 / (xClip.y - xClip.x);
        auto y = 2 / (yClip.y - yClip.x);
        auto z = -2 / (zClip.y - zClip.x);
        auto c = -(xClip.y + xClip.x) / (xClip.y - xClip.x);
        auto d = -(yClip.y + yClip.x) / (yClip.y - yClip.x);
        auto e = -(zClip.y + zClip.x) / (zClip.y - zClip.x);

        return {x, 0, 0, c,
                0, y, 0, d,
                0, 0, z, e,
                0, 0, 0, 1};
    }

    static Matrix4 getView(vec3 p, vec3 f, vec3 u) {
        vec3 r = f.cross(u);
        Matrix4 inverseMatrix = {
            r.x, u.x, f.x, p.x,
            r.y, u.y, f.y, p.y,
            r.z, u.z, f.z, p.z,
            0,   0,   0,   1
        };
        return inverseMatrix.invert();
    }

    static Matrix4 getIdentity() {
        return {1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1};
    }

};

class Direction {
public:
    float pitch, yaw, roll;

    Direction(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll) { }
    vec3 toVector() { return {pitch, yaw, roll}; }

    Matrix4 toMatrix() {
        auto a = sinf(pitch);
        auto b = cosf(pitch);
        auto c = sinf(yaw);
        auto d = cosf(yaw);
        auto e = sinf(roll);
        auto f = cosf(roll);

        return {
            d*b, -c*e-d*a*f, d*a*e-c*f, 0,
            a, b*f, -b*e, 0,
            c*b, d*e-c*a*f, c*a*e+d*f, 0,
            0, 0, 0, 1
        };
    }

    vec3 toForward() {
        return {
            cosf(pitch) * cosf(yaw),
            sinf(pitch),
            cosf(pitch) * sinf(yaw)
        };
    }

    vec3 toUp() {
        auto a = sinf(pitch);
        auto b = cosf(pitch);
        auto c = sinf(yaw);
        auto d = cosf(yaw);
        auto e = sinf(roll);
        auto f = cosf(roll);

        return {
            -a*d*f-c*e,
            b*f,
            d*e-a*c*f
        };
    }

    vec4 operator*(const vec4 &x) { return toMatrix() * x; }
    vec3 operator*(const vec3 &x) { return toMatrix() * x; }
    Direction operator+(const Direction &d) { return { pitch + d.pitch, yaw + d.yaw, roll + d.roll }; }
};

class ForwardDirection {
public:
    float pitch, yaw;

    ForwardDirection(float pitch, float yaw) : pitch(pitch), yaw(yaw) { }
    Direction toDirection() { return {pitch, yaw, 0}; }

    Matrix4 toMatrix() { return toDirection().toMatrix(); }
    vec3 toForward() { return toDirection().toForward(); }
    vec3 toUp() { return toDirection().toUp(); }

    vec4 operator*(const vec4 &x) { return toMatrix() * x; }
    vec3 operator*(const vec3 &x) { return toMatrix() * x; }
    ForwardDirection operator+(const ForwardDirection &d) { return {pitch + d.pitch, yaw + d.yaw}; }
};

#pragma once
#include <cmath>

// Newton-Raphson square root approximation
static constexpr float sqrt_nr(float n) noexcept {
    if (n < 0.0f) return -1.0f;
    if (n == 0.0f) return 0.0f;
    float x = n;
    float x1 = (x + n / x) * 0.5f;
    while (x1 < x) {
        x = x1;
        x1 = (x + n / x) * 0.5f;
    }
    return x;
}

// Forward declaration
struct Vector2Df;

struct Vector2D {
    int x, y;

    // Constructors
    constexpr Vector2D() noexcept : x(0), y(0) {}
    constexpr Vector2D(int x, int y) noexcept : x(x), y(y) {}

    // Arithmetic operators
    [[nodiscard]] constexpr Vector2D operator+(const Vector2D& o) const noexcept { return {x + o.x, y + o.y}; }
    [[nodiscard]] constexpr Vector2D operator-(const Vector2D& o) const noexcept { return {x - o.x, y - o.y}; }
    [[nodiscard]] constexpr Vector2D operator*(int scalar)        const noexcept { return {x * scalar, y * scalar}; }
    [[nodiscard]] constexpr Vector2D operator/(int scalar)        const noexcept { return {x / scalar, y / scalar}; }
    [[nodiscard]] constexpr Vector2D operator-()                  const noexcept { return {-x, -y}; }

    // Compound assignment operators
    constexpr Vector2D& operator+=(const Vector2D& o) noexcept { x += o.x; y += o.y; return *this; }
    constexpr Vector2D& operator-=(const Vector2D& o) noexcept { x -= o.x; y -= o.y; return *this; }
    constexpr Vector2D& operator*=(const Vector2D& o) noexcept { x *= o.x; y *= o.y; return *this; }
    constexpr Vector2D& operator*=(int scalar)        noexcept { x *= scalar; y *= scalar; return *this; }
    constexpr Vector2D& operator/=(int scalar)        noexcept { x /= scalar; y /= scalar; return *this; }

    // Comparison operators
    [[nodiscard]] constexpr bool operator==(const Vector2D& o) const noexcept { return x == o.x && y == o.y; }
    [[nodiscard]] constexpr bool operator!=(const Vector2D& o) const noexcept { return !(*this == o); }

    // Utility methods
    [[nodiscard]] constexpr float magnitude() const noexcept { return sqrt_nr(static_cast<float>(x*x + y*y)); }
    [[nodiscard]] constexpr int   dot(const Vector2D& o)     const noexcept { return x * o.x + y * o.y; }

    // Index operator
    [[nodiscard]] constexpr int&       operator[](int i)       noexcept { return i == 0 ? x : y; }
    [[nodiscard]] constexpr const int& operator[](int i) const noexcept { return i == 0 ? x : y; }

    // Conversion to Vector2Df (defined after Vector2Df is complete)
    [[nodiscard]] constexpr Vector2Df toFloat() const noexcept;
};

[[nodiscard]] constexpr inline Vector2D operator*(int scalar, const Vector2D& v) noexcept { return v * scalar; }

struct Vector2Df {
    float x, y;

    // Constructors
    constexpr Vector2Df() noexcept : x(0.0f), y(0.0f) {}
    constexpr Vector2Df(float x, float y) noexcept : x(x), y(y) {}

    // Implicit conversion from Vector2D
    constexpr Vector2Df(const Vector2D& v) noexcept : x(static_cast<float>(v.x)), y(static_cast<float>(v.y)) {}

    // Arithmetic operators
    [[nodiscard]] constexpr Vector2Df operator+(const Vector2Df& o) const noexcept { return {x + o.x, y + o.y}; }
    [[nodiscard]] constexpr Vector2Df operator-(const Vector2Df& o) const noexcept { return {x - o.x, y - o.y}; }
    [[nodiscard]] constexpr Vector2Df operator*(float scalar)       const noexcept { return {x * scalar, y * scalar}; }
    [[nodiscard]] constexpr Vector2Df operator/(float scalar)       const noexcept { return {x / scalar, y / scalar}; }
    [[nodiscard]] constexpr Vector2Df operator-()                   const noexcept { return {-x, -y}; }

    // Compound assignment operators
    constexpr Vector2Df& operator+=(const Vector2Df& o) noexcept { x += o.x; y += o.y; return *this; }
    constexpr Vector2Df& operator-=(const Vector2Df& o) noexcept { x -= o.x; y -= o.y; return *this; }
    constexpr Vector2Df& operator*=(const Vector2Df& o) noexcept { x *= o.x; y *= o.y; return *this; }
    constexpr Vector2Df& operator*=(float scalar)       noexcept { x *= scalar; y *= scalar; return *this; }
    constexpr Vector2Df& operator/=(float scalar)       noexcept { x /= scalar; y /= scalar; return *this; }

    // Comparison operators (epsilon-based for floating point)
    [[nodiscard]] constexpr bool operator==(const Vector2Df& o) const noexcept {
        constexpr float eps = 1e-6f;
        return (x - o.x) * (x - o.x) + (y - o.y) * (y - o.y) < eps * eps;
    }
    [[nodiscard]] constexpr bool operator!=(const Vector2Df& o) const noexcept { return !(*this == o); }

    // Utility methods
    [[nodiscard]] float magnitude() const noexcept { return std::sqrt(x*x + y*y); }  // std::sqrt, not std::sqrtf
    [[nodiscard]] constexpr float dot(const Vector2Df& o) const noexcept { return x * o.x + y * o.y; }

    // Returns a unit vector (zero vector if magnitude is too small)
    [[nodiscard]] Vector2Df normalized() const noexcept {
        float mag = magnitude();
        if (mag < 1e-6f) return {0.0f, 0.0f};
        return {x / mag, y / mag};
    }

    // Index operator
    [[nodiscard]] constexpr float&       operator[](int i)       noexcept { return i == 0 ? x : y; }
    [[nodiscard]] constexpr const float& operator[](int i) const noexcept { return i == 0 ? x : y; }

    // Conversion to Vector2D (truncates, does not round)
    [[nodiscard]] constexpr Vector2D toInt() const noexcept { return {static_cast<int>(x), static_cast<int>(y)}; }
};

[[nodiscard]] constexpr inline Vector2Df operator*(float scalar, const Vector2Df& v) noexcept { return v * scalar; }
[[nodiscard]] constexpr Vector2Df Vector2D::toFloat() const noexcept { return {static_cast<float>(x), static_cast<float>(y)}; }
#pragma once

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
    constexpr Vector2D& operator*=(int scalar)        noexcept { x *= scalar; y *= scalar; return *this; }
    constexpr Vector2D& operator/=(int scalar)        noexcept { x /= scalar; y /= scalar; return *this; }

    // Comparison operators
    [[nodiscard]] constexpr bool operator==(const Vector2D& o) const noexcept { return x == o.x && y == o.y; }
    [[nodiscard]] constexpr bool operator!=(const Vector2D& o) const noexcept { return !(*this == o); }

    // Utility methods
    [[nodiscard]] constexpr float magnitude() const noexcept { return sqrt_nr(static_cast<float>(x*x + y*y)); }
    [[nodiscard]] constexpr int   dot(const Vector2D& o) const noexcept { return x * o.x + y * o.y; }

    // Index operator
    [[nodiscard]] constexpr int&       operator[](int i)       noexcept { return i == 0 ? x : y; }
    [[nodiscard]] constexpr const int& operator[](int i) const noexcept { return i == 0 ? x : y; }
};

// Scalar * Vector
[[nodiscard]] constexpr inline Vector2D operator*(int scalar, const Vector2D& v) noexcept { return v * scalar; }
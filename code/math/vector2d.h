#pragma once
#include <defs.h>

namespace rast {
template <typename T>
class Vector2D {
  // TODO(DAN): Consistent usage of Vector<T> or just "Vector"
 public:
  T x, y;
  Vector2D() = default;
  Vector2D(T x, T y) : x(x), y(y) {}

  // Allow explicit vector conversions
  template <typename U>
  explicit Vector2D(const Vector2D<U> &other) : x(other.x), y(other.y) {}

  inline bool hasNaN() { return isNaN(x) || isNaN(y); }

  // Explicitness favored over code size.

  inline Vector2D operator-() { return Vector2D<T>(-x, -y); }
  inline Vector2D operator+() { return Vector2D<T>(x, y); }

  inline Vector2D &operator+=(const Vector2D &rhs) {
    x += rhx.x;
    y += rhx.y;
    return *this;
  }

  inline Vector2D operator+(const Vector2D &rhs) {
    return Vector2D(x + rhs.x, y + rhs.y);
  }

  inline Vector2D &operator-=(const Vector2D &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  inline Vector2D operator-(const Vector2D &rhs) {
    return Vector2D<T>(x - rhs.x, y - rhs.y);
  }

  template <typename U>
  inline Vector2D operator*(U f) {
    return Vector2D<T>(x * f, y * f);
  }

  // Note(Dan): Division operator not implemented for types not float/double.

  inline Vector2D operator/(float f) {
    assert(f != 0);
    T temp = 1.0f / f;
    return Vector2D<T>(x * temp, y * temp);
  }

  inline Vector2D operator/(double f) {
    assert(f != 0);
    T temp = 1.0 / f;
    return Vector2D<T>(x * temp, y * temp);
  }

  static inline Vector2D zero() { return Vector2D(0, 0); }

  static inline Vector2D one() { return Vector2D(1, 1); }
};
using vec2f = Vector2D<float>;
using ivec2 = Vector2D<int>;
using uivec2 = Vector2D<uint32_t>;
}  // namespace rast

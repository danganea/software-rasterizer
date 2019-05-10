#pragma once
#include "defs.h"

namespace rast {

template <typename T>
class Vector3D {
  // TODO(DAN): Consistent usage of Vector<T> or just "Vector"
 public:
  T x, y, z;
  Vector3D() = default;
  Vector3D(T x, T y, T z) : x(x), y(y), z(z) {}

  // Allow explicit vector conversions
  template <typename U>
  explicit Vector3D(const Vector3D<U>& other)
      : x(other.x), y(other.y), z(other.z) {}

  inline bool hasNaN() { return isNaN(x) || isNaN(y) || isNaN(z); }

  // Explicitness favored over code size.

  inline Vector3D operator-() { return Vector3D<T>(-x, -y, -z); }
  inline Vector3D operator+() { return Vector3D<T>(x, y, z); }

  inline Vector3D& operator+=(const Vector3D& rhs) {
    x += rhx.x;
    y += rhx.y;
    z += rhx.z;
    return *this;
  }

  inline Vector3D operator+(const Vector& rhs) {
    return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
  }

  inline Vector3D& operator-=(const Vector3D& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  inline Vector3D operator-(const Vector3D& rhs) {
    return Vector3D<T>(x - rhs.x, y - rhs.y, z - rhs.z);
  }

  template <typename U>
  inline Vector3D operator*(U f) {
    return Vector3D<T>(x * f, y * f, z * f);
  }

  // Specialization for speed on float/double
  inline Vector3D operator/(float f) {
    assert(f != 0);
    T temp = 1.0f / f;
    return Vector3D<T>(x * temp, y * temp, z * temp);
  }

  inline Vector3D operator/(double f) {
    assert(f != 0);
    T temp = 1.0 / f;
    return Vector3D<T>(x * temp, y * temp, z * temp);
  }

  // TODO(Dan): C++ Concepts for more specifity on U
  template <typename U>
  inline Vector3D operator/(U d) {
    assert(d != 0);
    return Vector3D<T>(x / d, y / d, z / d);
  }
};

using vec3f = Vector3D<float>;
using ivec3 = Vector3D<int>;
using uivec3 = Vector3D<uint32_t>;
}  // namespace rast

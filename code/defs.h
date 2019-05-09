#pragma once
#include <cstdint>
#include <cassert>
#include <utility>

template <typename T>
inline bool isNaN(const T x) {
  return std::isnan(x);
}

template <>
inline bool isNaN(const int x) {
  return false;
}

template <>
inline bool isNaN(const uint32_t x) {
  return false;
}

// Force-Inline capabilities for MSVC and GCC (Clang ???)
// Use sparingly

#ifdef _MSC_VER
#define RAST_ALWAYS_INLINE __forceinline
#elif defined(__GNUC__)
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#else
#define ALWAYS_INLINE inline
#endif

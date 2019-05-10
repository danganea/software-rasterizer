#pragma once
#include <cstring>
#include <defs.h>
#include "vector2d.h"
namespace rast {

using Pixel = uint32_t;
// Class that models a SDL_Texture.
// Used to model the screen-space "surface" we write pixels to.
// Writing pixels is done from the bottom left origin.
class Surface {
 public:
  Pixel *data = nullptr;
  int width = 0, height = 0, pitch = 0;

  Surface() = default;

  void clear() { std::memset(data, 0, pitch * height); }

  Surface(int w, int h) : width(w), height(h), pitch(width * sizeof(Pixel)) {
    data = new Pixel[width * height];
    this->clear();
  }

  Surface(const Surface &other)
      : width(other.width), height(other.height), pitch(other.pitch) {
    data = new Pixel[width * height];
  }

  void swap(Surface &lhs, Surface &rhs) noexcept;

  Surface(Surface &&other) noexcept {
    using std::swap;
    swap(*this, other);
  }

  Surface &operator=(const Surface &rhs) {
    Surface tmp(rhs);
    swap(*this, tmp);
    return *this;
  }

  Surface &operator=(const Surface &&rhs) noexcept {
    using std::swap;
    Surface tmp(std::move(rhs));
    swap(*this, tmp);
    return *this;
  }

  ~Surface() {
    delete[] data;
    width = height = pitch = 0;
  }

  // Write a pixel. Coordinates start from bottom left.
  void writePixel(int x, int y, Pixel color) {
    assert(data != nullptr && x < width && x >= 0 && y < height && y >= 0);
    data[width * (height - 1) - (y * height) + x] = color;
  }

  // Uses the midpoint (similar to Brasenham) line algorithm to draw a line
  void drawLine(ivec2 p1, ivec2 p2) {
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < 10; ++j) {
        writePixel(320 + j, i, 0xFFFFFF00);
      }
    }
  }

 private:
};

void swap(Surface &lhs, Surface &rhs) noexcept {
  using std::swap;
  swap(lhs.height, rhs.height);
  swap(lhs.width, rhs.width);
  swap(lhs.pitch, rhs.pitch);
  swap(lhs.data, rhs.data);
}

}  // namespace rast

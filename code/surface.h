#pragma once
#include <defs.h>
#include <cstring>
#include "vector2d.h"

////Disable CPP Core Guideline errors intellisense picks up on
//#pragma warning(disable: 26451)

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
  // Summary of how this works:
  // If you write the line equation as y = dy / dx * x + b , you can rewrite it
  // in explicit form by multiplying by dx:
  // F(x,y) = dy * x - dx * y +  dx * b. If F(x,y) > 0 P = (x,y) is lower than
  // the line and otherwise it is higher than the line.
  // Given a starting pixel P(x,y) we want to choose whether to take E(x +1, y)
  // or NE(x+1, y+1). We do this by choosing the closest pixel to our line. To
  // do that, we consider that if our line goes through P, we want to know which
  // pixel is closest (along y axis) to the line at (x+1) along x axis. We
  // decide that by looking at P(x + 1/2, y + 1/2) = d. If d is positive (below
  // the line) the point halfway between E and NE is closer to NE, so we pick
  // NE. Otherwise we pick E. We can calculate d for the first point, then
  // incrementally calculate d_new from d_old. If we multiply d by 2 we will
  // always get integers, making us avoid FPO.
  void drawLine(ivec2 p1, ivec2 p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    // Initial d calcualted based on the fact that P(x,y) will be 0 for first
    // (x,y)
    int d = 2 * dy - dx;
    // Increment to d if last choice was E.
    int incrE = 2 * dy;
    // Increment to d if last choice was NE.
    int incrNE = 2 * (dy - dx);
    int x = p1.x;
    int y = p1.y;

    while (x < p2.x) {
      writePixel(x, y, 0xFFFFFFFF);
      if (d <= 0) {  // Choose NE
        d += incrE;
        ++x;
      } else { // Choose E
        d += incrNE;
        x++;
        y++;
      }
    }
  }
};

void swap(Surface &lhs, Surface &rhs) noexcept {
  using std::swap;
  swap(lhs.height, rhs.height);
  swap(lhs.width, rhs.width);
  swap(lhs.pitch, rhs.pitch);
  swap(lhs.data, rhs.data);
}

}  // namespace rast

#pragma once
#include "global.h"

class Image {
  vector<float> frame;
public:
  size_t w, h;
  Image(size_t w, size_t h): w(w), h(h), frame(w * h * 3) {}
  inline bool notInside(size_t i, size_t j) {
    return i < 0 || i >= w || j < 0 || j >= h;
  }

  inline bool notInside(Vector2i v) {
    return notInside(v[0], v[1]);
  }

  inline void set(const size_t i, const size_t j, const RGB& color) {
    if (notInside(i, j)) return;
    fset(i, j, color);
  }

  // fast set
  inline void fset(const size_t i, const size_t j, const RGB& color) {
    for (size_t k = 0; k < 3; ++k)
      frame[3 * (j * w + i) + k] = color[k];
  }

  void clear() {
    fill(frame.begin(), frame.end(), 0);
  }

  void display() {
    glDrawPixels(w, h, GL_RGB, GL_FLOAT, frame.data());
  }

  void line(Vector2i src, Vector2i dst, const RGB& color = white) {
    if (notInside(src) && notInside(dst))return;
    bool steep = false;
    if (abs(src[0] - dst[0]) < abs(src[1] - dst[1])) {
      steep = true;
      swap(src[0], src[1]);
      swap(dst[0], dst[1]);
    }
    if (src[0] > dst[0])
      swap(src, dst);

    int inc_y = src[1] > dst[1] ? -1 : 1;

    Vector2i d = dst - src;
    int derror2 = abs(d[1]) * 2;
    int error2 = 0;
    int y = src[1];

    for (int x = src[0]; x <= dst[0]; ++x) {
      if (steep)set(y, x, color);
      else set(x, y, color);
      error2 += derror2;
      if (error2 > d[0]) {
        y += inc_y;
        error2 -= d[0] * 2;
      }
    }
  }

  // this interface is slow
  void wireTriangle(const MatrixXi& t) {
    for (int i = 0; i < 3; ++i) {
      int j = (i + 1) % 3;
      line(t.col(i), t.col(j));
    }
  }

  inline void wireTriangle(const Vector2i t[3]) {
    for (size_t i = 0; i < 3; ++i) {
      size_t j = (i + 1) % 3;
      line(t[i], t[j]);
    }
  }
};

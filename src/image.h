#pragma once
#include <Eigen/Dense>
#include <algorithm>
#include <GLFW/glfw3.h>
typedef Eigen::Vector3f RGB;

class Image {
  std::vector<float> frame;
public:
  size_t w, h;
  Image(size_t w, size_t h): w(w), h(h), frame(w * h * 3) {}
  inline bool notInside(size_t i, size_t j) const {
    return i < 0 || i >= w || j < 0 || j >= h;
  }

  inline bool notInside (const Eigen::Ref<const Eigen::Vector2i> &v)const {
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
    std::fill(frame.begin(), frame.end(), 0);
  }

  void draw() const {
    glDrawPixels(w, h, GL_RGB, GL_FLOAT, frame.data());
  }

  void resize(size_t i, size_t j) {
    w = i;
    h = j;
    frame.resize(w, h);
  }
};
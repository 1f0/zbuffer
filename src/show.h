#pragma once
#include "global.h"
#include "wavefront.h"
#include "transform.h"

class Image {
  vector<float> frame;
public:
  size_t w, h;
  Image(size_t w, size_t h): w(w), h(h), frame(w * h * 3) {}
  inline void set(size_t i, size_t j, const RGB& color) {
    if (i >= 0 && i < w && j >= 0 && j < h)
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

  void wireTriangle(const Vector2i t[]) {
    for (int i = 0; i < 3; ++i) {
      int j = (i + 1) % 3;
      line(t[i], t[j]);
    }
  }
};

MatrixXi project(const MatrixXf& pts3, float w, float h) {
  float length = min(w, h) - 1;

  MatrixXi pts2(2, pts3.cols());
  for (size_t i = 0; i < pts3.cols(); ++i) {
    pts2.col(i) = (pts3.col(i).head(2) * 0.5 * length + 0.5 * Vector2f(w, h)).cast<int>();
  }
  return pts2;
}

void wireframe(const Mesh& mesh, Image& buffer) {
  const MatrixXi& tris = mesh.tris;

  // clipping
  MatrixXi pts2 = project(mesh.pts, buffer.w, buffer.h);

  for (int i = 0; i < tris.cols(); ++i) {
    Vector3i face = tris.col(i);
    Vector2i tri_pts[3];
    for (int j = 0; j < 3; ++j)
      tri_pts[j] = pts2.col(face(j));
    buffer.wireTriangle(tri_pts);
  }
}

void simpleScanLine(const Mesh& mesh, Image& buffer) {

}

void show(const Mesh& mesh) {
  static Image buffer(width, height);
  static Mesh tmp(mesh.tris);
  if (update) {
    tmp.pts = mesh.pts;//restore
    transform(tmp.pts);
  }
  buffer.clear();
  switch (mode) {
  case wire: wireframe(tmp, buffer); break;
  case simScan: simpleScanLine(tmp, buffer); break;
  }
  buffer.display();
}

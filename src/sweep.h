#pragma once
#include "global.h"
#include "image.h"

MatrixXi project(const MatrixXf& pts3, float w, float h) {
  float length = min(w, h) - 1;

  MatrixXi pts2(2, pts3.cols());
  for (size_t i = 0; i < pts3.cols(); ++i) {
    pts2.col(i) = (pts3.col(i).head(2) * 0.5 * length + 0.5 * Vector2f(w, h)).cast<int>();
  }
  return pts2;
}

class EdgeTable{

};

void lineSweep(const Mesh& mesh, Image& buffer) {
  MatrixXi pts2 = project(mesh.tris, buffer.w, buffer.h);
  for (size_t i = 0; i < buffer.h; ++i) {

  }
}
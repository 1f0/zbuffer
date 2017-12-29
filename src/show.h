#pragma once
#include "global.h"
#include "image.h"
#include "wavefront.h"
#include "transform.h"
#include "sweep.h"

MatrixXi project(const MatrixXf& pts3, float w, float h) {
  float length = min(w, h) - 1;

  MatrixXi pts2(2, pts3.cols());
  for (size_t i = 0; i < pts3.cols(); ++i) {
    pts2.col(i) = (pts3.col(i).head(2) * 0.5 * length + 0.5 * Vector2f(w, h)).cast<int>();
  }
  return pts2;
}

void wireframe(const Mesh& mesh, Image& buffer) {
  //TODO: clipping here
  MatrixXi pts2 = project(mesh.tris, buffer.w, buffer.h);
  Vector2i tris_pts[3];
  for (size_t i = 0; i < pts2.cols(); i=i+3) {
    for(size_t j=0; j<3; ++j)
      tris_pts[j] = pts2.col(i+j);
    buffer.wireTriangle(tris_pts);
  }
}

void show(const Mesh& mesh) {
  static Image buffer(width, height);
  static Mesh tmp = mesh;
  if (update) {
    tmp = mesh;//restore
    transform(tmp.tris);
  }
  buffer.clear();
  switch (mode) {
  case wire: wireframe(tmp, buffer); break;
  case sweep: lineSweep(tmp, buffer); break;
  }
  buffer.display();
}

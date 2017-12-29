#pragma once
#include "global.h"
#include "image.h"
#include "sweep.h"
#include "wavefront.h"
#include "transform.h"

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

  MatrixXi tri_pts(2, 3);
  for (int i = 0; i < tris.cols(); ++i) {
    Vector3i face = tris.col(i);
    for (int j = 0; j < 3; ++j)
      tri_pts.col(j) = pts2.col(face(j));
    buffer.wireTriangle(tri_pts);
  }
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
  case simScan: lineSweep(tmp, buffer); break;
  }
  buffer.display();
}

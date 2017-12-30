#pragma once
#include "global.h"
#include "image.h"
#include "wavefront.h"
#include "transform.h"
#include "sweep.h"

void wireframe(const Mesh& mesh, Image& buffer) {
  //TODO: clipping here
  MatrixXi pts2 = project(mesh.tris, buffer.w, buffer.h);
  for (size_t i = 0; i < pts2.cols(); i = i + 3) {
    Vector2i tris_pts[3];
    for (size_t j = 0; j < 3; ++j)
      tris_pts[j] = pts2.col(i + j);
    buffer.wireTriangle(tris_pts);
  }
}

void fillBox(const Mesh& mesh, Image& buffer) {
  MatrixXi pts2 = project(mesh.tris, buffer.w, buffer.h);
  RGB color(1, 1, 1);
  for (size_t i = 0; i < pts2.cols(); i = i + 3) {
    Vector2i tris_pts[3];
    for (size_t j = 0; j < 3; ++j)
      tris_pts[j] = pts2.col(i + j);
    Vector3f a = mesh.tris.col(i + 1) - mesh.tris.col(i);
    Vector3f b = mesh.tris.col(i + 2) - mesh.tris.col(i + 1);
    float intensity = a.cross(b).normalized().dot(light);
    if (intensity > 0) //back face culling
      buffer.triangle(tris_pts, color * intensity);
  }
}

void zFillBox(const Mesh& mesh, Image& buffer) {
  MatrixXi pts2 = project(mesh.tris, buffer.w, buffer.h);
  VectorXf zvalues = mesh.tris.row(2);
  MatrixXf zbuffer(buffer.w, buffer.h);
  zbuffer.fill(-numeric_limits<float>::max());

  RGB color(1, 1, 1);
  for (size_t i = 0; i < pts2.cols(); i = i + 3) {
    Vector2i tris_pts[3];
    for (size_t j = 0; j < 3; ++j) {
      tris_pts[j] = pts2.col(i + j);
    }
    Vector3f a = mesh.tris.col(i + 1) - mesh.tris.col(i);
    Vector3f b = mesh.tris.col(i + 2) - mesh.tris.col(i + 1);
    float intensity = a.cross(b).normalized().dot(light);
    if (intensity < 0)
      intensity *= -0.5;
    buffer.zb_triangle(tris_pts, zvalues.segment(i, 3).transpose(), zbuffer, color * intensity);
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
  case box: zFillBox(tmp, buffer); break;
  case sweep: lineSweep(tmp, buffer); break;
  }
  buffer.display();
}

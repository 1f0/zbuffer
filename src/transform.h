#pragma once
#include "global.h"

void normalize(MatrixXf& pts){
  int n = pts.cols();
  Vector3f center = pts * VectorXf::Ones(n) / n;
  VectorXf one = VectorXf::Ones(n);
  pts = pts.colwise() - center;

  VectorXf norms = pts.colwise().norm();
  float norm = norms.maxCoeff();
  pts = pts / (norm == 0?1:norm);
}

Matrix3f rotateMat(const Vector3f& u, float t){
  Matrix3f ux;
  ux << 0, -u[2], u[1],
        u[2], 0, -u[0],
        -u[1], u[0], 0;
  Matrix3f uxu = u*u.transpose();
  return cos(t)*Matrix3f::Identity(3,3)+sin(t)*ux+(1-cos(t))*uxu;
}

void transform(MatrixXf& pts){
  pts = rotateMat({1,0,0},lat)*rotateMat({0,1,0},lng)*pts;
  pts *= scale;
  update = false;
}
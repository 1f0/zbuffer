#pragma once
#include <vector>
#include <string>
#include <Eigen/Dense>
class Mesh {
public:
  Eigen::MatrixXf pts;
  std::vector<std::vector<size_t> >faces;
  Mesh(const std::string& filename, bool nomalized = false);
  void rasterize(const size_t w, const size_t h);
  // parameters: longitude latitude scale
  void transform(const Eigen::Vector3f& parameters);
};
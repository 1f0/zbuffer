#pragma once
#include "mesh.h"
#include "image.h"

class Edge {
public:
  int ymax;
  float x, dx;
  size_t poly_id;
  bool operator < (const Edge& rhs) const {
    if (x == rhs.x)
      return dx < rhs.dx;
    return x < rhs.x;
  }
};

class Polygon {
public:
  Eigen::Vector4f params;//Ax+By+Cz+D=0
  RGB color;
  bool in_flag;
};

class EdgeTable {
public:
  std::vector<std::vector<Edge> >list;
  void update(const Image& buffer, const Mesh& mesh);
  EdgeTable(const Image& buffer, const Mesh& mesh);
};


class PolygonTable {
public:
  std::vector<Polygon> list;
  void update(const Mesh& mesh);
  PolygonTable(const Mesh& mesh);
  bool& flag(size_t i) {return list[i].in_flag;}
};

void scan(const Mesh& mesh, const EdgeTable& et, PolygonTable& pt, Image& buffer);
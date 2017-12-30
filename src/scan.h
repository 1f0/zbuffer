#include "mesh.h"
#include "image.h"

class Edge{
  int ymax, x;
  float dx;
};

class Polygon{
  size_t id;
  Vector4f param;//parameters of plane eqaution
  RGB color;
  bool in_flag;
}

class EdgeTable{
  vector<vector<Edge> >list;
public:
  EdgeTable(const size_t w, const size_t h);
};

class PolygonTable{
  vector<vector<Edge> >list;
};

class ActiveEdgeList{

};

class InPolygonList{

};

void scan(const Mesh& mesh, Image& buffer){
  EdgeTable et();
  PolygonTable pt();

}
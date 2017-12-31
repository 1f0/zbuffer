#include "scan.h"
#include <utility>
#include <algorithm>
#include <Eigen/Geometry>
#include <map>
using namespace Eigen;
using namespace std;

class Edge {
public:
  int ymax, x;
  float dx;
  size_t poly_id;
  bool operator< (const Edge& rhs) const {
    if (x != rhs.x)
      return dx < rhs.dx;
    return x < rhs.x;
  }
};

class Polygon {
public:
  Eigen::Vector4f params;//Ax+By+Cz+D=0
  RGB color;
  bool in_flag;
}

class EdgeTable {
public:
  std::vector<std::vector<Edge> >list;
  EdgeTable(const Image& buffer, const Mesh& mesh);
};

EdgeTable::EdgeTable(const Image& buf, const Mesh& m): list(buf.h) {
  for (size_t i = 0; i < m.faces.size(); ++i) {
    const vector<Vector3f>& f = m.faces[i];
    for (size_t j = 0; j < f.size(); ++j) {
      Vector3f& a = m.pts[f[(j + 1) % f.size()]];
      Vector3f& b = m.pts[f[j]];
      // simple clipping
      bool a_not_inside = buf.notInside((int)a.x(), (int)a.y());
      bool b_not_inside = buf.notInside((int)b.x(), (int)b.y());
      if (a_not_inside || b_not_inside)continue;
      if (a.y() == b.y())continue; // skip horizonal line

      if (a.y() > b.y())
        swap(a, b);
      Edge e;
      e.ymax = (int)b.y();
      e.x = (int)a.x();
      e.dx = (b.x() - a.x()) / (b.y() - a.y());
      e.poly_id = i;

      list[(int)a.y()].push_back(e);
    }
  }
  for (auto& entry : list)
    sort(entry.begin(), entry.end());
}

class PolygonTable {
  vector<vector<Edge> >list;
  PolygonTable(const Mesh& mesh);
  bool& flag(size_t i) {return list[i].in_flag;}
};

PolygonTable::PolygonTable(const Mesh& m): list(m.faces.size()) {
  for (size_t i = 0; i < m.faces.size(); ++i) {
    const vector<Vector3f>& f = m.faces[i];
    Vector3f& a = m.pts[f[0]];
    Vector3f& b = m.pts[f[1]];
    Vector3f& c = m.pts[f[2]];

    Polygon& poly = list[i];
    Vector3f normal = (b - a).cross(c - b).normalized();
    //TODO: check this
    params << normal, -b.dot(normal);

    Vector3f light(0, 0, 1);
    poly.color = normal.dot(light) * RGB(1, 1, 1);
    if (poly.color < 0)poly.color *= -0.5; //turn to gray
  }
}

typedef vector<Edge> ActiveEdgeTable;
// typedef map<size_t, bool> InPolygonList;
typedef vector<size_t> InPolygonList;

inline void drawHorizonal(Image& buffer, size_t start, size_t end, size_t y, const RGB& color) {
  for (size_t i = start; i <= end; ++i)
    buffer.set(i, y, color);
}

void scan(const Mesh& mesh, Image& buffer) {
  EdgeTable et(buffer, mesh);
  PolygonTable pt(mesh);
  ActiveEdgeTable aet;

  for (size_t y = 0; y < et.size(); ++y) {
    if (et[y].empty() && aet.empty())continue;

    ActiveEdgeTable tmp;
    merge(aet.begin(), aet.end(), et[y].begin(), et[y].end(), tmp.begin());

    InPolygonList ipl;
    int max_id = -1;
    for (auto e : tmp) {
      if (max_id < e.poly_id) {
        ipl.push_back(e.poly_id);
        max_id = e.poly_id;
        pt.list[e.poly_id] = false;
      }
    }

    Edge e1 = tmp[0];

    for (size_t j = 1; e1.x < buffer.w;) {
      const Edge& e2 = tmp[j];
      // closest polygon
      size_t closest_id = 0;
      float closest_z = -numeric_limit<float>::max();
      Vector2f coord((e1.x + e2.x) / 2.0, y);
      for (auto poly_id : ipl) {
        const Vector4f& params = pt.list[poly_id].params;
        float z = -(coord.dot(params.head(2)) + params.w()) / params.z();
        if (z > closest_z) {
          z = closest_z;
          closest_id = poly_id;
        }
      }

      drawHorizonal(buffer, e1.x, e2.x, y, pt[closest_id].color);

      //update flag
      pt.flag(e1.poly_id) != pt.flag(e1.poly_id);

      e1 = e2;
    }

    remove_if(tmp.begin(), tmp.end(),
    [&](const Edge & a) {return a.ymax == y + 1},
    tmp.end());

    for (auto& e : tmp)
      e.x += e.dx;
    aet = tmp;

  }
}

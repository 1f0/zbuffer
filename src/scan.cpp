#include "scan.h"
#include <utility>
#include <algorithm>
#include <Eigen/Geometry>
#include <limits>
#include <iostream>
#include <set>
using namespace Eigen;
using namespace std;

EdgeTable::EdgeTable(const Image& buf, const Mesh& m): list(buf.h) {}

void EdgeTable::update(const Image& buf, const Mesh& m) {
  for (auto& entry : list)
    entry.clear();

  for (size_t i = 0; i < m.faces.size(); ++i) {
    const vector<size_t>& f = m.faces[i];
    for (size_t j = 0; j < f.size(); ++j) {
      Vector3f a = m.pts.col(f[(j + 1) % f.size()]);
      Vector3f b = m.pts.col(f[j]);
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

PolygonTable::PolygonTable(const Mesh& m): list(m.faces.size()) {
  calculateColor(m);
}

void PolygonTable::calculateColor(const Mesh& m) {
  for (size_t i = 0; i < m.faces.size(); ++i) {
    const vector<size_t>& f = m.faces[i];
    const Vector3f& a = m.pts.col(f[0]);
    const Vector3f& b = m.pts.col(f[1]);
    const Vector3f& c = m.pts.col(f[2]);

    Polygon& poly = list[i];
    Vector3f normal = (b - a).cross(c - b).normalized();
    poly.params << normal, 0;

    Vector3f light(0, 0, 1);
    float intensity = normal.dot(light);
    poly.color = intensity * RGB(1, 1, 1);
    if (intensity < 0) poly.color *= -0.4; //turn to gray
  }
}

void PolygonTable::updateAfterRasterize(const Mesh& m) {
  for (size_t i = 0; i < m.faces.size(); ++i) {
    const vector<size_t>& f = m.faces[i];
    const Vector3f& a = m.pts.col(f[0]);
    Polygon& poly = list[i];
    poly.params.w() = -a.dot(poly.params.head(3));
  }
}

typedef vector<Edge> ActiveEdgeTable;
typedef vector<size_t> InPolygonList;

inline void drawHorizonal(Image& buffer, size_t start, size_t end, size_t y, const RGB& color) {
  for (size_t i = start; i <= end; ++i)
    buffer.set(i, y, color);
}

void postprocess(ActiveEdgeTable& tmp, const size_t y) {
  tmp.erase(
  remove_if(tmp.begin(), tmp.end(), [&](const Edge & a) {return a.ymax == y + 1;}),
  tmp.end()
  );

  for (auto& e : tmp)
    e.x += e.dx;
  sort(tmp.begin(), tmp.end());
}


// don't using std::map or std::set to do this
// it is slow
void buildIPL(InPolygonList& ipl, const ActiveEdgeTable& tmp) {
  for (auto && e : tmp)
    ipl.push_back(e.poly_id);
  sort(ipl.begin(), ipl.end());
  auto last = unique(ipl.begin(), ipl.end());
  ipl.erase(last, ipl.end());

}

int findClosest(PolygonTable& pt, const InPolygonList& ipl, const Vector2f& coord) {
  // closest polygon
  int closest_id = -1;
  float closest_z = numeric_limits<float>::lowest();

  for (auto poly_id : ipl) {
    if (pt.flag(poly_id) == false)continue;
    const Vector4f& params = pt.list[poly_id].params;
    float z = -(coord.dot(params.head(2)) + params.w()) / params.z();
    if (z > closest_z) {
      closest_z = z;
      closest_id = poly_id;
    }
  }
  return closest_id;
}

void scan(const Mesh& mesh, const EdgeTable& et, PolygonTable& pt, Image& buffer) {
  ActiveEdgeTable aet;

  for (size_t y = 0; y < et.list.size(); ++y) {
    if (et.list[y].empty() && aet.empty())continue;

    ActiveEdgeTable tmp(et.list[y].size() + aet.size());
    merge(aet.begin(), aet.end(), et.list[y].begin(), et.list[y].end(), tmp.begin());

    InPolygonList ipl;
    buildIPL(ipl, tmp);

    for (auto && poly_id : ipl)
      pt.flag(poly_id) = false;

    Edge e1 = tmp[0];
    for (size_t j = 1; j < tmp.size(); ++j) {//for all edge pair
      //update flag
      pt.flag(e1.poly_id) = !pt.flag(e1.poly_id);
      const Edge& e2 = tmp[j];

      if (e1.x != e2.x) {
        int closest_id = findClosest(pt, ipl, Vector2f((e1.x + e2.x) / 2.0, y));
        if (closest_id >= 0)
          drawHorizonal(buffer, (int)e1.x, (int)e2.x, y, pt.list[closest_id].color);
      }
      e1 = e2;
    }

    postprocess(tmp, y);
    aet = tmp;
  }
}
#pragma once
#include "global.h"

class Image {
  vector<float> frame;
public:
  size_t w, h;
  Image(size_t w, size_t h): w(w), h(h), frame(w * h * 3) {}
  inline bool notInside(size_t i, size_t j) {
    return i < 0 || i >= w || j < 0 || j >= h;
  }

  inline bool notInside(Vector2i v) {
    return notInside(v[0], v[1]);
  }

  inline void set(const size_t i, const size_t j, const RGB& color = white) {
    if (notInside(i, j)) return;
    fset(i, j, color);
  }

  // fast set
  inline void fset(const size_t i, const size_t j, const RGB& color = white) {
    for (size_t k = 0; k < 3; ++k)
      frame[3 * (j * w + i) + k] = color[k];
  }

  void clear() {
    fill(frame.begin(), frame.end(), 0);
  }

  void display() {
    glDrawPixels(w, h, GL_RGB, GL_FLOAT, frame.data());
  }

  void line(Vector2i src, Vector2i dst) {
    if (notInside(src) && notInside(dst))return;
    bool steep = false;
    if (abs(src[0] - dst[0]) < abs(src[1] - dst[1])) {
      steep = true;
      swap(src[0], src[1]);
      swap(dst[0], dst[1]);
    }
    if (src[0] > dst[0])
      swap(src, dst);

    int inc_y = src[1] > dst[1] ? -1 : 1;

    Vector2i d = dst - src;
    int derror2 = abs(d[1]) * 2;
    int error2 = 0;
    int y = src[1];

    for (int x = src[0]; x <= dst[0]; ++x) {
      if (steep)set(y, x);
      else set(x, y);
      error2 += derror2;
      if (error2 > d[0]) {
        y += inc_y;
        error2 -= d[0] * 2;
      }
    }
  }

  // this interface is slow
  void wireTriangle(const MatrixXi& t) {
    for (int i = 0; i < 3; ++i) {
      int j = (i + 1) % 3;
      line(t.col(i), t.col(j));
    }
  }

  void wireTriangle(const Vector2i t[3]) {
    for (size_t i = 0; i < 3; ++i) {
      size_t j = (i + 1) % 3;
      line(t[i], t[j]);
    }
  }

  inline Vector3f barycentric(const Vector2i t[], const Vector2i& P) {
    Vector3f u(t[2][0]-t[0][0], t[1][0]-t[0][0], t[0][0]-P[0]);
    Vector3f v(t[2][1]-t[0][1], t[1][1]-t[0][1], t[0][1]-P[1]);
    Vector3f tmp = u.cross(v);
    if(abs(tmp[2])<1)return Vector3f(-1,1,1);//degenerate
    return Vector3f(1-(tmp.x()+tmp.y())/tmp.z(), tmp.y()/tmp.z(), tmp.x()/tmp.z());
  }

  void triangle(const Vector2i t[3]) {
    Vector2i boxmin(w-1, h-1);
    Vector2i boxmax(0, 0);
    Vector2i clamp = boxmin;
    for(size_t i=0;i<3;++i)
      for(size_t j=0;j<2;++j){
        boxmin[j] = max(0, min(boxmin[j], t[i][j]));
        boxmax[j] = min(clamp[j], max(boxmax[j], t[i][j]));
      }
    Vector2i P;
    RGB color(rand()%255, rand()%255, rand()%255);
    for(P.x()=boxmin.x();P.x()<boxmax.x();P.x()++)
      for(P.y()=boxmin.y();P.y()<boxmax.y();P.y()++){
        Vector3f bc = barycentric(t, P);
        if(bc.x()<0||bc.y()<0||bc.z()<0)continue;
        set(P.x(), P.y(), color);
      }
  }
};

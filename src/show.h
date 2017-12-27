#pragma once
#include "global.h"
#include "wavefront.h"

void triangle() {
  glBegin(GL_TRIANGLES);
  
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0.0, 1.0, 0.0);

  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(-1.0, -1.0, 0.0);
  
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(1.0, -1.0, 0.0);
  
  glEnd();  
}

class Image{
  vector<unsigned> frame;
  const size_t w,h;
public:
  Image(size_t w, size_t h):w(w),h(h),frame(w*h*3){}
  inline void set(size_t i, size_t j, RGB color){
    for(size_t k=0;k<3;++k)
        frame[3*(w*i+j)+k] = color[k];
  }

  void clear() {
    RGB black;
    black << 0,0,0;
    for(size_t i=0;i<w;++i)
      for(size_t j=0;j<h;++j)
        set(i, j, black);
  };

  void display() {
    glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_INT, frame.data());
  }

  inline void checkRange(const Vector2i& pt){
    assert(pt[0] >= 0 && pt[0] < w);
    assert(pt[1] >= 0 && pt[1] < h);
  }

  void line(Vector2i src, Vector2i dst) {
    checkRange(src);
    checkRange(dst);
    
    bool steep = false;
    if (abs(src[0] - dst[0]) < abs(src[1] - dst[1])){
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

    for (int x=src[0]; x <= dst[0]; ++x) {
      if(steep)set(x, y, white);
      else set(x, y, white);

      error2 += derror2;
      if (error2 > d[0]) {
        y += inc_y;
        error2 -= d[0] * 2;
      }
    }
  }
};

void show(Mesh& mesh) {
  MatrixXf& pts = mesh.pts;
  MatrixXi& tris = mesh.tris;
  // wireframe

  static Image buffer(width, height);
  buffer.clear();

  Vector2i src, dst;
  src << 0, 0;
  dst << width, height;

  buffer.line(src, dst);
  buffer.display();
}

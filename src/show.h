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
  vector<float> frame;
  const size_t w,h;
  inline int mod(int a, int b){
  	return (a % b + b)%b;
  }
public:
  Image(size_t w, size_t h):w(w),h(h),frame(w*h*3){}
  inline void set(size_t i, size_t j, const RGB& color){
  	i = mod(i, w);
  	j = mod(j, h);
    for(size_t k=0;k<3;++k)
      frame[3*(j*w+i)+k] = color[k];
  }

  void clear() {
  	for(size_t i = 0; i < frame.size(); ++i)
  		frame[i] = 0;
  }

  void display() {
    glDrawPixels(w, h, GL_RGB, GL_FLOAT, frame.data());
  }

  void line(Vector2i src, Vector2i dst) {
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
      if(steep)set(y, x, white);
      else set(x, y, white);

      error2 += derror2;
      if (error2 > d[0]) {
        y += inc_y;
        error2 -= d[0] * 2;
      }
    }
  }
};

void wireframe(const Mesh& mesh, Image& buffer) {
  const MatrixXf& pts = mesh.pts;
  const MatrixXi& tris = mesh.tris;
  for (int i=0;i<tris.cols();++i){
  	Vector3i face = tris.col(i);
  	for (int j=0;j<3;++j){
  	  Vector3f wh(width-1, height-1, 1);
  	  Vector3f vs = (pts.col(face(j))+Vector3f::Ones(3,1)).cwiseProduct(wh)/2;
  	  Vector3f vt = (pts.col(face((j+1)%3))+Vector3f::Ones(3,1)).cwiseProduct(wh)/2;
  	  buffer.line({(int)vs[0], (int)vs[1]}, {(int)vt[0], (int)vt[1]});
  	}
  }  
}

void show(const Mesh& mesh){
  static Image buffer(width, height);
  buffer.clear();
  wireframe(mesh, buffer);
  buffer.display();
}
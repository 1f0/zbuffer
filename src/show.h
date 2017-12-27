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

void show(Mesh& mesh) {
  // debug
  MatrixXf& pts = mesh.pts;
  MatrixXu& tris = mesh.tris;
  cout << pts << endl;
  cout << tris << endl;
}

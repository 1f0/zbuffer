#pragma once
#include "image.h"
#include "mesh.h"
#include <Eigen/Dense>
#include <iostream>

namespace Display {
using namespace std;
using namespace Eigen;

size_t width = 640;
size_t height = 480;
float lng = 0, lat = 0;
float scale = 0.8;
bool need_update = true;

void debugDisplay(const Mesh& mesh, Image& buffer) {
  const MatrixXf& pts = mesh.pts;
  const RGB white(1, 1, 1);
  for (size_t i = 0; i < pts.cols(); ++i)
    buffer.set(pts(0, i), pts(1, i), white);
}

void display(const Mesh& mesh) {
  static Image buffer(width, height);
  static Mesh cached_mesh = mesh;

  if (need_update) {
    cached_mesh = mesh;//restore
    cached_mesh.transform({lng, lat, scale});
    cached_mesh.rasterize(width, height);
    need_update = false;
  }

  buffer.clear();
  debugDisplay(cached_mesh, buffer);
  buffer.draw();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  const float roate_angle = 0.0625;
  const float scale_factor = 8 / 7.0;
  const float scale_limit = 128;

  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {
    case GLFW_KEY_ESCAPE: exit(0);
    case GLFW_KEY_W: lat -= roate_angle; break;
    case GLFW_KEY_S: lat += roate_angle; break;
    case GLFW_KEY_A: lng -= roate_angle; break;
    case GLFW_KEY_D: lng += roate_angle; break;
    case GLFW_KEY_Q: if (scale < scale_limit)scale *= scale_factor; break;
    case GLFW_KEY_E: if (scale > 1 / scale_limit)scale /= scale_factor; break;
    }
    cout << "scale: " << scale << " lng: " << lng << " lat: " << lat  << endl;
    need_update = true;
  }
}

void monitorFPS() {
  static double last_time = glfwGetTime();
  static int frame_count = 0;
  double current_time = glfwGetTime();
  ++frame_count;
  if (current_time - last_time >= 1) {
    cout << "fps: " << frame_count / (current_time - last_time) << endl;
    frame_count = 0;
    last_time = current_time;
  }
}
}


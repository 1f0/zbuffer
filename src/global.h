#pragma once
#include<iostream>
#include<Eigen/Dense>
#include<GLFW/glfw3.h>

using namespace std;
using namespace Eigen;

const size_t width = 640;
const size_t height = 480;

typedef Vector3f RGB;
const RGB white(1, 1, 1);
const RGB black(0, 0, 0);
const RGB red(1, 0, 0);
const RGB green(0, 1, 0);
const RGB blue(0, 0, 1);

enum Mode {wire, simScan, numOfMode};
Mode mode = wire;
bool update = true;

// longitude 经度, latidude 纬度
float lng = 0, lat = 0;
float scale = 0.5;
#include "mesh.h"
#include <fstream>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <locale>
using namespace std;
using namespace Eigen;

vector<vector<string> > parseText(const string& file) {
  ifstream input(file);
  if (input.fail())
    throw runtime_error("open " + file + " fail.");
  string buffer;

  vector<vector<string> >lines;
  for (size_t i = 1; getline(input, buffer); ++i) {
    istringstream iss(buffer);
    vector<string> tokens{//split
      istream_iterator<string>{iss},
      istream_iterator<string>{}
    };
    if (tokens.size() > 0) //trim blank and '/'
      lines.push_back(tokens);
  }
  return lines;
}

string trimSlash(string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
    return !(ch == '/');
  }).base(), s.end());
  return s;
}

void normalize(MatrixXf& pts) {
  int n = pts.cols();
  Vector3f center = pts * VectorXf::Ones(n) / n;
  VectorXf one = VectorXf::Ones(n);
  pts = pts.colwise() - center;

  VectorXf norms = pts.colwise().norm();
  float norm = norms.maxCoeff();
  pts = pts / (norm == 0 ? 1 : norm);
}

Matrix3f rotateMat(const Vector3f& u, float t) {
  Matrix3f ux;
  ux << 0, -u[2], u[1],
  u[2], 0, -u[0],
  -u[1], u[0], 0;
  Matrix3f uxu = u * u.transpose();
  return cos(t) * Matrix3f::Identity(3, 3) + sin(t) * ux + (1 - cos(t)) * uxu;
}

void transformPoints(MatrixXf& pts, const float lng, const float lat, const float scale) {
  const Vector3f xAxis(1, 0, 0);
  Vector3f x = rotateMat(xAxis, lng) * Vector3f(1, 0, 0);
  pts = rotateMat(x, lat) * rotateMat({0, 1, 0}, lng)*pts;
  pts *= scale;
}

void Mesh::transform(const Vector3f& p) {
  transformPoints(pts, p[0], p[1], p[2]);
}

void Mesh::rasterize(const size_t w, const size_t h) {
  float length = min((float)w, (float)h) - 1;
  float offset[3] = {w, h, 0};
  for (size_t i = 0; i < pts.cols(); ++i)
    for (size_t j = 0; j < 3; ++j) {
      pts(j, i) = (int)(pts(j, i) * length / 2 + offset[j] / 2);
    }
}

Mesh::Mesh(const std::string& filename, bool nomalized) {
  vector<vector<string> >lines = parseText(filename);
  vector<vector<string> >pts_txt, idxs_txt;
  for (size_t i = 0; i < lines.size(); ++i) {
    if (lines[i][0] == "v")
      pts_txt.push_back(lines[i]);
    if (lines[i][0] == "f") {
      idxs_txt.push_back(lines[i]);
      faces.push_back(vector<size_t>(lines[i].size() - 1));
    }
  }

  // get coordinates from text
  pts.resize(3, pts_txt.size());
  for (size_t i = 0; i < pts_txt.size(); ++i)
    for (size_t j = 0; j < 3; ++j)
      pts(j, i) = stof(pts_txt[i][j + 1]);

  for (size_t i = 0; i < idxs_txt.size(); ++i) {
    for (size_t j = 0; j < idxs_txt[i].size()-1; ++j) {
      faces[i][j] = stoi(trimSlash(idxs_txt[i][j + 1])) - 1;
    }
  }

  if (nomalized)normalize(pts);
}
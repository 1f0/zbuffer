#pragma once
#include "global.h"

vector<vector<string> > parse_text(const string& file){
  ifstream input(file);
  if(input.fail())
    throw runtime_error("open "+file+" fail.");
  string buffer;

  vector<vector<string> >lines;
  for (size_t i = 1; getline(input, buffer); ++i) {
    istringstream iss(buffer);
    vector<string> tokens{//split
      istream_iterator<string>{iss},
      istream_iterator<string>{}
    };
    if(tokens.size()>0)//trim blank
      lines.push_back(tokens);
  }
  return lines;
}

class Mesh{
public:
  MatrixXf pts;
  MatrixXu tris;
}

void readObj(const string& filename, Mesh& mesh) {
  MatrixXf& pts = mesh.pts;
  MatrixXu& tris = mesh.tris;

  vector<vector<string> >lines = parse_text(filename);
  vector<vector<string> >pts_txt, tris_txt;
  for (size_t i = 0; i < lines.size(); ++i){
    if (lines[i][0] == "v")
      pts_txt.push_back(line[i]);
    if (lines[i][0] == "f")
      tris_txt.push_back(line[i]);
  }
  // store value from text
  pts.resize(3, pts_txt.size());
  tris.resize(3, tris_txt.size());
  for(auto&& t:pts_txt)
    for(size_t j = 0; j < 3; ++j)
      pts(j, i) = stof(t[j]);
  for(auto&& t:tris_txt)
    for(size_t j = 0; j < 3; ++j)
      pts(j, i) = stof(t[j])-1;
}

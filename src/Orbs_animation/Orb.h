#pragma once
#include <Color.h>

#include "Vector3.h"

class Orb {
 public:
  void init(float _size, float _spd, float _hue);
  void move(float dt);
  void translate(Vector3 spd);
  void rotate(float r);
  void scale(float s);
  void render();

 private:
  Vector3 pos, spd;
  float size;
  Color c;
  float hue;
  void reset(float x, float y, float z);
};
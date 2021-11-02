#pragma once
#include <Color.h>

struct Vector3 {
  float x, y, z;
  void add(Vector3 v) {
    x += v.x;
    y += v.y;
    z += v.z;
  }
  void mult(Vector3 v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
  }
  void mult(float s) {
    x *= s;
    y *= s;
    z *= s;
  }
};

class Orb {
 public:
  void init(float _size, float _spd, float _hue);
  void move(float dt);
  void translate(float x, float y, float z);
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
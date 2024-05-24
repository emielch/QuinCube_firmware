#pragma once

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
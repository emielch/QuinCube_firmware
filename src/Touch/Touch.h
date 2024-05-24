#pragma once

class Touch {
 public:
  bool active;
  float pos, pow;
  float hue;
  Touch();
  void init(float _hue);
  void newPos(float _pos);
  float movedDist();

 private:
  float posDiff;
};
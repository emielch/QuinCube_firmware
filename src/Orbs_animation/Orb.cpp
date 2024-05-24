#include "Orb.h"

#include "LEDManager.h"
#include "config.h"

void Orb::init(float _size, float _spd, float _hue) {
  pos.x = random(1, CUBE_SIZE - 1);
  pos.y = random(1, CUBE_SIZE - 1);
  pos.z = random(1, CUBE_SIZE - 1);

  _spd *= 1000;
  spd.x = random(-_spd, _spd) / 1000.;
  spd.y = random(-_spd, _spd) / 1000.;
  spd.z = random(-_spd, _spd) / 1000.;

  size = _size / 2.;
  hue = _hue;
}

void Orb::move(float dt) {
  hue += 5 * dt;
  if (hue >= 360) hue -= 360;
  c.setHSB(hue, 100, 30);

  Vector3 dtSpd = spd;
  dtSpd.mult(dt);
  pos.add(dtSpd);

  /// check for reset
  if (pos.x > CUBE_SIZE + size) {
    reset(-size, -1, -1);
  } else if (pos.x < -size) {
    reset(CUBE_SIZE + size, -1, -1);
  } else if (pos.y > CUBE_SIZE + size) {
    reset(-1, -size, -1);
  } else if (pos.y < -size) {
    reset(-1, CUBE_SIZE + size, -1);
  } else if (pos.z > CUBE_SIZE + size) {
    reset(-1, -1, -size);
  } else if (pos.z < -size) {
    reset(-1, -1, CUBE_SIZE + size);
  }
}

void Orb::reset(float x, float y, float z) {
  if (x == -1) x = random(1, CUBE_SIZE - 1);
  if (y == -1) y = random(1, CUBE_SIZE - 1);
  if (z == -1) z = random(1, CUBE_SIZE - 1);
  pos = {x, y, z};
}

void Orb::translate(Vector3 spd) {
  pos.add(spd);
}

void Orb::rotate(float r) {
  float xNew = (pos.x - CUBE_SIZE * 0.5) * cos(r) - (pos.z - CUBE_SIZE * 0.5) * sin(r);
  float zNew = (pos.x - CUBE_SIZE * 0.5) * sin(r) + (pos.z - CUBE_SIZE * 0.5) * cos(r);

  pos.x = xNew + CUBE_SIZE * 0.5;
  pos.z = zNew + CUBE_SIZE * 0.5;
}

void Orb::scale(float s) {
  s+=1;
  pos.x = (pos.x - CUBE_SIZE * 0.5) * s + CUBE_SIZE * 0.5;
  pos.z = (pos.z - CUBE_SIZE * 0.5) * s + CUBE_SIZE * 0.5;
  size *= s;
  spd.x *= s;
  spd.y *= s;
  spd.z *= s;
}

void Orb::render() {
  int x1 = constrain(pos.x - size, 0, CUBE_SIZE - 1);
  int x2 = constrain(pos.x + size, 0, CUBE_SIZE - 1);
  int y1 = constrain(pos.y - size, 0, CUBE_SIZE - 1);
  int y2 = constrain(pos.y + size, 0, CUBE_SIZE - 1);
  int z1 = constrain(pos.z - size, 0, CUBE_SIZE - 1);
  int z2 = constrain(pos.z + size, 0, CUBE_SIZE - 1);

  for (int x = x1; x <= x2; x++) {
    for (int y = y1; y <= y2; y++) {
      for (int z = z1; z <= z2; z++) {
        float Dx = x - pos.x;
        float Dy = y - pos.y;
        float Dz = z - pos.z;
        float dist = sqrt(Dx * Dx + Dy * Dy + Dz * Dz);

        float bri = size - dist;
        bri = bri / size;
        if (bri <= 0) continue;

        Color pixCol = ledManager.getVoxel(x, y, z);
        pixCol.add(c, bri);

        ledManager.setVoxel(x, y, z, pixCol.red(), pixCol.green(), pixCol.blue());
      }
    }
  }
}

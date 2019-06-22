#include <stdio.h>
#include <cmath>

class vector {
  private:
    float _x, _y;
  public:
    vector(float x, float y) : _x(x), _y(y) { }
    float  get_x() const { return _x; }
    float  get_y() const { return _y; }
    float  get_length() const { return sqrtf(_x * _x + _y * _y); }
    // Some operator overloading for vector computations.
};

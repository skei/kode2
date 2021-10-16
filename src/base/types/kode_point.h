#ifndef kode_point_included
#define kode_point_included
//----------------------------------------------------------------------

template <typename T>
class KODE_Point {

public:

  union {
    struct { T x,y; };
    struct { T w,h; };
    struct { T hor,vert; };
  };

public:

  KODE_Point() {
    x = 0;
    y = 0;
  }

  KODE_Point(T v) {
    x = v;
    y = v;
  }

  KODE_Point(T x_, T y_) {
    x = x_;
    y = y_;
  }

};

//----------------------------------------------------------------------

typedef KODE_Point<float>   KODE_FPoint;
typedef KODE_Point<int32_t> KODE_IPoint;
//----------------------------------------------------------------------
#endif

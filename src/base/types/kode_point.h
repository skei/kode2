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

  KODE_Point() {}
  KODE_Point(T v) {}
  KODE_Point(T x, T y) {}

};

//----------------------------------------------------------------------

typedef KODE_Point<float>   KODE_FPoint;
typedef KODE_Point<int32_t> KODE_IPoint;
//----------------------------------------------------------------------
#endif

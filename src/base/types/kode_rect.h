#ifndef kode_rect_included
#define kode_rect_included
//----------------------------------------------------------------------

#include "kode.h"
//#include "base/types/kode_point.h"

template <typename T>
class KODE_Rect {

//------------------------------
public:
//------------------------------

  union {
    struct { T x,y,w,h; };
    struct { T left,top,right,bottom; };
  };

//------------------------------
public:
//------------------------------

  KODE_Rect() {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
  }

  //KODE_Rect(T _v) {
  //  x = _v;
  //  y = _v;
  //  w = _v;
  //  h = _v;
  //}

  KODE_Rect(T _w, T _h) {
    x = 0;
    y = 0;
    w = _w;
    h = _h;
  }

  KODE_Rect(T _x, T _y, T _w, T _h) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;
  }

  //KODE_Rect(KODE_Point<T> APos) {}
  //KODE_Rect(KODE_Point<T> APos, KODE_Point<T> ASize) {}

//------------------------------
public:
//------------------------------

  KODE_Rect<T>& operator += (KODE_Rect<T> R) {
    add(R);
    return *this;
  }

  KODE_Rect<T>& operator -= (KODE_Rect<T> R) {
    sub(R);
    return *this;
  }

//------------------------------
public:
//------------------------------

  void setX1(T _x1) { w -= (_x1 - x);   x = _x1; }
  void setY1(T _y1) { h -= (_y1 - y);   y = _y1; }
  void setX2(T _x2) { w += (_x2 - x - w); }
  void setY2(T _y2) { h += (_y2 - y - h); }

  T x1() { return x; }
  T y1() { return y; }
  T x2() { return x + w; }
  T y2() { return y + h; }

  //----------

  void setPos(T AXpos, T AYpos) {
    x = AXpos;
    y = AYpos;
  }

  //----------

  void setSize(T AWidth, T AHeight) {
    w = AWidth;
    h = AHeight;
  }

  //----------

  void scale(T AScale) {
    x *= AScale;
    y *= AScale;
    w *= AScale;
    h *= AScale;
  }


  //----------

  void add(T AValue) {
    x += AValue;
    y += AValue;
    w += AValue;
    h += AValue;
  }

  void add(T ax, T ay, T aw, T ah) {
    x += ax;
    y += ay;
    w += aw;
    h += ah;
  }

  void add(KODE_Rect<T> R) {
    x += R.x;
    y += R.y;
    w += R.w;
    h += R.h;
  }

  void sub(T AValue) {
    x -= AValue;
    y -= AValue;
    w -= AValue;
    h -= AValue;
  }

  void sub(T ax, T ay, T aw, T ah) {
    x -= ax;
    y -= ay;
    w -= aw;
    h -= ah;
  }

  void sub(KODE_Rect<T> R) {
    x -= R.x;
    y -= R.y;
    w -= R.w;
    h -= R.h;
  }

  void grow(T AValue) {
    x -= AValue;
    y -= AValue;
    w += (AValue + AValue);
    h += (AValue + AValue);
  }

  void grow(T ax, T ay, T aw, T ah) {
    x -= ax;
    y -= ay;
    w += (ax + aw);
    h += (ay + ah);
  }

  void grow(KODE_Rect<T> R) {
    x -= R.x;
    y -= R.y;
    w += (R.x + R.w);
    h += (R.y + R.h);
  }

  void shrink(T AValue) {
    x += AValue;
    y += AValue;
    w -= (AValue + AValue);
    h -= (AValue + AValue);
  }

  void shrink(T ax, T ay, T aw, T ah) {
    x += ax;
    y += ay;
    w -= (ax + aw);
    h -= (ay + ah);
  }

  void shrink(KODE_Rect<T> R) {
    x += R.x;
    y += R.y;
    w -= (R.x + R.w);
    h -= (R.y + R.h);
  }

  void addLeft(T ASize)    { x -= ASize; w += ASize; }
  void addRight(T ASize)   { w += ASize; }
  void addTop(T ASize)     { y -= ASize; h += ASize; }
  void addBottom(T ASize)  { h += ASize; }

  void removeLeft(T ASize)    { x += ASize; w -= ASize; }
  void removeRight(T ASize)   { w -= ASize; }
  void removeTop(T ASize)     { y += ASize; h -= ASize; }
  void removeBottom(T ASize)  { h -= ASize; }

//------------------------------
public:
//------------------------------

  bool isEmpty() {
    if (w <= 0.0f) return true;
    if (h <= 0.0f) return true;
    return false;
  }

  //----------

  bool isNotEmpty() {
    if ((w > 0.0f) && (h > 0.0f)) return true;
    return false;
  }

  //----------

  /*
   _____
  |   _ |__
  |  |  |  |
  |__|__|  |
     |_____|


         _____
      _ |__   |
     |  |  |  |
     |  |__|__|
     |_____|


  */

  //----------

  // intersection
  // largest x
  // smallest x2

  void overlap(KODE_Rect<T> R) {
    if ( R.x1() > x1() ) setX1( R.x1() );
    if ( R.x2() < x2() ) setX2( R.x2() );
    if ( R.y1() > y1() ) setY1( R.y1() );
    if ( R.y2() < y2() ) setY2( R.y2() );
  }

//  void overlap(KODE_Rect<T> R) {
//    if ( R.x1() > x1() ) setX1( R.x1() );
//    if ( R.y1() > y1() ) setY1( R.y1() );
//    if ( R.x2() < x2() ) setX2( R.x2() );
//    if ( R.y2() < y2() ) setX2( R.y2() );
//  }

  //----------

  // union / combine
  // smallest x
  // largest x2

  //----------

  // touches (intersects)
  // false:
  // x > r.x2
  // x2 < r.x

//  bool touches(KODE_Rect<T> R) {
//    if ( x    > R.x2() ) return false;
//    if ( x2() < R.x    ) return false;
//    if ( y    > R.y2() ) return false;
//    if ( y2() < R.y    ) return false;
//    return true;
//  }

  bool touches(KODE_Rect<T> R) {
    if (R.x1() > x2() ) return false; // too far right
    if (R.y1() > y2() ) return false; // too far down
    if (R.x2() < x1() ) return false; // too far left
    if (R.y2() < y1() ) return false; // too far up
    return true;
  }

  //----------

  // is_fully_inside
  // false
  // x < r.x
  // x2 > r.x2

  //----------

  // fully_contains
  // false
  // x > ax
  // x2 < ax2

  //----------

  // returns true if APoint is inside
  // if widget.contains(mousex,mousey)

  bool contains(T xpos, T ypos) {
    if (xpos < x   ) return false;
    if (xpos > x2()) return false;
    if (ypos < y   ) return false;
    if (ypos > y2()) return false;
    return true;
  }

  //----------

  // combine rects
  // (so that both of them fits inside)
  // parent.combine(child)

  void combine(KODE_Rect<T> R) {
    //printf("combine %.0f,%.0f,%.0f,%.0f R %.0f,%.0f,%.0f,%.0f -> ",x,y,w,h,R.x,R.y,R.w,R.h);
    if ( R.x1() < x1() ) setX1( R.x1() );
    if ( R.x2() > x2() ) setX2( R.x2() );
    if ( R.y1() < y1() ) setY1( R.y1() );
    if ( R.y2() > y2() ) setY2( R.y2() );
    //printf("%.0f,%.0f,%.0f,%.0f\n",x,y,w,h);
  }

  //----------

  // remove ARect (so that it lies outside)

  void remove(KODE_Rect<T> ARect)   {
  }


//  // returns true if any of the edges intersects
//  // if widget.intersect(updaterect)
//
//  // ax2 < x
//  // ax > x2
//
//  bool touches(KODE_Rect<T> ARect) {
//    if (ARect.x     > x2()) return false;
//    if (ARect.x2()  < x   ) return false;
//    if (ARect.y     > y2()) return false;
//    if (ARect.y2()  < y   ) return false;
//    return true;
//  }
//
//  //----------
//
//  // largest x
//  // smallest x2
//
//  void intersection(KODE_Rect<T> ARect) {
//    if (ARect.x2()  > x2()) x2(ARect.x2());
//  }
//
//
//  //----------
//
//  void shrink(KODE_Rect<T> ARect) {
//  }
//
//  void grow(KODE_Rect<T> ARect) {
//  }
//
//  //----------
//
//  // returns true if ARect is completely inside this
//  // (no clipping needed, opacity)
//  // if widget.inside(cliprect)
//
//  /*
//  bool inside(KODE_Rect<T> ARect) {
//    if (ARect.x     < x   ) return false;
//    if (ARect.x2()  > x2()) return false;
//    if (ARect.y     < y   ) return false;
//    if (ARect.y2()  > y2()) return false;
//    return true;
//  }
//  */
//
//  //----------
//

};

//----------

typedef KODE_Rect<float>    KODE_FRect;
typedef KODE_Rect<int32_t>  KODE_IRect;

//----------------------------------------------------------------------
#endif














//class KODE_Rect {
//
//public:
//
//  union {
//    struct { float x,y,w,h; };
//    struct { float left,top,right,bottom; };
//  };
//
//public:
//
//  KODE_Rect() {}
//  KODE_Rect(float v) {}
//  KODE_Rect(float w, float h) {}
//  KODE_Rect(float x, float y, float w, float h) {}
//  KODE_Rect(KODE_Point APos) {}
//  KODE_Rect(KODE_Point APos,KODE_Point ASize) {}
//
//public:
//
//  void x1(float _x1) { w -= (_x1 - x); x = _x1; }
//  void y1(float _y1) { h -= (_y1 - y); y = _y1; }
//  float x1() { return x; }
//  float y1() { return y; }
//
//  void x2(float _x2) { w += (_x2 - x - w); }
//  void y2(float _y2) { h += (_y2 - y - h); }
//  float x2() { return x + w; }
//  float y2() { return y + h; }
//
//  void setPos(float AXpos, float AYpos) {
//    x = AXpos;
//    y = AYpos;
//  }
//
//  void setSize(float AWidth, float AHeight) {
//    w = AWidth;
//    h = AHeight;
//  }
//
//public:
//
//  // returns true if any of the edges intersects
//
//  bool intersect(KODE_Rect ARect) {
//    return false;
//  }
//
//  // returns true if we are completely inside ARect
//
//  bool inside(KODE_Rect ARect) {
//    return false;
//  }
//
//  // returns true if APoint is inside
//
//  bool contains(KODE_Point APoint) {
//    return false;
//  }
//
//  // combine rects (so that both of them fits inside)
//
//  void combine(KODE_Rect ARect) {
//  }
//
//  // remove ARect (so that it lies outside)
//
//  void remove(KODE_Rect ARect)   {
//  }
//
//};


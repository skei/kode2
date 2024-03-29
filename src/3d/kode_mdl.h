#ifndef kode_mdl_included
#define kode_mdl_included
//----------------------------------------------------------------------

#include "kode.h"

struct KODE_MdlHeader {
  int     ident;                // magic number: "IDPO" (1330660425)
  int     version;              // version: 6
  vec3_t  scale;                // scale factor
  vec3_t  translate;            // translation vector
  float   boundingradius;       // bounding sphere radius
  vec3_t  eyeposition;          // eyes' position
  int     num_skins;            // number of textures
  int     skinwidth;            // texture width
  int     skinheight;           // texture height
  int     num_verts;            // number of vertices
  int     num_tris;             // number of triangles
  int     num_frames;           // number of frames
  int     synctype;             // 0 = synchron, 1 = random
  int     flags;                // state flag
  float   size;
};

struct KODE_MdlSkin {
  int       group = 0;          // 0 = single, 1 = group
  uint8_t*  data  = KODE_NULL;  // texture data
};

struct KODE_MdlGroupSkin {
  int       group = 1;          // 1 = group
  int       nb    = 0;          // number of pics
  float*    time  = KODE_NULL;  // time duration for each pic
  uint8_t** data  = KODE_NULL;  // texture data
};

struct KODE_MdlTexCoord {
  int onseam  = 0;
  int s       = 0;
  int t       = 0;
};

struct KODE_MdlTriangle {
  int facesfront  = 0;          // 0 = backface, 1 = frontface
  int vertex[3]   = {0};        // vertex indices
};

struct KODE_MdlVertex {
  uint8_t v[3]        = {0};
  uint8_t normalIndex = 0;
};

struct KODE_MdlSimpleFrame {
  mdl_vertex_t  bboxmin;              // bouding box min
  mdl_vertex_t  bboxmax;              // bouding box max
  char          name[16] = {0};
  mdl_vertex_t* verts    = KODE_NULL; // vertex list of the frame
};

struct KODE_MdlFrame {
  int               type  = 0;  // 0 = simple, !0 = group
  mdl_simpleframe_t frame;      // this program can't read models composed of group frames!
};

struct KODE_MdlGroupFrame {
  int                 type    = 0;          // !0 = group
  mdl_vertex_t        min;                  // min pos in all simple frames
  mdl_vertex_t        max;                  // max pos in all simple frames
  float*              time    = KODE_NULL;  // time duration for each frame
  mdl_simpleframe_t*  frames  = KODE_NULL;  // simple frame list
};

struct KODE_MdlModel {
  mdl_header_t*   header    = KODE_NULL;
  mdl_skin_t*     skins     = KODE_NULL;
  mdl_texcoord_t* texcoords = KODE_NULL;
  mdl_triangle_t* triangles = KODE_NULL;
  mdl_frame_t*    frames    = KODE_NULL;
};

//----------------------------------------------------------------------
#endif

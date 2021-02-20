#ifndef kode_3d_included
#define kode_3d_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gfx/kode_bitmap.h"

//----------------------------------------------------------------------

struct KODE_3D_Vertex {
  uint32_t  flags = 0;
  vec3_t    pos;
  vec3_t    normal;
};

struct KODE_3D_TexCoord {
  uint32_t  flags = 0;
  float     u = 0.0f;
  float     v = 0.0f;
};

struct KODE_3D_Morph {
  KODE_3D_Vertex* vertices;
};

struct KODE_3D_Triangle {
  uint32_t  flags = 0;
  uint32_t  v1    = 0;
  uint32_t  v2    = 0;
  uint32_t  v3    = 0;
};

struct KODE_3D_Material {
  uint32_t      flags         = 0;
  uint32_t      num_textures  = 0;
  KODE_Bitmap** textures      = KODE_NULL;
};

struct KODE_3D_Mesh {
  uint32_t          num_vertices  = 0;
  uint32_t          num_triangles = 0;
  uint32_t          num_morphs    = 0;
  KODE_3D_Vertex*   vertices      = KODE_NULL;
  KODE_3D_TexCoord* texcoords     = KODE_NULL;
  KODE_3D_Triangle* triangles     = KODE_NULL;
  KODE_3D_Morph*    morphs        = KODE_NULL;
};

struct KODE_3D_Model {
  KODE_3D_Mesh*     mesh      = KODE_NULL;;
  KODE_3D_Material* material  = KODE_NULL;;
};

//----------------------------------------------------------------------











//struct mdl_header_t {
//  int     ident;              // magic number: "IDPO" // 1330660425
//  int     version;            // version: 6
//  vec3_t  scale;              // scale factor
//  vec3_t  translate;          // translation vector
//  float   boundingradius;
//  vec3_t  eyeposition;        // eyes' position
//  int     num_skins;          // number of textures
//  int     skinwidth;          // texture width
//  int     skinheight;         // texture height
//  int     num_verts;          // number of vertices
//  int     num_tris;           // number of triangles
//  int     num_frames;         // number of frames
//  int     synctype;           // 0 = synchron, 1 = random
//  int     flags;              // state flag
//  float   size;
//};

//struct mdl_skin_t {
//  int       group;            // 0 = single, 1 = group
//  uint8_t*  data;             // texture data
//};

//struct mdl_groupskin_t {
//  int       group;            /* 1 = group */
//  int       nb;               /* number of pics */
//  float*    time;             /* time duration for each pic */
//  uint8_t** data;             /* texture data */
//};



//struct mdl_texcoord_t {
//  int onseam;
//  int s;
//  int t;
//};

//struct mdl_triangle_t {
//  int facesfront;             /* 0 = backface, 1 = frontface */
//  int vertex[3];              /* vertex indices */
//};


//struct mdl_vertex_t {
//  uint8_t v[3];
//  uint8_t normalIndex;
//};

//struct mdl_simpleframe_t {
//  mdl_vertex_t  bboxmin;      /* bouding box min */
//  mdl_vertex_t  bboxmax;      /* bouding box max */
//  char          name[16];
//  mdl_vertex_t* verts;        /* vertex list of the frame */
//};

//struct mdl_frame_t {
//  int               type;     /* 0 = simple, !0 = group */
//  mdl_simpleframe_t frame;    /* this program can't read models composed of group frames! */
//};

//struct mdl_groupframe_t {
//  int                 type;   /* !0 = group */
//  mdl_vertex_t        min;    /* min pos in all simple frames */
//  mdl_vertex_t        max;    /* max pos in all simple frames */
//  float*              time;   /* time duration for each frame */
//  mdl_simpleframe_t*  frames; /* simple frame list */
//};

//struct mdl_model_t {
//  mdl_header_t*   header;
//  mdl_skin_t*     skins;
//  mdl_texcoord_t* texcoords;
//  mdl_triangle_t* triangles;
//  mdl_frame_t*    frames;
//};



//----------------------------------------------------------------------
#endif

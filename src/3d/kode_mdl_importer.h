#ifndef kode_mdl_importer_included
#define kode_mdl_importer_included
//----------------------------------------------------------------------

#include "kode.h"
#include "3d/kode_3d.h"

//----------------------------------------------------------------------

struct mdl_header_t {
  int     ident;              // magic number: "IDPO" // 1330660425
  int     version;            // version: 6
  vec3_t  scale;              // scale factor
  vec3_t  translate;          // translation vector
  float   boundingradius;
  vec3_t  eyeposition;        // eyes' position
  int     num_skins;          // number of textures
  int     skinwidth;          // texture width
  int     skinheight;         // texture height
  int     num_verts;          // number of vertices
  int     num_tris;           // number of triangles
  int     num_frames;         // number of frames
  int     synctype;           // 0 = synchron, 1 = random
  int     flags;              // state flag
  float   size;
};

struct mdl_skin_t {
  int       group;            // 0 = single, 1 = group
  uint8_t*  data;             // texture data
};

struct mdl_groupskin_t {
  int       group;            /* 1 = group */
  int       nb;               /* number of pics */
  float*    time;             /* time duration for each pic */
  uint8_t** data;             /* texture data */
};

struct mdl_texcoord_t {
  int onseam;
  int s;
  int t;
};

struct mdl_triangle_t {
  int facesfront;             /* 0 = backface, 1 = frontface */
  int vertex[3];              /* vertex indices */
};

struct mdl_vertex_t {
  uint8_t v[3];
  uint8_t normalIndex;
};

struct mdl_simpleframe_t {
  mdl_vertex_t  bboxmin;      /* bouding box min */
  mdl_vertex_t  bboxmax;      /* bouding box max */
  char          name[16];
  mdl_vertex_t* verts;        /* vertex list of the frame */
};

struct mdl_frame_t {
  int               type;     /* 0 = simple, !0 = group */
  mdl_simpleframe_t frame;    /* this program can't read models composed of group frames! */
};

struct mdl_groupframe_t {
  int                 type;   /* !0 = group */
  mdl_vertex_t        min;    /* min pos in all simple frames */
  mdl_vertex_t        max;    /* max pos in all simple frames */
  float*              time;   /* time duration for each frame */
  mdl_simpleframe_t*  frames; /* simple frame list */
};

struct mdl_model_t {
  mdl_header_t*   header;
  mdl_skin_t*     skins;
  mdl_texcoord_t* texcoords;
  mdl_triangle_t* triangles;
  mdl_frame_t*    frames;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class KODE_MdlImporter {

//------------------------------
private:
//------------------------------

  KODE_3D_Mesh* MMesh         = KODE_NULL;
  KODE_3D_Mesh* MMaterial     = KODE_NULL;
  uint32_t      MNumTextures  = 0;
  uint32_t**    MTextures     = KODE_NULL;

//------------------------------
private:
//------------------------------

  FILE* fp = KODE_NULL;

  mdl_header_t    mdl_header;
  mdl_skin_t*     mdl_skins     = KODE_NULL;
  mdl_frame_t*    mdl_frames    = KODE_NULL;
  mdl_texcoord_t* mdl_texcoords = KODE_NULL;
  mdl_triangle_t* mdl_triangles = KODE_NULL;


//------------------------------
public:
//------------------------------

  bool read(const char* AFilename) {
    KODE_DPrint("Reading '%s'\n",AFilename);
    fp = fopen(AFilename,"rb");
    if (!fp) {
      KODE_DPrint("Error. Couldn't open '%s'\n",AFilename);
      return false;
    }

    read_mdl_header();

    if ((mdl_header.ident != 1330660425) || (mdl_header.version != 6)) {
      fprintf(stderr, "Error: bad version or identifier\n");
      fclose(fp);
      return false;
    }

    read_mdl_skins();
    read_mdl_texcoords();
    read_mdl_triangles();
    read_mdl_frames();

    fclose (fp);

    return true;
  }

  //----------

  bool convert() {
    return false;
  }

  //----------

  void cleanup() {

    for (int i=0; i<mdl_header.num_skins; i++) { KODE_Free(mdl_skins[i].data); }
    for (int i=0; i<mdl_header.num_frames; ++i) { KODE_Free(mdl_frames[i].frame.verts); }

    KODE_Free(mdl_skins);
    KODE_Free(mdl_texcoords);
    KODE_Free(mdl_triangles);
    KODE_Free(mdl_frames);
    //KODE_Free(mdl_header);

  }

//------------------------------
public:
//------------------------------

  void print_mdl_header() {
    KODE_DPrint("header:\n");
    KODE_DPrint("  ident           %i\n",mdl_header.ident);
    KODE_DPrint("  scale           %.3f, %.3f, %.3f\n",mdl_header.scale.x, mdl_header.scale.y, mdl_header.scale.z);
    KODE_DPrint("  translate       %.3f, %.3f, %.3f\n",mdl_header.translate.x, mdl_header.translate.y, mdl_header.translate.z);
    KODE_DPrint("  bounding_radius %.3f\n",mdl_header.boundingradius);
    KODE_DPrint("  eye_position    %.3f, %.3f, %.3f\n",mdl_header.eyeposition.x,mdl_header.eyeposition.y,mdl_header.eyeposition.z);
    KODE_DPrint("  num_skins       %i\n",mdl_header.num_skins);
    KODE_DPrint("  skin_width      %i\n",mdl_header.skinwidth);
    KODE_DPrint("  skin_height     %i\n",mdl_header.skinheight);
    KODE_DPrint("  num_verts       %i\n",mdl_header.num_verts);
    KODE_DPrint("  num_tris        %i\n",mdl_header.num_tris);
    KODE_DPrint("  num_frames      %i\n",mdl_header.num_frames);
    KODE_DPrint("  sync_type       %i\n",mdl_header.synctype);
    KODE_DPrint("  flags           %i\n",mdl_header.flags);
    KODE_DPrint("  size            %.3f\n",mdl_header.size);
  }

//------------------------------
private:
//------------------------------

  uint8_t read_byte(void) {
    uint8_t b;
    fread(&b,sizeof(uint8_t),1,fp);
    return b;
  }

  //----------

  int read_int(void) {
    int i;
    fread(&i,sizeof(int),1,fp);
    return i;
  }

  //----------

  float read_float(void) {
    float f;
    fread(&f,sizeof(float),1,fp);
    return f;
  }

  //----------

  vec3_t read_vector(void) {
    vec3_t v;
    fread(&v.x,sizeof(float),1,fp);
    fread(&v.y,sizeof(float),1,fp);
    fread(&v.z,sizeof(float),1,fp);
    return v;
  }

  //----------

  char* read_string(char* ptr, int num) {
    fread(ptr,sizeof(char),num,fp);
    return ptr;

  }

  //----------

  mdl_vertex_t read_vertex() {
    mdl_vertex_t vertex;
    vertex.v[0] = read_byte();
    vertex.v[1] = read_byte();
    vertex.v[2] = read_byte();
    vertex.normalIndex = read_byte();
    return vertex;
  }

//------------------------------
private:
//------------------------------

  uint8_t* read_bytes(int num) {
    uint8_t* buffer = (uint8_t*)KODE_Malloc(num * sizeof(uint8_t));
    fread(buffer,sizeof(uint8_t),num,fp);
    return buffer;
  }

  //----------

  float* read_floats(int num) {
    float* buffer = (float*)KODE_Malloc(num * sizeof(float));
    fread(buffer,sizeof(float),num,fp);
    return buffer;
  }

//------------------------------
private:
//------------------------------

  void read_mdl_header() {
    //mdl_header = (mdl_header_t*)KODE_Malloc(sizeof(mdl_header_t));
    mdl_header.ident          = read_int();
    mdl_header.version        = read_int();
    mdl_header.scale          = read_vector();
    mdl_header.translate      = read_vector();
    mdl_header.boundingradius = read_float();
    mdl_header.eyeposition    = read_vector();
    mdl_header.num_skins      = read_int();
    mdl_header.skinwidth      = read_int();
    mdl_header.skinheight     = read_int();
    mdl_header.num_verts      = read_int();
    mdl_header.num_tris       = read_int();
    mdl_header.num_frames     = read_int();
    mdl_header.synctype       = read_int();
    mdl_header.flags          = read_int();
    mdl_header.size           = read_float();
  }

  void read_mdl_skins() {
    mdl_skins = (mdl_skin_t *)KODE_Malloc(sizeof(mdl_skin_t) * mdl_header.num_skins);
    for (int i=0; i<mdl_header.num_skins; i++) {
      int skinsize =  mdl_header.skinwidth * mdl_header.skinheight;
      //uint8_t* temp = (uint8_t*)KODE_Malloc(size);
      mdl_skins[i].data = (uint8_t *)KODE_Malloc(sizeof(uint8_t) * skinsize);
      int group = read_int();
      if (group == 0) {
        mdl_skins[i].group = group;
        mdl_skins[i].data = read_bytes(skinsize);
        //s.nb = 0;
        //s.time = KODE_NULL;
        //temp = read_byte_buffer(size);
        //s.data  = convert_skin(temp,size);
      }
      else {
        //s.group = 1;
        //s.nb    = read_int();
        //KODE_DPrint("    group: %i\n",s.nb);
        //s.time  = read_float_buffer(s.nb);
        ////s.data  = read_byte_buffer(s.nb * mdl_header.skin_width * mdl_header.skin_height);
        //temp = read_byte_buffer(s.nb * size);
        //s.data  = convert_skin(temp,s.nb * size);
      }
      //KODE_Free(temp);
    }
  }

  void read_mdl_texcoords() {
    mdl_texcoords = (mdl_texcoord_t *)KODE_Malloc(sizeof(mdl_texcoord_t) * mdl_header.num_verts);
    for (int i=0; i<mdl_header.num_verts; i++) {
      mdl_texcoords[i].onseam = read_int();
      mdl_texcoords[i].s = read_int();
      mdl_texcoords[i].t = read_int();
      KODE_DPrint("  texcoord %i onseam %i u %i v %i\n",i,mdl_texcoords[i].onseam,mdl_texcoords[i].s,mdl_texcoords[i].t);
    }
  }

  void read_mdl_triangles() {
    mdl_triangles = (mdl_triangle_t *)KODE_Malloc(sizeof(mdl_triangle_t) * mdl_header.num_tris);
    //fread(mdl_triangles, sizeof(mdl_triangle_t), mdl_header.num_tris, fp);
    for (int i=0; i<mdl_header.num_tris; i++) {
      mdl_triangles[i].facesfront = read_int();
      mdl_triangles[i].vertex[0] = read_int();
      mdl_triangles[i].vertex[1] = read_int();
      mdl_triangles[i].vertex[2] = read_int();
      KODE_DPrint("  triangle %i front %i x %i y %i z %i\n",i,mdl_triangles[i].facesfront,mdl_triangles[i].vertex[0],mdl_triangles[i].vertex[1],mdl_triangles[i].vertex[2]);
    }
  }

  mdl_vertex_t* read_vertices(int num) {
    mdl_vertex_t* vertices = (mdl_vertex_t *)KODE_Malloc(sizeof(mdl_vertex_t) * num);
    //fread(vertices, sizeof(mdl_vertex_t), num, fp);
    for (int i=0; i<num; i++) {
      vertices[i].v[0] = read_byte();
      vertices[i].v[1] = read_byte();
      vertices[i].v[2] = read_byte();
      vertices[i].normalIndex = read_byte();
      KODE_DPrint("  vertex %i v1 %i v2 %i v3 %i normal %i\n",i,vertices[i].v[0],vertices[i].v[1],vertices[i].v[2]);
    }
    return vertices;
  }

  void read_mdl_frames() {
    mdl_frames = (mdl_frame_t *)KODE_Malloc(sizeof(mdl_frame_t) * mdl_header.num_frames);
    for (int i=0; i<mdl_header.num_frames; ++i) {
      int type = read_int();
      if (type == 0) { // mdl_simpleframe_t
        mdl_frames[i].type = type;
        mdl_frames[i].frame.bboxmin = read_vertex();
        mdl_frames[i].frame.bboxmax = read_vertex();
        read_string(mdl_frames[i].frame.name,16);
        KODE_DPrint("  frame %i type %i name %s\n",i,mdl_frames[i].type,mdl_frames[i].frame.name);
        mdl_frames[i].frame.verts = read_vertices(mdl_header.num_verts);
      }
      else { // mdl_groupframe_t
        KODE_DPrint("  frame %i type %i\n",i,type);
        //int num               = type;
        //mdl_frames[i].type    = type;               /* !0 = group */
        //mdl_frames[i].min     = read_vertex();      /* min pos in all simple frames */
        //mdl_frames[i].max     = read_vertex();      /* max pos in all simple frames */
        //mdl_frames[i].time    = read_floats(num);   /* time duration for each frame */
        //mdl_frames[i].frames  = read_frames(num);  /* simple frame list */
      }
    }
  }

};

//----------------------------------------------------------------------
#endif

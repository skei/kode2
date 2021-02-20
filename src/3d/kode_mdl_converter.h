#ifndef kode_mdl_converter_included
#define kode_mdl_converter_included
//----------------------------------------------------------------------

#include "kode.h"
//#include "3d/kode_3d.h"

#define MESH_SCALE (1.0f/255.0f)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "extern/stb/stb_image_write.h"

//----------------------------------------------------------------------

uint8_t mdl_colormap[256][3] = {
  {  0,   0,   0}, { 15,  15,  15}, { 31,  31,  31}, { 47,  47,  47},
  { 63,  63,  63}, { 75,  75,  75}, { 91,  91,  91}, {107, 107, 107},
  {123, 123, 123}, {139, 139, 139}, {155, 155, 155}, {171, 171, 171},
  {187, 187, 187}, {203, 203, 203}, {219, 219, 219}, {235, 235, 235},
  { 15,  11,   7}, { 23,  15,  11}, { 31,  23,  11}, { 39,  27,  15},
  { 47,  35,  19}, { 55,  43,  23}, { 63,  47,  23}, { 75,  55,  27},
  { 83,  59,  27}, { 91,  67,  31}, { 99,  75,  31}, {107,  83,  31},
  {115,  87,  31}, {123,  95,  35}, {131, 103,  35}, {143, 111,  35},
  { 11,  11,  15}, { 19,  19,  27}, { 27,  27,  39}, { 39,  39,  51},
  { 47,  47,  63}, { 55,  55,  75}, { 63,  63,  87}, { 71,  71, 103},
  { 79,  79, 115}, { 91,  91, 127}, { 99,  99, 139}, {107, 107, 151},
  {115, 115, 163}, {123, 123, 175}, {131, 131, 187}, {139, 139, 203},
  {  0,   0,   0}, {  7,   7,   0}, { 11,  11,   0}, { 19,  19,   0},
  { 27,  27,   0}, { 35,  35,   0}, { 43,  43,   7}, { 47,  47,   7},
  { 55,  55,   7}, { 63,  63,   7}, { 71,  71,   7}, { 75,  75,  11},
  { 83,  83,  11}, { 91,  91,  11}, { 99,  99,  11}, {107, 107,  15},
  {  7,   0,   0}, { 15,   0,   0}, { 23,   0,   0}, { 31,   0,   0},
  { 39,   0,   0}, { 47,   0,   0}, { 55,   0,   0}, { 63,   0,   0},
  { 71,   0,   0}, { 79,   0,   0}, { 87,   0,   0}, { 95,   0,   0},
  {103,   0,   0}, {111,   0,   0}, {119,   0,   0}, {127,   0,   0},
  { 19,  19,   0}, { 27,  27,   0}, { 35,  35,   0}, { 47,  43,   0},
  { 55,  47,   0}, { 67,  55,   0}, { 75,  59,   7}, { 87,  67,   7},
  { 95,  71,   7}, {107,  75,  11}, {119,  83,  15}, {131,  87,  19},
  {139,  91,  19}, {151,  95,  27}, {163,  99,  31}, {175, 103,  35},
  { 35,  19,   7}, { 47,  23,  11}, { 59,  31,  15}, { 75,  35,  19},
  { 87,  43,  23}, { 99,  47,  31}, {115,  55,  35}, {127,  59,  43},
  {143,  67,  51}, {159,  79,  51}, {175,  99,  47}, {191, 119,  47},
  {207, 143,  43}, {223, 171,  39}, {239, 203,  31}, {255, 243,  27},
  { 11,   7,   0}, { 27,  19,   0}, { 43,  35,  15}, { 55,  43,  19},
  { 71,  51,  27}, { 83,  55,  35}, { 99,  63,  43}, {111,  71,  51},
  {127,  83,  63}, {139,  95,  71}, {155, 107,  83}, {167, 123,  95},
  {183, 135, 107}, {195, 147, 123}, {211, 163, 139}, {227, 179, 151},
  {171, 139, 163}, {159, 127, 151}, {147, 115, 135}, {139, 103, 123},
  {127,  91, 111}, {119,  83,  99}, {107,  75,  87}, { 95,  63,  75},
  { 87,  55,  67}, { 75,  47,  55}, { 67,  39,  47}, { 55,  31,  35},
  { 43,  23,  27}, { 35,  19,  19}, { 23,  11,  11}, { 15,   7,   7},
  {187, 115, 159}, {175, 107, 143}, {163,  95, 131}, {151,  87, 119},
  {139,  79, 107}, {127,  75,  95}, {115,  67,  83}, {107,  59,  75},
  { 95,  51,  63}, { 83,  43,  55}, { 71,  35,  43}, { 59,  31,  35},
  { 47,  23,  27}, { 35,  19,  19}, { 23,  11,  11}, { 15,   7,   7},
  {219, 195, 187}, {203, 179, 167}, {191, 163, 155}, {175, 151, 139},
  {163, 135, 123}, {151, 123, 111}, {135, 111,  95}, {123,  99,  83},
  {107,  87,  71}, { 95,  75,  59}, { 83,  63,  51}, { 67,  51,  39},
  { 55,  43,  31}, { 39,  31,  23}, { 27,  19,  15}, { 15,  11,   7},
  {111, 131, 123}, {103, 123, 111}, { 95, 115, 103}, { 87, 107,  95},
  { 79,  99,  87}, { 71,  91,  79}, { 63,  83,  71}, { 55,  75,  63},
  { 47,  67,  55}, { 43,  59,  47}, { 35,  51,  39}, { 31,  43,  31},
  { 23,  35,  23}, { 15,  27,  19}, { 11,  19,  11}, {  7,  11,   7},
  {255, 243,  27}, {239, 223,  23}, {219, 203,  19}, {203, 183,  15},
  {187, 167,  15}, {171, 151,  11}, {155, 131,   7}, {139, 115,   7},
  {123,  99,   7}, {107,  83,   0}, { 91,  71,   0}, { 75,  55,   0},
  { 59,  43,   0}, { 43,  31,   0}, { 27,  15,   0}, { 11,   7,   0},
  {  0,   0, 255}, { 11,  11, 239}, { 19,  19, 223}, { 27,  27, 207},
  { 35,  35, 191}, { 43,  43, 175}, { 47,  47, 159}, { 47,  47, 143},
  { 47,  47, 127}, { 47,  47, 111}, { 47,  47,  95}, { 43,  43,  79},
  { 35,  35,  63}, { 27,  27,  47}, { 19,  19,  31}, { 11,  11,  15},
  { 43,   0,   0}, { 59,   0,   0}, { 75,   7,   0}, { 95,   7,   0},
  {111,  15,   0}, {127,  23,   7}, {147,  31,   7}, {163,  39,  11},
  {183,  51,  15}, {195,  75,  27}, {207,  99,  43}, {219, 127,  59},
  {227, 151,  79}, {231, 171,  95}, {239, 191, 119}, {247, 211, 139},
  {167, 123,  59}, {183, 155,  55}, {199, 195,  55}, {231, 227,  87},
  {127, 191, 255}, {171, 231, 255}, {215, 255, 255}, {103,   0,   0},
  {139,   0,   0}, {179,   0,   0}, {215,   0,   0}, {255,   0,   0},
  {255, 243, 147}, {255, 247, 199}, {255, 255, 255}, {159,  91,  83}
};

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

class KODE_MdlConverter {

//------------------------------
private:
//------------------------------

  mdl_model_t mdl;
  FILE*       fp = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_MdlConverter() {
  }

  //----------

  ~KODE_MdlConverter() {
  }

//------------------------------
public:
//------------------------------

//------------------------------
public:
//------------------------------

  mdl_model_t* read_mdl(const char *filename) {
    KODE_DPrint("Reading '%s'\n",filename);
    fp = fopen(filename,"rb");
    if (!fp) {
      KODE_DPrint("Error. Couldn't open '%s'\n",filename);
      return KODE_NULL;
    }
    read_header();
    if ((mdl.header->ident != 1330660425) || (mdl.header->version != 6)) {
      fprintf(stderr, "Error: bad version or identifier\n");
      fclose(fp);
      return KODE_NULL;
    }
    read_skins();
    read_texcoords();
    read_triangles();
    read_frames();
    fclose (fp);
    return &mdl;
  }

  //----------

  void delete_mdl() {
    delete_skins();
    delete_texcoords();
    delete_triangles();
    delete_frames();
    delete_header();
    //KODE_Delete(mdl.header);
  }

  //----------

  void write_tscn(const char* filename) {
  }


//------------------------------
private:
//------------------------------

  uint8_t* convert_skin(uint8_t* skin, int size) {
    uint8_t* buffer = (uint8_t*)KODE_Malloc(4 * size);
    for (int i=0; i<size; i++) {
      uint8_t c = skin[i];
      buffer[i*4  ] = mdl_colormap[c][0]; // b
      buffer[i*4+1] = mdl_colormap[c][1]; // g
      buffer[i*4+2] = mdl_colormap[c][2]; // r
      buffer[i*4+3] = 255;                // a
    }
    return buffer;
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

  //----------

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

  //----------

  void read_header() {
    mdl.header = (mdl_header_t*)KODE_Malloc(sizeof(mdl_header_t));
    mdl.header->ident          = read_int();
    mdl.header->version        = read_int();
    mdl.header->scale          = read_vector();
    mdl.header->translate      = read_vector();
    mdl.header->boundingradius = read_float();
    mdl.header->eyeposition    = read_vector();
    mdl.header->num_skins      = read_int();
    mdl.header->skinwidth      = read_int();
    mdl.header->skinheight     = read_int();
    mdl.header->num_verts      = read_int();
    mdl.header->num_tris       = read_int();
    mdl.header->num_frames     = read_int();
    mdl.header->synctype       = read_int();
    mdl.header->flags          = read_int();
    mdl.header->size           = read_float();
  }

  //----------

  void read_skins() {
    mdl.skins = (mdl_skin_t *)KODE_Malloc(sizeof(mdl_skin_t) * mdl.header->num_skins);
    for (int i=0; i<mdl.header->num_skins; i++) {
      int skinsize =  mdl.header->skinwidth * mdl.header->skinheight;
      //uint8_t* temp = (uint8_t*)KODE_Malloc(size);
      mdl.skins[i].data = (uint8_t *)KODE_Malloc(sizeof(uint8_t) * skinsize);
      int group = read_int();
      if (group == 0) {
        mdl.skins[i].group = group;
        mdl.skins[i].data = read_bytes(skinsize);
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
        ////s.data  = read_byte_buffer(s.nb * mdl.header.skin_width * mdl.header.skin_height);
        //temp = read_byte_buffer(s.nb * size);
        //s.data  = convert_skin(temp,s.nb * size);
      }
      //KODE_Free(temp);
    }
  }

  //----------

  void read_texcoords() {
    mdl.texcoords = (mdl_texcoord_t *)KODE_Malloc(sizeof(mdl_texcoord_t) * mdl.header->num_verts);
    for (int i=0; i<mdl.header->num_verts; i++) {
      mdl.texcoords[i].onseam = read_int();
      mdl.texcoords[i].s = read_int();
      mdl.texcoords[i].t = read_int();
      KODE_DPrint("  texcoord %i onseam %i u %i v %i\n",i,mdl.texcoords[i].onseam,mdl.texcoords[i].s,mdl.texcoords[i].t);
    }
  }

  //----------

  void read_triangles() {
    mdl.triangles = (mdl_triangle_t *)KODE_Malloc(sizeof(mdl_triangle_t) * mdl.header->num_tris);
    //fread(mdl.triangles, sizeof(mdl_triangle_t), mdl.header.num_tris, fp);
    for (int i=0; i<mdl.header->num_tris; i++) {
      mdl.triangles[i].facesfront = read_int();
      mdl.triangles[i].vertex[0] = read_int();
      mdl.triangles[i].vertex[1] = read_int();
      mdl.triangles[i].vertex[2] = read_int();
      KODE_DPrint("  triangle %i front %i x %i y %i z %i\n",i,mdl.triangles[i].facesfront,mdl.triangles[i].vertex[0],mdl.triangles[i].vertex[1],mdl.triangles[i].vertex[2]);
    }
  }

  //----------

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

  //----------

  void read_frames() {
    mdl.frames = (mdl_frame_t *)KODE_Malloc(sizeof(mdl_frame_t) * mdl.header->num_frames);
    for (int i=0; i<mdl.header->num_frames; ++i) {
      int type = read_int();
      if (type == 0) { // mdl_simpleframe_t
        mdl.frames[i].type = type;
        mdl.frames[i].frame.bboxmin = read_vertex();
        mdl.frames[i].frame.bboxmax = read_vertex();
        read_string(mdl.frames[i].frame.name,16);
        KODE_DPrint("  frame %i type %i name %s\n",i,mdl.frames[i].type,mdl.frames[i].frame.name);
        mdl.frames[i].frame.verts = read_vertices(mdl.header->num_verts);
      }
      else { // mdl_groupframe_t
        KODE_DPrint("  frame %i type %i\n",i,type);
        //int num               = type;
        //mdl.frames[i].type    = type;               /* !0 = group */
        //mdl.frames[i].min     = read_vertex();      /* min pos in all simple frames */
        //mdl.frames[i].max     = read_vertex();      /* max pos in all simple frames */
        //mdl.frames[i].time    = read_floats(num);   /* time duration for each frame */
        //mdl.frames[i].frames  = read_frames(num);  /* simple frame list */
      }
    }
  }

  //----------

  //--------------------
  //
  //--------------------

  //void delete_bytes(uint8_t* buffer) {
  //  KODE_Free(buffer);
  //}

  //----------

  void delete_floats(float* buffer) {
    KODE_Free(buffer);
  }

//------------------------------
private:
//------------------------------

  void delete_skins() {
    for (int i=0; i<mdl.header->num_skins; i++) {
      KODE_Free(mdl.skins[i].data);
    }
    KODE_Free(mdl.skins);
  }

  //----------

  void delete_texcoords() {
    KODE_Free(mdl.texcoords);
  }

  //----------

  void delete_triangles() {
    KODE_Free(mdl.triangles);
  }

  //----------

  void delete_frames() {
    for (int i=0; i<mdl.header->num_frames; ++i) {
      //delete_frame(i);
      KODE_Free(mdl.frames[i].frame.verts);
    }
    KODE_Free(mdl.frames);
  }

  //----------

  void delete_header() {
    KODE_Free(mdl.header);
  }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//  void write_obj(const char* filename, int fr) {
//    FILE* fp = fopen(filename,"wt");
//    char buffer[256];
//    int num_tris  = mdl.header->num_tris;
//    vec3_t scale = mdl.header->scale;
//    vec3_t translate = mdl.header->scale;
//    mdl_simpleframe_t* frame = &mdl.frames[fr].frame;
//    // faces
//    for (int i=0; i<num_tris; i++) {
//
//      int i0 = mdl.triangles[i].vertex[0];
//      int i1 = mdl.triangles[i].vertex[1];
//      int i2 = mdl.triangles[i].vertex[2];
//
//      mdl_vertex_t* vtx0 = &frame->verts[i0];
//      mdl_vertex_t* vtx1 = &frame->verts[i1];
//      mdl_vertex_t* vtx2 = &frame->verts[i2];
//
//      vec3_t v0 = vec3_t( vtx0->v[0], vtx0->v[1], vtx0->v[2] );
//      vec3_t v1 = vec3_t( vtx1->v[0], vtx1->v[1], vtx1->v[2] );
//      vec3_t v2 = vec3_t( vtx2->v[0], vtx2->v[1], vtx2->v[2] );
//
//      v0 = (v0 * scale * MESH_SCALE) + translate;
//      v1 = (v1 * scale * MESH_SCALE) + translate;
//      v2 = (v2 * scale * MESH_SCALE) + translate;
//
//      //v0 = ((v0 / 255.0f) - 0.5f) * 2.0f;
//      //v1 = ((v1 / 255.0f) - 0.5f) * 2.0f;
//      //v2 = ((v2 / 255.0f) - 0.5f) * 2.0f;
//
//      //v0 = (v0 / 255.0f);
//      //v1 = (v1 / 255.0f);
//      //v2 = (v2 / 255.0f);
//
//      sprintf(buffer,"v %f %f %f\n",v0.x,v0.y,v0.z);
//      fwrite(buffer,strlen(buffer),1,fp);
//      sprintf(buffer,"v %f %f %f\n",v1.x,v1.y,v1.z);
//      fwrite(buffer,strlen(buffer),1,fp);
//      sprintf(buffer,"v %f %f %f\n",v2.x,v2.y,v2.z);
//      fwrite(buffer,strlen(buffer),1,fp);
//    }
//    // texcoords
//    for (int i=0; i<num_tris; i++) {
//      int i0 = (i * 3);
//      int i1 = (i * 3) + 1;
//      int i2 = (i * 3) + 2;
//      float u0 = (float)i0;
//      float v0 = 0.0f;
//      float u1 = (float)i1;
//      float v1 = 0.0f;
//      float u2 = (float)i2;
//      float v2 = 0.0f;
//      //float u0 = (float) (i0 & 0xff);
//      //float v0 = (float)((i0 & 0xff00) >> 8);
//      //float u1 = (float) (i1 & 0xff);
//      //float v1 = (float)((i1 & 0xff00) >> 8);
//      //float u2 = (float) (i2 & 0xff);
//      //float v2 = (float)((i2 & 0xff00) >> 8);
//      sprintf(buffer,"vt %f %f\n",u0,v0);
//      fwrite(buffer,strlen(buffer),1,fp);
//      sprintf(buffer,"vt %f %f\n",u1,v1);
//      fwrite(buffer,strlen(buffer),1,fp);
//      sprintf(buffer,"vt %f %f\n",u2,v2);
//      fwrite(buffer,strlen(buffer),1,fp);
//    }
//    // faces
//    for (int i=0; i<num_tris; i++) {
//      int f1 = 1 + (i * 3);
//      int f2 = 1 + (i * 3) + 1;
//      int f3 = 1 + (i * 3) + 2;
//      sprintf(buffer,"f %i/%i %i/%i %i/%i\n",f1,f1,f3,f3,f2,f2);
//      fwrite(buffer,strlen(buffer),1,fp);
//    }
//    fclose(fp);
//  }
//
//  //----------
//
//  void write_vertex_data(const char* filename) {
//    int num_tris = mdl.header->num_tris;
//    float skinwidth = (float)mdl.header->skinwidth;
//    float skinheight = (float)mdl.header->skinheight;
//    //int num_verts = mdl.header->num_verts;
//    KODE_DPrint("writing data '%s'\n",filename);
//    uint8_t* buffer = (uint8_t*)KODE_Malloc(num_tris * 3 * 4 * 1);
//    // line 1. real uv
//    uint8_t* ptr = buffer;
//    for (int i=0; i<num_tris; i++) {
//
//      //bool front_facing = (mdl.triangles[i].facesfront > 0);
//
//      int i0 = mdl.triangles[i].vertex[0];
//      int i1 = mdl.triangles[i].vertex[1];
//      int i2 = mdl.triangles[i].vertex[2];
//
//      /*
//        s = (GLfloat)mdl->texcoords[mdl->triangles[i].vertex[j]].s;
//        t = (GLfloat)mdl->texcoords[mdl->triangles[i].vertex[j]].t;
//        if (!mdl->triangles[i].facesfront && mdl->texcoords[mdl->triangles[i].vertex[j]].onseam) {
//          s += mdl->header.skinwidth * 0.5f; // Backface
//	      }
//        // Scale s and t to range from 0.0 to 1.0
//        s = (s + 0.5) / mdl->header.skinwidth;
//        t = (t + 0.5) / mdl->header.skinheight;
//      */
//
//
//      float u0 = (((float)mdl.texcoords[i0].s + 0.5f) / skinwidth);
//      float v0 = (((float)mdl.texcoords[i0].t + 0.5f) / skinheight);
//      //bool on_seam0 = (mdl.texcoords[i0].onseam > 0);
//      //if (on_seam0 && !front_facing) u0 += 0.5f;
//      *ptr++ = (u0 * 255.0f);
//      *ptr++ = (v0 * 255.0f);
//      *ptr++ = 0;
//      *ptr++ = 255;
//
//      float u1 = (((float)mdl.texcoords[i1].s + 0.5f) / skinwidth);
//      float v1 = (((float)mdl.texcoords[i1].t + 0.5f) / skinheight);
//      //bool on_seam1 = (mdl.texcoords[i1].onseam > 0);
//      //if (on_seam1 && !front_facing) { u1 += 0.5f; }
//      *ptr++ = (u1 * 255.0f);
//      *ptr++ = (v1 * 255.0f);
//      *ptr++ = 0;
//      *ptr++ = 255;
//
//      float u2 = (((float)mdl.texcoords[i2].s + 0.5f) / skinwidth);
//      float v2 = (((float)mdl.texcoords[i2].t + 0.5f) / skinheight);
//      //bool on_seam2 = (mdl.texcoords[i2].onseam > 0);
//      //if (on_seam2 && !front_facing) { u2 += 0.5f; }
//      *ptr++ = (u2 * 255.0f);
//      *ptr++ = (v2 * 255.0f);
//      *ptr++ = 0;
//      *ptr++ = 255;
//
//    }
//    // line 2. barycentric coordinates
//    //int res =
//    stbi_write_png(filename,num_tris*3,1,4,buffer,(num_tris*3*4));
//  }
//
//  //----------
//
//  void write_vertex_frames(const char* filename) {
//    //int num_verts = mdl.header->num_verts;
//    int num_tris = mdl.header->num_tris;
//    int num_frames = mdl.header->num_frames;
//    KODE_DPrint("writing anim '%s' (%i tris %i frames)\n",filename,num_tris,num_frames);
//    uint8_t* vertex_buffer = (uint8_t*)KODE_Malloc(num_tris*3*4*num_frames);
//    //KODE_DPrint("  verts %i frames %i\n",w,h);
//    uint8_t* ptr = vertex_buffer;
//    for (int fr=0; fr<num_frames; fr++) {
//      mdl_simpleframe_t* frame = &mdl.frames[fr].frame;
//      //int pos = (num_tris * 3 * fr * 4);
//
//      //for (int ve=0; ve<num_verts; ve++) {
//      for (int tr=0; tr<num_tris; tr++) {
//
//        int i0 = mdl.triangles[tr].vertex[0];
//        int i1 = mdl.triangles[tr].vertex[1];
//        int i2 = mdl.triangles[tr].vertex[2];
//
//        mdl_vertex_t* vtx0 = &frame->verts[i0];
//        mdl_vertex_t* vtx1 = &frame->verts[i1];
//        mdl_vertex_t* vtx2 = &frame->verts[i2];
//
//        vec3_t v0 = vec3_t( vtx0->v[0], vtx0->v[1], vtx0->v[2] );
//        vec3_t v1 = vec3_t( vtx1->v[0], vtx1->v[1], vtx1->v[2] );
//        vec3_t v2 = vec3_t( vtx2->v[0], vtx2->v[1], vtx2->v[2] );
//
//        //v0 = (v0 * scale * MESH_SCALE) + translate;
//        //v1 = (v1 * scale * MESH_SCALE) + translate;
//        //v2 = (v2 * scale * MESH_SCALE) + translate;
//
//        uint8_t r0 = v0.x;
//        uint8_t g0 = v0.y;
//        uint8_t b0 = v0.z;
//        *ptr++ = r0;
//        *ptr++ = g0;
//        *ptr++ = b0;
//        *ptr++ = 255;
//
//        uint8_t r1 = v1.x;
//        uint8_t g1 = v1.y;
//        uint8_t b1 = v1.z;
//        *ptr++ = r1;
//        *ptr++ = g1;
//        *ptr++ = b1;
//        *ptr++ = 255;
//
//        uint8_t r2 = v2.x;
//        uint8_t g2 = v2.y;
//        uint8_t b2 = v2.z;
//        *ptr++ = r2;
//        *ptr++ = g2;
//        *ptr++ = b2;
//        *ptr++ = 255;
//
//      }
//    }
//    //int res =
//    stbi_write_png(filename,num_tris*3,num_frames,4,vertex_buffer,(num_tris*3*4));
//    KODE_Free(vertex_buffer);
//  }
//
//  //----------
//
//  void write_normal_frames(const char* filename) {
//    //int num_verts = mdl.header->num_verts;
//    int num_tris = mdl.header->num_tris;
//    int num_frames = mdl.header->num_frames;
//    KODE_DPrint("writing anim '%s' (%i tris %i frames)\n",filename,num_tris,num_frames);
//    uint8_t* buffer = (uint8_t*)KODE_Malloc(num_tris*3*num_frames*4);
//    //KODE_DPrint("  verts %i frames %i\n",w,h);
//    for (int fr=0; fr<num_frames; fr++) {
//      //mdl_simpleframe_t* frame = &mdl.frames[fr].frame;
//      vec3_t* normals = calc_vertex_normals(fr);
//      int pos = (num_tris * 3 * fr * 4);
//
//      for (int tr=0; tr<num_tris; tr++) {
//
//        int i0 = mdl.triangles[tr].vertex[0];
//        int i1 = mdl.triangles[tr].vertex[1];
//        int i2 = mdl.triangles[tr].vertex[2];
//
//        vec3_t n0 = vec3_t( normals[i0].x, normals[i0].y, normals[i0].z );
//        vec3_t n1 = vec3_t( normals[i1].x, normals[i1].y, normals[i1].z );
//        vec3_t n2 = vec3_t( normals[i2].x, normals[i2].y, normals[i2].z );
//
//        uint8_t r0 = n0.x * 127.0f;
//        uint8_t g0 = n0.y * 127.0f;
//        uint8_t b0 = n0.z * 127.0f;
//        buffer[pos    ] = r0;
//        buffer[pos + 1] = g0;
//        buffer[pos + 2] = b0;
//        buffer[pos + 3] = 255;
//        pos += 4;
//
//        uint8_t r1 = n1.x * 127.0f;
//        uint8_t g1 = n1.y * 127.0f;
//        uint8_t b1 = n1.z * 127.0f;
//        buffer[pos    ] = r1;
//        buffer[pos + 1] = g1;
//        buffer[pos + 2] = b1;
//        buffer[pos + 3] = 255;
//        pos += 4;
//
//        uint8_t r2 = n2.x * 127.0f;
//        uint8_t g2 = n2.y * 127.0f;
//        uint8_t b2 = n2.z * 127.0f;
//        buffer[pos    ] = r2;
//        buffer[pos + 1] = g2;
//        buffer[pos + 2] = b2;
//        buffer[pos + 3] = 255;
//        pos += 4;
//
//      }
//      KODE_Free(normals);
//    }
//    //int res =
//    stbi_write_png(filename,num_tris*3,num_frames,4,buffer,(num_tris*3*4));
//    KODE_Free(buffer);
//  }
//
//  //----------
//
//  void write_skins(const char* filename) {
//    int num = mdl.header->num_skins;
//    KODE_DPrint("writing skins '%s' (%i skins)\n",filename,num);
//    int width = mdl.header->skinwidth;
//    int height = mdl.header->skinheight;
//    int size = width * height * 4;
//    uint8_t* buffer = (uint8_t*)KODE_Malloc(num * size);
//    uint8_t* ptr = buffer;
//
//    for (int i=0; i<num; i++) {
//      uint8_t* temp = convert_skin(mdl.skins[i].data,(width*height));
//      KODE_Memcpy(ptr,temp,size);
//      ptr += size;
//      KODE_Free(temp);
//    }
//
//    //int res =
//    stbi_write_png(
//      filename,
//      width,
//      height * num,
//      4,          // 1=C, 2=CA, 3=RGB, 4=RGBA
//      buffer,     //mdl.skins[num].data,
//      width * 4   // stride (bytes)
//    );
//
//    KODE_Free(buffer);
//  }
//
//  //----------
//
//  void write_tscn(const char* filename) {
//  }
//
////------------------------------
//private:
////------------------------------
//
//  //----------
//
//  //# v1,v2,v3 or v1,v3,v2 ??
//
//  vec3_t* calc_vertex_normals(int fr) {
//    int numver = mdl.header->num_verts;
//    int numtri = mdl.header->num_tris;
//    mdl_vertex_t* vertices = mdl.frames[fr].frame.verts;
//    vec3_t* vert_normals = (vec3_t*)KODE_Malloc(numver * sizeof(vec3_t));
//
//    for (int i=0; i<numver; i++) {
//      vert_normals[i] = vec3_t(0,0,0);
//    }
//
//    for (int i=0; i<numtri; i++) {
//      mdl_triangle_t* tri = &mdl.triangles[i];
//      int tri_v1 = tri->vertex[0];
//      int tri_v2 = tri->vertex[1];
//      int tri_v3 = tri->vertex[2];
//      //uint8_t bx,by,bz;
//      mdl_vertex_t* v1 = &vertices[tri_v1];
//      mdl_vertex_t* v2 = &vertices[tri_v2];
//      mdl_vertex_t* v3 = &vertices[tri_v3];
//      vec3_t v_1 = vec3_t(v1->v[0], v1->v[1], v1->v[2]);
//      vec3_t v_2 = vec3_t(v2->v[0], v2->v[1], v2->v[2]);
//      vec3_t v_3 = vec3_t(v3->v[0], v3->v[1], v3->v[2]);
//      vec3_t a = v_3 - v_1;
//      vec3_t b = v_2 - v_1;
//      vec3_t normal = a.cross(b);
//      vert_normals[tri_v1] += normal;
//      vert_normals[tri_v2] += normal;
//      vert_normals[tri_v3] += normal;
//    }
//
//    for (int i=0; i<numver; i++) {
//      vert_normals[i].normalize();
//    }
//
//    return vert_normals;
//  }
//
//  //----------
//
//  //func get_description():
//  //	var d = ""
//  //	d += "skin_width: " + str(md2.header.skin_width) + "\n"
//  //	d += "skin_height: " + str(md2.header.skin_height) + "\n"
//  //	d += "frame_size: " + str(md2.header.frame_size) + "\n"
//  //	d += "num_skins : " + str(md2.header.num_skins) + "\n"
//  //	d += "num_vertices: " + str(md2.header.num_vertices) + "\n"
//  //	d += "num_tris: " + str(md2.header.num_tris) + "\n"
//  //	d += "num_glcmds: " + str(md2.header.num_glcmds) + "\n"
//  //	d += "num_frames: " + str(md2.header.num_frames) + "\n"
//  //	d += "skins...\n"
//  //	for s in range(md2.header.num_skins):
//  //		d += str(s) + ". "
//  //		d += md2.skins[s].name
//  //		d += " "
//  //	d += "\n"
//  //	d += "frames...\n"
//  //	for f in range(md2.header.num_frames):
//  //		d += str(f) + ":"
//  //		d += md2.frames[f].name
//  //		d += " "
//  //	d += "\n"
//  //	return d
//

//----------------------------------------------------------------------
#endif

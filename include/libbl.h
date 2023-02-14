//  libbl.h
//  (heders)
//
//  Created by David Kviloria on 10/14/20.
//

// License: Public Domain

// This is free and unencumbered software released into the public domain.
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#ifndef _lib_bl_h_
#define _lib_bl_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#ifdef LIBBL_USE_APPLE_OPENGL && __APPLE__
#include <OpenGL/gl.h>
#else
#error "LIBBL COMPILATION: `LIBBL_USE_APPLE_OPENGL` must be defined"
// FIXME: this causing to crash when tryng to use external GL context loader
#if 0
#if defined(LIBBL_INJECT_EXT_GL_LOADER)
#include LIBBL_INJECT_EXT_GL_LOADER
#else
#error "LIBBL_INJECT_EXT_GL_LOADER must be defined and must be including GL context loader"
#endif
#endif
#endif

#if __APPLE__
#include <strings.h>
#else
#include <string.h>
#endif

#ifdef LIBBL_STB_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb/stb_image.h"
#endif

#define PI 3.14159265
#define PI_DEG (PI / 180.0f)

#if defined(LIBBL_ENTITY_MANAGER_SIZE)
#define GLOBAL_M_SIZE         LIBBL_ENTITY_MANAGER_SIZE
#else
#define GLOBAL_M_SIZE         512
#endif

typedef unsigned char         u8_t;
typedef unsigned short        u16_t;
typedef unsigned int          u32_t;
typedef unsigned long long    u64_t;
typedef short                 i16_t;
typedef int                   i32_t;
typedef long long             i64_t;
typedef signed char           s8_t;
typedef signed short          s16_t;
typedef signed int            s32_t;
typedef signed long long      s64_t;
typedef float                 f32_t;
typedef double                f64_t;
typedef char                  c_t;
typedef char                  b_t;
typedef const char            cc_t;
typedef size_t                sz_t;
typedef intptr_t              iptr_t;
typedef uintptr_t             uptr_t;

#define _x_ 0
#define _y_ 1
#define _z_ 2
#define _w_ 3

#define _r_ 0
#define _g_ 1
#define _b_ 2
#define _a_ 3

#define _width_ 0
#define _height_ 1
#define _depth_ 2

#define LIBBL_DEFINE_VEC(n) \
  typedef f32_t vec##n[n];  \

LIBBL_DEFINE_VEC(2);

LIBBL_DEFINE_VEC(3);

//
// @Vertex
//

typedef struct {
vec3                          position;
vec2                          uv;
} bl_vertex_t;

//
// @VertexIndex
//
typedef struct {
u32_t                         vertices[3];
} bl_vertex_index_t;

//
// @debug_state
//
typedef struct
{
u32_t                         draw_call_count;
u32_t                         vertex_count;
u32_t                         index_count;
u32_t                         texture_count;
u32_t                         shader_count;
u32_t                         entity_count;
} bl_stats_t;

//
// @entity
//
typedef struct
{
  u8_t                        flags;
  cc_t*                       name;
  u16_t                       index;
  vec3                        position;
  vec3                        scale;
  f32_t                       rotation;
} bl_ent_t;

static bl_stats_t             bl_global_stats = {0};
bl_ent_t* global_manager      [GLOBAL_M_SIZE];

//
// @transform
//
typedef struct
{
  vec2                        position;
  vec2                        rotation;
  vec2                        scale;
} bl_transfor_t;

/**
 * @Utility [ENTITY]
 *
 * @param {cc_t*} name
 * @param {bl_ent_t*} _manager[]
 * @return {bl_ent_t*}
 *
 * @brief Get entity by name from manager array
 *
 */
bl_ent_t* bl_get_entity(cc_t* name, bl_ent_t* _manager[]);

/**
 * @Utility [ENTITY]
 *
 * @param {bl_ent_t*} entity
 * @param {bl_ent_t*} _manager[GLOBAL_M_SIZE]
 *
 * @brief Add entity to global manager
 *
 */
void bl_register_entity(bl_ent_t* ent, bl_ent_t* _manager[GLOBAL_M_SIZE]);

/**
 * @Utility [FILE]
 *
 * @param {cc_t*} path
 * @return {cc_t*}
 *
 * @brief Load file from disk, Store in the memory and return pointer to the memory
 *
 */
cc_t* read_file_content(cc_t* path);

/**
 * @Gl [TEXTURE]
 *
 * @param {cc_t*} path
 * @param {u32_t*} texture_id
 *
 * @brief Load file from disk, Store in the memory and generate GL_TEXTURE buffer
 */
void load_and_create_texture(cc_t* path, u32_t* texture_id, u32_t __SLOT, i32_t __FLIP_IMG_V);

//
// @VertexArray
//

typedef struct
{
u32_t                         m_renderer_id;
} vertex_array_t;

/**
 * @Gl [VERTEX_ARRAY]
 *
 * @param {vertex_array_t*} vao
 *
 * @brief Generates vertex array object and binds it
 *
 */
void gen_and_bind_vertex_array(vertex_array_t* vao);

/**
 * @Gl [VERTEX_ARRAY]
 *
 * @param {vertex_array_t*} vao
 *
 * @brief Unbinds vertex array object
 *
 */
void unbind_vertex_array(vertex_array_t* vao);

/**
 * @Gl [VERTEX_ARRAY]
 *
 * @param {vertex_array_t*} vao
 *
 * @brief Deletes vertex array object from GPU memory
 *
 */
void free_vertex_array(vertex_array_t* vao);

//
// @VertexBuffer
//
typedef struct
{
u32_t                         m_renderer_id;
} vertex_buffer_t;

/**
 * @Gl [VERTEX_BUFFER]
 *
 * @param {vertex_buffer_t*} vbo
 *
 * @brief Generates vertex buffer object
 *
 */
void gen_vertex_buffer(vertex_buffer_t* vbo);

/**
 * @Gl [VERTEX_BUFFER]
 *
 * @param {vertex_buffer_t*} vbo
 *
 * @brief Binds vertex buffer object
 *
 */
void bind_vertex_buffer(vertex_buffer_t* vbo);

/**
 * @Gl [VERTEX_BUFFER]
 *
 * @param {iptr_t*} size
 * @param {f32_t*} data
 *
 * @brief Upload data to vertex buffer object from CPU memory to GPU memory
 *
 */
void vertex_buffer_data(iptr_t size, f32_t* data);

/**
 * @Gl [VERTEX_BUFFER]
 *
 * @brief Unbinds vertex buffer object
 *
 */
void unbind_vertex_buffer();

/**
 * @Gl [VERTEX_BUFFER]
 *
 * @param {vertex_buffer_t*} vbo
 *
 * @brief Deletes vertex buffer object from GPU memory
 *
 */
void free_vertex_buffer(vertex_buffer_t* vbo);

//
// @IndiceBuffer
//
typedef struct
{
u32_t                         m_renderer_id;
} indice_buffer_t;

/**
 * @Gl [INDICE_BUFFER]
 *
 * @param {indice_buffer_t*} ebo
 *
 * @brief Generates indice buffer object
 *
 */
void gen_indice_buffer(indice_buffer_t* ebo);

/**
 * @Gl [INDICE_BUFFER]
 *
 * @param {indice_buffer_t*} ebo
 *
 * @brief Binds indice buffer object
 *
 */
void bind_indice_buffer(indice_buffer_t* ebo);

/**
 * @Gl [INDICE_BUFFER]
 *
 * @param {iptr_t*} size
 * @param {u32_t*} data
 *
 * @brief Upload data to indice buffer object from CPU memory to GPU memory
 *
 */
void indice_buffer_data(iptr_t size, u32_t* data);

/**
 * @Gl [INDICE_BUFFER]
 *
 * @param {vertex_buffer_t*} ebo
 *
 * @brief Delete indice buffer object from GPU memory
 *
 */
void free_indice_buffer(vertex_buffer_t* ebo);

//
// @ShaderProgram
//
typedef struct
{
i32_t                         m_program_id;
} shader_program_t;

//
// @ShaderType
//
typedef enum {
  ShaderType_VERTEX   =      GL_VERTEX_SHADER,
  ShaderType_FRAGMENT =      GL_FRAGMENT_SHADER,
} shader_type_t;

/**
 * @Gl [SHADER_PROGRAM]
 *
 * @param {cc_t*} contnet
 * @param {shader_type_t*} type
 *
 * @return {u32_t}
 *
 * @brief Compiles shader program from source code
 *
 */
u32_t compile_shader(cc_t* content, shader_type_t type);

/**
 * @Gl [SHADER_PROGRAM]
 *
 * @param {shader_program_t*} program
 * @param {u32_t*} fragment_shader
 * @param {u32_t*} vertex_shader
 *
 * @brief Links shader program from vertex and fragment shader
 *
 */
void create_program(shader_program_t* p, u32_t fsh, u32_t vsh);

/**
 * @Gl [SHADER_PROGRAM]
 *
 * @param {shader_program_t*} program
 *
 * @brief Uses/binds shader program
 *
 */
void attach_program(shader_program_t* p);

/**
 * @Gl [SHADER_PROGRAM]
 *
 * @param {shader_program_t*} program
 *
 * @brief Unbinds shader program
 *
 */
void detach_program(shader_program_t* p);

/**
 * @Gl [Draw]
 *
 * @param {s32_t} count - Number of vertices to draw
 *
 * @brief Draws elements from indice buffer
 *
 */
void draw_triangles(s32_t count);

//////////////////////////////////////////////////////////
// @IMPLEMENTATION
//

#if defined(LIBBL_IMPLEMENTATION)

bl_ent_t* bl_get_entity(cc_t* name, bl_ent_t* _manager[])
{
  for (u32_t i = 0; i <= GLOBAL_M_SIZE; i++) {
    if (_manager[i] == NULL)  { continue; }
    if (strcmp(name, _manager[i]->name) == 0) {
      return                  _manager[i];
    }
  }
  return                      NULL;
}

void bl_register_entity(bl_ent_t* ent, bl_ent_t* _manager[GLOBAL_M_SIZE])
{
  _manager[*&ent->index] =    ent;
}

//////////////////////////////////////////////////////////
// @Utility functions
//
cc_t* read_file_content(cc_t* path)
{
  FILE*                       infile;
  c_t*                        buffer;
  s64_t                       numbytes;

  infile = fopen              (path, "r");
  if (infile == NULL) {
    perror                    ("fopen\t");
    exit                      (EXIT_FAILURE);
  }

  fseek                       (infile, 0L, SEEK_END);
  numbytes = ftell            (infile);
  fseek                       (infile, 0L, SEEK_SET);
  buffer =                    (b_t*) calloc(numbytes, sizeof(b_t));
  if (buffer == NULL)         { exit  (EXIT_FAILURE); }

  fread                       (buffer, sizeof(b_t), numbytes, infile);
  fclose                      (infile);
  return                      buffer;
}

void load_and_create_texture(cc_t* path, u32_t* texture_id, u32_t __SLOT, i32_t __FLIP_IMG_V)
{

  #if                         defined(STB_IMAGE_IMPLEMENTATION)

  if (__SLOT == NULL)         { __SLOT = 0; }
  if (__FLIP_IMG_V == NULL)   { __FLIP_IMG_V = 0; }

  stbi_set_flip_vertically_on_load(__FLIP_IMG_V);

  s32_t                        width;
  s32_t                        height;
  s32_t                        channels;
  u8_t*                         buffer;

  buffer = stbi_load          (path, &width, &height, &channels, 0);

  glGenTextures               (1, texture_id);
  glActiveTexture             (GL_TEXTURE0 + __SLOT);
  glBindTexture               (GL_TEXTURE_2D, texture_id);

  u32_t                       texDataFormat;
  if (channels == 4)          { texDataFormat = GL_RGBA; }
  if (channels == 3)          { texDataFormat = GL_RGB; }

  glTexParameteri             (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri             (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri             (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri             (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D                (GL_TEXTURE_2D, 0, texDataFormat, width, height, 0, texDataFormat, GL_UNSIGNED_BYTE, buffer);
  glGenerateMipmap            (GL_TEXTURE_2D);
  stbi_image_free             (buffer);

  #else
  #error                      "STB_IMAGE_IMPLEMENTATION not defined"
  #endif
}

//
// @VertexArray
//

void gen_and_bind_vertex_array(vertex_array_t* vao)
{
#if defined(LIBBL_USE_APPLE_OPENGL) && defined(__APPLE__)
  glGenVertexArraysAPPLE      (1, &vao->m_renderer_id);
  glBindVertexArrayAPPLE      (vao->m_renderer_id);
#else
  glGenVertexArrays           (1, &vao->m_renderer_id);
  glBindVertexArray           (vao->m_renderer_id);
#endif
}

void unbind_vertex_array(vertex_array_t* vao)
{
#if defined(LIBBL_USE_APPLE_OPENGL) && defined(__APPLE__)
  glBindVertexArrayAPPLE      (0);
#else
  glBindVertexArray           (0);
#endif
}

void free_vertex_array(vertex_array_t* vao)
{
#if defined(LIBBL_USE_APPLE_OPENGL) && defined(__APPLE__)
  glDeleteVertexArraysAPPLE   (1, &vao->m_renderer_id);
#else
  glDeleteVertexArrays        (1, &vao->m_renderer_id);
#endif
}

//
// @VertexBuffer
//

void gen_vertex_buffer(vertex_buffer_t* vbo)
{
  glGenBuffers                (1, &vbo->m_renderer_id);
}

void bind_vertex_buffer(vertex_buffer_t* vbo)
{
  glBindBuffer                (GL_ARRAY_BUFFER, vbo->m_renderer_id);
}

void vertex_buffer_data(iptr_t size, f32_t* data)
{
  glBufferData                (GL_ARRAY_BUFFER, size, *&data, GL_STATIC_DRAW);
}

void unbind_vertex_buffer()
{
  glBindBuffer                (GL_ARRAY_BUFFER, 0);
}

void free_vertex_buffer(vertex_buffer_t* vbo)
{
  glDeleteBuffers             (1, &vbo->m_renderer_id);
}

//
// @IndiceBuffer
//

void gen_indice_buffer(indice_buffer_t* ebo)
{
  glGenBuffers                (1, &ebo->m_renderer_id);
}

void bind_indice_buffer(indice_buffer_t* ebo)
{
  glBindBuffer                (GL_ELEMENT_ARRAY_BUFFER, ebo->m_renderer_id);
}

void indice_buffer_data(iptr_t size, u32_t* data)
{
  glBufferData                (GL_ELEMENT_ARRAY_BUFFER, size, *&data, GL_STATIC_DRAW);
}

void free_indice_buffer(vertex_buffer_t* ebo)
{
  glDeleteBuffers             (1, &ebo->m_renderer_id);
}

//
// @ShaderProgram
//
u32_t compile_shader(cc_t* content, shader_type_t type)
{
  i32_t                       compile_status;
  u32_t shader =              glCreateShader(type);
  if(0 == shader)
  {
    fprintf                   (stderr, "Error creating shader.\n");
    exit                      (EXIT_FAILURE);
  }
  glShaderSource              (shader, 1, &content, NULL);
  glCompileShader             (shader);

  glGetShaderiv               (shader, GL_COMPILE_STATUS, &compile_status);
  if                          (compile_status == GL_FALSE )
  {
    GLchar                    messages[256];
    glGetShaderInfoLog        (shader, sizeof(messages), 0, &messages[0]);
    fprintf                   (stderr, "%s\n", messages);
    exit                      (EXIT_FAILURE);
  }

  return                      shader;
}

void create_program(shader_program_t* p, u32_t fsh, u32_t vsh)
{
  p->m_program_id =           glCreateProgram();
  glAttachShader              (p->m_program_id, fsh);
  glAttachShader              (p->m_program_id, vsh);
  glLinkProgram               (p->m_program_id);

  glDeleteShader              (fsh);
  glDeleteShader              (vsh);
}

void attach_program(shader_program_t* p)
{
  glUseProgram                (p->m_program_id);
}

void detach_program(shader_program_t* p)
{
  glDeleteProgram             (p->m_program_id);
}

void draw_triangles(s32_t count)
{
  glDrawElements              (GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
  bl_global_stats.draw_call_count++;
}
#endif // LIBBL_IMPLEMENTATION

#ifdef                        __cplusplus
};
#endif

#endif                        /* _lib_bl_h_ */

#ifndef GL_TYPES_HPP
#define GL_TYPES_HPP

#include <GL/glew.h>

// utilities to convert between GLtypes and their GLenum representations

// GL type to GLenum
template <typename GLtype>
struct getEnum {
  const static GLenum value;
};

template<>
struct getEnum<GLbyte> {
  const static GLenum value = GL_BYTE;
};

template<>
struct getEnum<GLubyte> {
  const static GLenum value = GL_UNSIGNED_BYTE;
};

template<>
struct getEnum<GLshort> {
  const static GLenum value = GL_SHORT;
};

template<>
struct getEnum<GLushort> {
  const static GLenum value = GL_UNSIGNED_SHORT;
};

template<>
struct getEnum<GLint> {
  const static GLenum value = GL_INT;
};

template<>
struct getEnum<GLuint> {
  const static GLenum value = GL_UNSIGNED_INT;
};
/** same typedef as unsigned short...
template<>
struct getEnum<GLhalf> {
  const static GLenum value = GL_HALF_FLOAT;
};
*/
template<>
struct getEnum<GLfloat> {
  const static GLenum value = GL_FLOAT;
};

template<>
struct getEnum<GLdouble> {
  const static GLenum value = GL_DOUBLE;
};

// GLenum to GLtype size
constexpr std::size_t sizeofGLType(GLenum type) {
  
  switch (type) {

    case GL_BYTE:
      return sizeof(GLbyte);

    case GL_UNSIGNED_BYTE:
      return sizeof(GLubyte);

    case GL_SHORT:
      return sizeof(GLshort);

    case GL_UNSIGNED_SHORT:
      return sizeof(GLushort);
      
    case GL_INT:
      return sizeof(GLint);

    case GL_UNSIGNED_INT:
      return sizeof(GLuint);

    case GL_HALF_FLOAT:
      return sizeof(GLhalf);

    case GL_FLOAT:
      return sizeof(GLfloat);

    case GL_DOUBLE:
      return sizeof(GLdouble);
  }
  // default
  return sizeof(GLfloat);
}

#endif //GL_TYPES_HPP

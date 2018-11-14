#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

using std::string;
using std::vector;
using sf::Shader;  

struct Attribute {

  string name;         // as it appears in shader
  int size;            // element count
  GLenum type;         
  GLboolean normalized;
};

class Batch {

public:

  Batch(string, string, vector<Attribute>&);
  void addModel();
  
private:

  GLuint VAO, VBO, shaderHandle;
  Shader shader;

  int sizeofGLType(GLenum);
};

Batch::Batch(string vertFilename, string fragFilename, vector<Attribute> &attrs)
  : shader() {

  if (!shader.loadFromFile(vertFilename, fragFilename)) {
    std::cout << "Batch failed to load shader " << vertFilename
              << ", " << fragFilename << std::endl;
  }

  this->shaderHandle = shader.getNativeHandle();

  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  
  glBindVertexArray(this->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

  unsigned int stride = 0;

  for (auto const& attr : attrs) {

    stride += attr.size * sizeofGLType(attr.type);
  }

  unsigned long offset = 0;
  
  for (auto const& attr : attrs) {

    GLint loc = glGetAttribLocation(shaderHandle, attr.name.c_str());
    glEnableVertexAttribArray(loc);

    glVertexAttribPointer(loc, attr.size, attr.type,
                          attr.normalized, stride, (GLvoid*)offset);

    offset += attr.size * sizeofGLType(attr.type);
  }

  // clear GL state bindings
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

int Batch::sizeofGLType(GLenum type) {
  
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

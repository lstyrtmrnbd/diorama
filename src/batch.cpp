
using std::string;
using std::vector;
using sf::Shader;  

struct Attribute {

  string name;
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
    cout << "Batch failed to load shader " << vertFilename
         << ", " << fragFilename << endl;
  }

  this->shaderHandle = shader.getNativeHandle();

  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  
  glBindVertexArray(this->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

  int stride = 0;

  for (auto& attr : attrs) {

    stride += attr.size * sizeofGLType(attr.type);
  }

  int offset = 0;
  
  for (auto& attr : attrs) {

    GLint loc = glGetAttribLocation(shaderHandle, attr.name);
    glEnableVertexAttribArray(loc);

    glVertexAttribPointer(loc, attr.size, attr.type, stride, (GLvoid*)offset);

    offset += attr.size * sizeofGLType(attr.type);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Batch::sizeofGLType(GLenum type) {
  
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
}


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
  
};

Batch::Batch(string vertFilename, string fragFilename, vector<Attribute> &attrs)
  : shader() {

  if (!shader.loadFromFile(vertFilename, fragFilename)) {
    cout << "Batch failed to load shader " << vertFilename
         << ", " << fragFilename << endl;
  }

  shaderHandle = shader.getNativeHandle();

  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  
  glBindVertexArray(this->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

  int stride = 0;

  for (auto& attr : attrs) {

    stride += 
  }
  
  for (auto& attr : attrs) {
    GLint loc = glGetAttribLocation(shaderHandle, attr.name);
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, attr.size, attr.type, );
  }
}

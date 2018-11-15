#ifndef TYPED_BATCH_HPP
#define TYPED_BATCH_HPP

/**
 * The important calls are: 
 * glVertexAttribPointer(location, size, typeEnum, stride, offset)
 * and:
 * glBufferData(GL_ARRAY_BUFFER, sizeof, data, GL_DYNAMIC_DRAW) 
 */

namespace typed {

  template <typename T, int N>
  struct Attribute {

    Attribute(string handle, bool normal) : name(handle), normalized(normal){};
    string name;
    static const int size = N;
    typedef T type;
    GLboolean normalized;
    
  };

  template <typename... Attrs>
  class Batch {

  public:
    Batch(Attrs...);
    void addModel(vector<typename Attrs::type>&...);
  };
}

namespace {

  void typedBatchExample() {
    using typed::Attribute;
    using typed::Batch;
    
    Attribute<GLfloat, 3> position("position", false);
    Attribute<GLfloat, 4> color("color", true);
    Attribute<GLfloat, 2> tex("texCoords", true)
      
    Batch<Attribute<GLfloat, 3>,
          Attribute<GLfloat, 4>,
          Attribute<GLfloat, 2>> posColorTexBatch(position, color, tex);   

    Batch.addModel();
  }

  // V.S.

  void batchExample() {
    
    Attribute position = { "position", 3, GL_FLOAT, false };
    Attribute color = { "color", 4, GL_FLOAT, true};
    Attribute tex = { "texCoords", 2, GL_FLOAT, true};

    vector<Attribute> attributes = { position, color, tex };
    Batch posColorTexBatch("fs.frag", "vs.frag", attributes);
  }
}

#endif // TYPED_BATCH_HPP

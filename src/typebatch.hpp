#ifndef TYPED_BATCH_HPP
#define TYPED_BATCH_HPP

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

#include <GL/glew.h>

#include "attribute.hpp"
#include "gltypes.hpp"
#include "model.hpp"
#include "utility.hpp"

/**
 * The important calls are: 
 * - glVertexAttribPointer(location, size, typeEnum, stride, offset)
 * - glBufferData(GL_ARRAY_BUFFER, sizeof, data, GL_DYNAMIC_DRAW) 
 * - glBufferSubData(GL_ARRAY_BUFFER, offset, size, const GLvoid * data)
 */

/**
 * Implementation options:
 * - Elements are interleaved in VBO
 * - VBO is STATIC and reconstructed per draw
 */

using std::tuple, std::string, std::vector, sf::Shader;

template <typename... Attrs>
class Batch {

private:
    
  tuple<Attrs...> attributes;
  GLuint VAO, VBO, shaderHandle;
  Shader shader;

  vector<Model<Attrs...>> models;
  long vertCount;
  size_t vertSize;

public:

  Batch(string vertFilename, string fragFilename, Attrs... args)
    : attributes{args...}, shader() {

    if (!shader.loadFromFile(vertFilename, fragFilename)) {
      std::cout << "Batch failed to load shader " << vertFilename
                << ", " << fragFilename << std::endl;
    }
          
    shaderHandle = shader.getNativeHandle();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
      
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    unsigned int stride = 0;

    auto countStride =
      [&stride](const auto& attr) {

        typedef typename std::decay<decltype(attr)>::type::type aType;

        stride += attr.size * sizeof(aType);
      };

    forTuple(attributes, countStride);

    unsigned long offset = 0;

    auto vertexAttribPointer =
      [&offset, stride, this](const auto& attr) {

        typedef typename std::decay<decltype(attr)>::type::type aType;
        
        GLint loc = glGetAttribLocation(shaderHandle, attr.name.c_str());
        glEnableVertexAttribArray(loc);
                                   
        glVertexAttribPointer(loc, attr.size, getEnum<aType>::value,
                              attr.normalized, stride, (GLvoid*)offset);
        
        offset += attr.size * sizeof(aType);
      };

    forTuple(attributes, vertexAttribPointer);

    // clear GL state bindings
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }


  void addModel(Model<Attrs...>& model) {

    vertCount += model.getVertCount();

    vertSize += model.getVertSize();
    
    models.push_back(std::move(model));
  }
  
  void pushModels() {

    glNamedBufferData(VBO, vertSize, NULL, GL_STATIC_DRAW);

    for (auto& model : models) {

      bufferTupleVectors(model.getVertices(), VBO);
    }
  }
  
  void draw() {

    glBindVertexArray(VAO);
    
    pushModels();
    
    sf::Shader::bind(&shader);

    glDrawArrays(GL_TRIANGLES, 0, vertCount);    
  }
  
};

#endif // TYPED_BATCH_HPP

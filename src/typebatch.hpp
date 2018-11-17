#ifndef TYPED_BATCH_HPP
#define TYPED_BATCH_HPP

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include <SFML/Graphics.hpp>

#include <GL/glew.h>

#include "attribute.hpp"
#include "utility.hpp"
#include "gltypes.hpp"

/**
 * The important calls are: 
 * glVertexAttribPointer(location, size, typeEnum, stride, offset)
 * and:
 * glBufferData(GL_ARRAY_BUFFER, sizeof, data, GL_DYNAMIC_DRAW) 
 */

/**
 * Elements are interleaved in VBO
 * Benchmark later: make batch parent or interface,
 *   give interleaved and non-interleaved implementations 
 */

using std::tuple, std::string, std::vector, sf::Shader;

template <typename... Attrs>
class Batch {

private:
    
  tuple<Attrs...> attributes;
  GLuint VAO,VBO, shaderHandle;
  Shader shader;

  template <typename FuncT>
  void forAttr(FuncT func) {
    
    forTuple(attributes, func);
  }

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

    forAttr(countStride);

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

    forAttr(vertexAttribPointer);

    // clear GL state bindings
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  // vector<glm::vecX>& per attribute
  void addModel(vector<typename vecType<Attrs>::type> const&... args) {

    auto vecs = std::make_tuple(args...);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // zip arg vector of vecs into single vector
    // glBufferData on that vectors.data
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

};

#endif // TYPED_BATCH_HPP

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
 * - glVertexAttribPointer(location, size, typeEnum, stride, offset)
 * - glBufferData(GL_ARRAY_BUFFER, sizeof, data, GL_DYNAMIC_DRAW) 
 * - glBufferSubData(GL_ARRAY_BUFFER, offset, size, const GLvoid * data)
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
  // need to save and supply offset
  void addModel(vector<typename vecType<Attrs>::type> const&... args) {

    auto vecs = std::make_tuple(args...);

    size_t size = 0;
    
    auto countSize =
      [&size](auto& vec){

        for (auto& elt : vec) {
          typedef typename std::decay<decltype(elt)>::type::value_type vtype;
          size += sizeof(vtype) * elt.length();
        }
      };

    forTuple(vecs, countSize);
    glNamedBufferData(VBO, size, NULL, GL_DYNAMIC_DRAW);
    
    bufferTupleVectors(vecs, VBO);
  }

  void draw() {

    glBindVertexArray(VAO);
    
    sf::Shader::bind(&shader);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
  }
  
};

#endif // TYPED_BATCH_HPP

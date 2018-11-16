#ifndef TYPED_BATCH_HPP
#define TYPED_BATCH_HPP

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <SFML/Graphics.hpp>

#include <GL/glew.h>

#include "gltypes.hpp"
/**
 * The important calls are: 
 * glVertexAttribPointer(location, size, typeEnum, stride, offset)
 * and:
 * glBufferData(GL_ARRAY_BUFFER, sizeof, data, GL_DYNAMIC_DRAW) 
 */

using std::tuple, std::string, std::vector, sf::Shader;

// TUPLE FOR_EACH
template <size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp)>::type
  for_each(tuple<Tp...> &, FuncT)
  { }

template <size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp)>::type
  for_each(tuple<Tp...>& t, FuncT f) {
  
    f(std::get<I>(t));
    for_each<I + 1, FuncT, Tp...>(t, f);
  }
  
template <typename T, int N>
struct Attribute {

  Attribute(string handle, bool normal) : name(handle), normalized(normal) {};
  string name;
  static const int size = N;
  typedef T type;
  GLboolean normalized;
    
};

template <typename... Attrs>
class Batch {

private:
    
  tuple<Attrs...> attributes;
  GLuint VAO,VBO, shaderHandle;
  Shader shader;

  template <typename FuncT>
  void forAttr(FuncT func) {
    
    for_each(attributes, func);
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
      [&stride](const auto attr) {

        typedef typename decltype(attr)::type aType;

        stride += attr.size * sizeof(aType);
      };

    forAttr(countStride);

    unsigned long offset = 0;

    auto vertexAttribPointer =
      [&offset, stride, this](const auto attr) {

        typedef typename decltype(attr)::type aType;
        
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
  
  void addModel(vector<typename Attrs::type>&...){};

};

#endif // TYPED_BATCH_HPP

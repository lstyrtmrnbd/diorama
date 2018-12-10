#ifndef TYPED_BATCH_HPP
#define TYPED_BATCH_HPP

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

#include <GL/glew.h>

#include "attribute.hpp"
#include "camera.hpp"
#include "gltypes.hpp"
#include "model.hpp"
#include "utility.hpp"

/**
  The important calls are: 
  - glVertexAttribPointer(location, size, typeEnum, stride, offset)
  - glBufferData(GL_ARRAY_BUFFER, sizeof, data, GL_DYNAMIC_DRAW) 
  - glBufferSubData(GL_ARRAY_BUFFER, offset, size, const GLvoid * data)
*/

/**
  Implementation decisions:
  - Elements are interleaved in VBO
  - VBO is STATIC and reconstructed per draw
*/

using std::tuple, std::string, std::vector, std::shared_ptr, sf::Shader, sf::Texture;

template <typename... Attrs>
class Batch {

private:
    
  tuple<Attrs...> attributes;
  GLuint VAO, VBO, shaderHandle;
  Shader shader;

  vector<Model<Attrs...>> models;
  long vertCount;
  size_t vertSize;

  vector<Texture> textures;
  
  shared_ptr<Camera> camera;

public:
  
  Batch(string vertFilename, string fragFilename, Attrs... args)
    : attributes{args...}, shader{}, camera{nullptr} {

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

  //offsetting
  void bufferModels() {

    glNamedBufferData(VBO, vertSize, NULL, GL_STATIC_DRAW);

    size_t offset = 0;
    
    for (auto& model : models) {
      auto worldVerts = model.getWorldVertices();
      offset += bufferTupleVectors(worldVerts, VBO, offset);
    }
  }

  void setCamera(Camera * newCamera) {

    camera = shared_ptr<Camera>(newCamera);
  }

  shared_ptr<Camera> getCamera() {

    return camera;
  }
  
  void draw() {

    glBindVertexArray(VAO);
    
    bufferModels();
    
    sf::Shader::bind(&shader);

    if (camera != nullptr) {

      GLuint viewProjection = glGetUniformLocation(shaderHandle, "viewProjection");
      glUniformMatrix4fv(viewProjection, 1, GL_FALSE, &camera->getVP()[0][0]);
    } else {

      std::cout << "No camera available in batch" << std::endl;
    }

    glDrawArrays(GL_TRIANGLES, 0, vertCount);    
  }
  
};

#endif // TYPED_BATCH_HPP

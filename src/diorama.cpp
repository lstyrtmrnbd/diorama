#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "typebatch.hpp"

using glm::vec2;
using glm::vec3;
using glm::mat4;
using std::string;

int main() {

  sf::ContextSettings settings(24, 0, 0, 4, 5); //depth, stencil, AA, major, minor
  
  sf::Window window(sf::VideoMode(800, 600), "Diorama", sf::Style::Default, settings);
  window.setVerticalSyncEnabled(true);

  window.setActive(true);

  // load resources, initialize the OpenGL states...

  glewInit();

  glClearColor(0.5f,0.5f,0.5f,1.0f);
  glEnable(GL_DEPTH_TEST);

  /// BUFFERS
  // By default, counterclockwise polygons are taken to be front-facing
  vec3 vertices[6];
  vertices[0] = vec3(-0.5f, 1.0f,0.0f);
  vertices[1] = vec3(-0.5f,-1.0f,0.0f);
  vertices[2] = vec3( 0.5f,-1.0f,0.0f);

  vertices[3] = vec3( 0.5f,-1.0f,0.0f);
  vertices[4] = vec3( 0.5f, 1.0f,0.0f);
  vertices[5] = vec3(-0.5f, 1.0f,0.0f);

  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  vec2 texCoords[6];
  texCoords[0] = vec2( 0.0f, 1.0f);
  texCoords[1] = vec2( 0.0f, 0.0f);
  texCoords[2] = vec2( 1.0f, 0.0f);

  texCoords[3] = vec2( 1.0f, 0.0f);
  texCoords[4] = vec2( 1.0f, 1.0f);
  texCoords[5] = vec2( 0.0f, 1.0f);

  GLuint texCoordBuffer;
  glGenBuffers(1, &texCoordBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

  Attribute<GLfloat, 3> position {"position", false};
  Attribute<GLfloat, 2> texCoord {"texCoords", true};

  /**
  Batch<Attribute<GLfloat, 3>,
        Attribute<GLfloat, 2>> batch {"src/simple.vert", "src/simple.frag",
                                      position, texCoord};
  */
  auto batch = std::make_unique<Batch<Attribute<GLfloat, 3>,
                                      Attribute<GLfloat, 2>>>("src/simple.vert", "src/simple.frag", position, texCoord);
  
  vector<vec3> vverts = {{-1.0f, 1.0f, 0.0f },
                         {-1.0f,-1.0f, 0.0f },
                         { 1.0f,-1.0f, 0.0f },

                         { 1.0f,-1.0f, 0.0f },
                         { 1.0f, 1.0f, 0.0f },
                         {-1.0f, 1.0f, 0.0f }};

  vector<vec2> vtexs = {{ 0.0f, 1.0f },
                        { 0.0f, 0.0f },
                        { 1.0f, 0.0f },

                        { 1.0f, 0.0f },
                        { 1.0f, 1.0f },
                        { 0.0f, 1.0f }};

  Model<Attribute<GLfloat, 3>,
        Attribute<GLfloat, 2>> square {vverts, vtexs};

  square.scale(vec3(0.5f,0.5f,1.0f));
  
  batch->addModel(square);
  
  /**
  /// TEXTURE
  sf::Image defaultImage;
  defaultImage.loadFromFile("poster0.jpg");

  sf::Texture defaultTexture;
  defaultTexture.loadFromImage(defaultImage);

  GLuint texHandle = defaultTexture.getNativeHandle();

  /// MATRICES
  mat4 translation(1.0f);
  mat4 rotation(1.0f);
  mat4 scale = glm::scale(mat4(1.0f), vec3(0.5f,0.5f,0.5f));

  mat4 model = translation * rotation * scale;
  mat4 view(1.0f);
  mat4 projection(1.0f);

  GLuint modelLoc = glGetUniformLocation(shaderHandle, "model");
  GLuint viewLoc = glGetUniformLocation(shaderHandle, "view");
  GLuint projectionLoc = glGetUniformLocation(shaderHandle, "projection");
  */
  /// LOOP
  bool running = true;
  while (running) {
    
    sf::Event event;
    while (window.pollEvent(event)) {

      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Escape) {

        running = false;
      }

      if (event.type == sf::Event::Closed) {
        
        running = false;
      }
      else if (event.type == sf::Event::Resized) {
          
        glViewport(0, 0, event.size.width, event.size.height); // adjust the viewport when the window is resized
      }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /**
    glActiveTexture(GL_TEXTURE0);
    sf::Texture::bind(&defaultTexture);

    defaultShader.setUniform("texture0", defaultTexture);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    */

    batch->draw();
    
    // end the current frame (internally swaps the front and back buffers)
    window.display();
  }

  // release resources...

  return 0;
}

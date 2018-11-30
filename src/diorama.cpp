#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "typebatch.hpp"

using glm::vec2, glm::vec3;

int main() {

  sf::ContextSettings settings(24, 0, 0, 3, 3); //depth, stencil, AA, major, minor
  
  sf::Window window(sf::VideoMode(800, 600), "Diorama", sf::Style::Default, settings);
  window.setVerticalSyncEnabled(true);

  window.setActive(true);

  //GL prep

  glewInit();

  glClearColor(0.5f,0.5f,0.5f,1.0f);
  glEnable(GL_DEPTH_TEST);

  //BATCH
  
  Attribute<GLfloat, 3> position {"position", false};
  Attribute<GLfloat, 2> texCoord {"texCoords", true};

  auto batch = std::make_unique<Batch<Attribute<GLfloat, 3>,
                                      Attribute<GLfloat, 2>>>("src/simple.vert", "src/simple.frag", position, texCoord);

  // By default, counterclockwise polygons are taken to be front-facing
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

  square.scale(vec3(0.25f,0.25f,1.0f));
  square.translate(vec3(-0.5f,0.0f,0.0f));

  batch->addModel(square);

  Model<Attribute<GLfloat, 3>,
        Attribute<GLfloat, 2>> square2 {vverts, vtexs};

  square2.scale(vec3(0.25f,0.25f,1.0f));
  square2.translate(vec3(0.5f,0.0f,0.0f));

  batch->addModel(square2);
  
  /**
  /// TEXTURE
  sf::Image defaultImage;
  defaultImage.loadFromFile("poster0.jpg");

  sf::Texture defaultTexture;
  defaultTexture.loadFromImage(defaultImage);

  GLuint texHandle = defaultTexture.getNativeHandle();


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

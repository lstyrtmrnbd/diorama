#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>

int main() {

  using glm::vec3;
  using std::string;

  sf::ContextSettings settings(24, 0, 0, 4, 5); //depth, stencil, AA, major, minor
  
  sf::Window window(sf::VideoMode(800, 600), "Diorama", sf::Style::Default, settings);
  window.setVerticalSyncEnabled(true);

  // activate the window
  window.setActive(true);

  // load resources, initialize the OpenGL states, ...

  glewInit();

  glClearColor(0.5f,0.5f,0.5f,1.0f);

  vec3 vertices[3];
  vertices[0] = vec3(0.5f,0.5f,0.0f);
  vertices[1] = vec3(0.0f,0.0f,0.0f);
  vertices[2] = vec3(1.0f,0.0f,0.0f);
  
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  const string vertSrc = "in vec3 position;"\
                         "void main() {"\
                         " gl_Position = vec4(position.x,position.y,position.z,1.0);"\
                         "}";

  const string fragSrc = "void main() {"\
                         "gl_FragColor = vec4(0.0,1.0,0.5,1.0);"\
                         "}";

  sf::Shader defaultShader;

  if (!defaultShader.loadFromMemory(vertSrc, fragSrc)) std::cout << "shader fail\n";
  
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
    
    sf::Shader::bind(&defaultShader);

    glEnableVertexAttribArray(0);   
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //tell pipeline how to interpret data in the buffer

    glDrawArrays(GL_TRIANGLES, 0 , 3);

    glDisableVertexAttribArray(0);
    
    window.display(); // end the current frame (internally swaps the front and back buffers)
  }

  // release resources...

  return 0;
}

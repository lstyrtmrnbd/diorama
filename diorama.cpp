#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
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
  
  const string vertSrc = "#version 330\n"\
                         "in vec3 position;"\
                         "in vec2 texCoord;"\
                         "out vec2 texCoordV;"\
                         "void main() {"\
                         "  texCoordV = texCoord;"\
                         "  gl_Position = vec4(position.x,position.y,position.z,1.0);"\
                         "}";

  const string fragSrc = "#version 330\n"\
                         "in vec2 texCoordV;"\
                         "uniform sampler2D texture0;"\
                         "void main() {"\
                         "  vec3 tex = texture2D(texture0, texCoordV).rgb;"\
                         "  vec3 color = vec3(0.0, 1.0, 0.5);"\
                         "  gl_FragColor = vec4(tex, 1.0);"\
                         "}";

  sf::Shader defaultShader;

  if (!defaultShader.loadFromMemory(vertSrc, fragSrc)) std::cout << "#####Default shader failed\n";

  GLuint shaderHandle = defaultShader.getNativeHandle();

  GLint positionLoc = glGetAttribLocation(shaderHandle, "position");
  GLint texCoordLoc = glGetAttribLocation(shaderHandle, "texCoord");

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  glEnableVertexAttribArray(positionLoc);
  glEnableVertexAttribArray(texCoordLoc);
    
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
  glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_TRUE, 0, 0);
  
  
  sf::Image defaultImage;
  defaultImage.loadFromFile("poster0.jpg");

  sf::Texture defaultTexture;
  defaultTexture.loadFromImage(defaultImage);

  GLuint texHandle = defaultTexture.getNativeHandle();

  
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

    // shader uniforms set with
    //defaultShader.setUniform("uName", 1.0f); // overloads for sfml and sfml/glsl types

    // overloaded for sf::Texture& as well
    //shader.setUniform("texture", texture);
    
    sf::Shader::bind(&defaultShader);

    glActiveTexture(GL_TEXTURE0);
    //glUniform1i("texture0", /*GL_TEXTURE*/0);
    sf::Texture::bind(&defaultTexture);

    defaultShader.setUniform("texture0", defaultTexture);
    
    glDrawArrays(GL_TRIANGLES, 0 , 6);

    //glDisableVertexAttribArray(positionLoc);
    
    window.display(); // end the current frame (internally swaps the front and back buffers)
  }

  // release resources...

  return 0;
}

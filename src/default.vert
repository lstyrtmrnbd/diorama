#version 330

in vec3 position;
in vec2 texCoord;
out vec2 texCoordV;                           

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

  mat4 mvp = projection * view * model;
  
  texCoordV = texCoord;                                              
  gl_Position = mvp * vec4(position,1.0);      
}

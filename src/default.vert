#version 330

in vec3 position;
in vec2 texCoord;
out vec2 texCoordV;                           

void main() {
  
  texCoordV = texCoord;                                               
  gl_Position = vec4(position.x,position.y,position.z,1.0);           
}

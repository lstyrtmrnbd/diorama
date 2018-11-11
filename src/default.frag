#version 330

in vec2 texCoordV;
uniform sampler2D texture0;
out vec4 fragColor;

void main() {

  vec2 flipCoords = vec2(texCoordV.x, 1.0 - texCoordV.y);
  vec3 tex = texture2D(texture0, flipCoords).rgb;
  vec3 color = vec3(0.0, 1.0, 0.5);
  fragColor = vec4(tex, 1.0);
}

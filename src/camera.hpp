#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity.hpp"

using glm::vec3, glm::mat4;

class Camera : public Entity {
  
private:

  mat4 view, projection;

  //vec3 position, target, up;

  vec3 target, up;

public:

  Camera();
  
  void lookAt(vec3 target);

  void move(vec3 offset);
  
  void setPerspective(float fov, float aspect, float nearClip, float farClip);
  void setOrthographic(float left, float right, float bottom,
                       float top, float zNear, float zFar);
  
  mat4 getView() const;
  mat4 const & getProjection() const { return projection; }
  mat4 getVP() const;

  vec3 getPosition() const { return position; }
  vec3 getTarget() const { return target; }
  vec3 getUp() const { return up; }
};

#endif//CAMERA_HPP

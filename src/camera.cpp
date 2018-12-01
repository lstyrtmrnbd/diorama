#include "camera.hpp"

Camera::Camera(): view{1.0f}, projection{1.0f}, position{0.0f,0.0f,-1.0f},
    target{0.0f,0.0f,0.0f}, up{0.0f,1.0f,0.0f} {

  lookAt(vec3(0.0f,0.0f,0.0f));
  setPerspective(90.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}

void Camera::move(vec3 offset) {

  position += offset;
  target += offset;
}

void Camera::setPosition(vec3 newPosition) {

  position = newPosition;
  target += newPosition;
}

void Camera::lookAt(vec3 target) {

  target = target;
}

void Camera::setPerspective(float fov, float aspect, float nearClip, float farClip) {

  projection = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
}

void Camera::setOrthographic(float left, float right, float bottom,
                       float top, float zNear, float zFar) {

  projection = glm::ortho(left, right, bottom, top, zNear, zFar);
}

mat4 Camera::getView() const {

  return glm::lookAt(position, target, up);
}

mat4 Camera::getVP() const {

  return projection * glm::lookAt(position, target, up);
}

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <functional>

#include <SFML/System/Time.hpp>

#include <glm/vec3.hpp>

using std::function, sf::Time, glm::vec3;

class Entity {

private:

  double speed;
  
  vec3 position, direction;

  function<vec3(vec3, Time, Time)> movefun = [](vec3 position, Time step, Time total)
                                             { return position; };
  
public:

  Entity(vec3 pos, vec3 movef) : position{pos}, movefun{movef}
  {}

  void move(vec3 offset) { position += offset; }

  void setPosition(vec3 position) { this->position = position; }
  vec3 getPosition() const { return position; }

  void setDirection(vec3 direction) { this->direction = direction; }
  vec3 getDirection() const { return direction; }

  void update(Time step, Time total) { position = movefun(step, total); }
  
};

#endif//ENTITY_HPP

#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <vector>

#include <SFML/System/Time.hpp>

#include "entity.hpp"

using sf::Time, std::vector;

class Driver {

private:
  
  const Time dt;

  vector<Entity*> entities;

public:
  
  Driver(Time timestep) : dt{timestep}, entities{}
  {}
  
  void simulatePeriod(vector<Entity>& entities, Time& period, Time& total) {

    while (period >= dt) {

      for (auto& entity : entities) {

        entity.update(dt, total);
      }
      
      period -= dt;
      total += dt;
    }
  }

  void addEntity(Entity* ent) { entities.push_back(ent); }
};

#endif//DRIVER_HPP

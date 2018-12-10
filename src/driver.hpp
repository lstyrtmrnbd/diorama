#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <vector>

#include <SFML/System.hpp>

class Driver {

  using sf::Time, std::vector;

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
};

#endif//DRIVER_HPP

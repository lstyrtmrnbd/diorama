#ifndef ATTRIBUTE_HPP
#define ATTRIBUTE_HPP

#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>

using std::string;

template <typename T, int N>
struct Attribute {

  Attribute(string handle, bool normal)
    : name(handle), normalized(normal)
  {}
  
  string name;
  static const int size = N;
  typedef T type;
  GLboolean normalized;
    
};

// gives appropriate glm::vec type to hold attribute components
template <typename Ta>
struct vecType {

  typedef typename glm::vec<Ta::size, typename Ta::type, glm::defaultp> type;
};


#endif//ATTRIBUTE_HPP

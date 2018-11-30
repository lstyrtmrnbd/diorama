#ifndef MODEL_HPP
#define MODEL_HPP

#include <tuple>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "attribute.hpp"
#include "camera.hpp"
#include "utility.hpp"

using std::tuple, std::vector, std::get;
using glm::vec3, glm::vec4, glm::mat4;

template <typename... Attrs>
class Model {

  // tuple of vectors of glm::vecs
  using vertTuple = tuple<vector<typename vecType<Attrs>::type>...>;
  
private:
  
  vertTuple vertices;
  long vertCount;
  mat4 translation, rotation, scalation;
  
public:

  Model(vector<typename vecType<Attrs>::type>... args)
    : vertices{args...}, vertCount{static_cast<long>(get<0>(vertices).size())},
      translation{1.0f}, rotation{1.0f}, scalation{1.0f}
  {}

  vertTuple & getVertices() { return vertices; }

  vertTuple getWorldVertices() {

    mat4 trs = translation * rotation * scalation;
    
    vertTuple worldVerts = vertices;
    
    for (auto& vec : get<0>(worldVerts)) {

      vec4 temp(vec, 1.0);
      temp = trs * temp;
      vec.x = temp.x;
      vec.y = temp.y;
      vec.z = temp.z;
    }

    return worldVerts;
  }

  long getVertCount() { return vertCount; }

  size_t getVertSize() {

    size_t size;
    
    auto countSize =
      [&size](auto& vec) {

        for (auto& elt : vec) {
          typedef typename std::decay<decltype(elt)>::type::value_type vtype;
          size += sizeof(vtype) * elt.length();
        }
      };

    forTuple(vertices, countSize);

    return size;
  }

  void translate(vec3 offset) {

    translation = glm::translate(mat4(1.0f), offset) * translation;
  }

  void scale(vec3 scalars) {

    scalation = glm::scale(mat4(1.0f), scalars) * scalation;
  }

  void rotate(float angle, vec3 axis) {

    rotation = glm::rotate(mat4(1.0f), angle, axis) * rotation;
  }

};

#endif//MODEL_HPP

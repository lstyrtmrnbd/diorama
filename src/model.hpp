#ifndef MODEL_HPP
#define MODEL_HPP

#include <tuple>
#include <vector>

#include "attribute.hpp"
#include "utility.hpp"

using std::tuple, std::vector, std::get;

template <typename... Attrs>
class Model {

  using vertTuple = tuple<vector<typename vecType<Attrs>::type>...>;
  
private:
  
  vertTuple vertices;
  long vertCount;
  
public:

  Model(vector<typename vecType<Attrs>::type>... args)
    : vertices{args...}, vertCount{static_cast<long>(get<0>(vertices).size())}
  {}

  vertTuple & getVertices() { return vertices; }

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

};

#endif//MODEL_HPP

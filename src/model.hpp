#ifndef MODEL_HPP
#define MODEL_HPP

#include <tuple>
#include <vector>

#include "attribute.hpp"

using std::tuple, std::vector;

template <typename... Attrs>
class Model {

  using vertTuple = tuple<vector<typename vecType<Attrs>::type>...>;
  
private:
  tuple<vector<typename vecType<Attrs>::type>...> vertices;

public:

  Model(vector<typename vecType<Attrs>::type>... args) : vertices{args...}
  {}

  vertTuple const & getVertices() { return vertices; }

};

#endif//MODEL_HPP

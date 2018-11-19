#ifndef FOR_HPP
#define FOR_HPP

#include <tuple>
#include <vector>

#include <glm/glm.hpp>

/**
 * Generic iteration over common structures
 */

using std::tuple, std::vector, glm::vec;

// TUPLE FOR_EACH
template <size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp)>::type
  forTuple(tuple<Tp...> &, FuncT)
  { }

template <size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp)>::type
  forTuple(tuple<Tp...>& t, FuncT f) {
  
    f(std::get<I>(t));
    forTuple<I + 1, FuncT, Tp...>(t, f);
  }

// GLM::VEC FOR_EACH
template <typename Vec, typename FuncT>
void forVec(Vec v, FuncT f) {

  for(auto i = 0; i < v.length(); ++i) {

    f(v[i]);
  }
}

// Buffers a glm::vec into bound GL_ARRAY_BUFFER,
// returns size of buffered data so it can be added to offset
template <typename Vec>
size_t bufferVec(const Vec& v, size_t offset) {

  typedef typename Vec::value_type vtype;
  
  vector<vtype> data;

  auto fillData =
    [&data](const auto& vi){

      data.push_back(vi);
    };

  forVec(v, fillData);

  size_t size = sizeof(vtype) * data.size();
  
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, (GLvoid*)data.data());

  return size;
}

// Proceed up the first vector in the tuple, buffering a vec
//  per tuple member vector each step
template <typename... Tvs>
void bufferTupleVectors(tuple<Tvs...>& tup, GLuint buffer) {

  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  
  size_t offset = 0;
  size_t i = 0;

  auto perVector =
    [&offset, &i](auto& vect){

      offset += bufferVec(vect[i], offset);
    };
  
  auto& firstVector = std::get<0>(tup);
  for (; i < firstVector.size(); ++i) {

    forTuple(tup, perVector);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#endif//FOR_HPP

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

// Flattens a tuple of vectors of vecs into a vector of T
// All vectors in the tuple must be at least as long as the first
/**
template <typename T, typename... Tvs>
vector<T> flattenTupVectVec(tuple<Tvs...>& tup) {

  vector<T> ret;

  auto tupLength = std::tuple_size<tup>::value;
  auto firstLength = std::get<0>(tup).size();

  auto unpackVecs =
    [&ret](auto veci) {

      ret.push_back(veci);
    };

  auto unpackAtI =
    [&tup](auto i){

      auto perColumn =
        [i](const auto& vect){

          forVec(vect[i], unpackVecs); 
        };
      
      forTuple(tup, perColumn);
    };
  
  for (auto i = 0; i < firstLength; ++i) unpackAtI(i);

  return ret;
}
*/
#endif//FOR_HPP

#ifndef TOOLS_H
#define TOOLS_H
#include <vector>
#include <QString>

namespace tool {

template<typename T, typename ...Args>
static inline auto make_(Args &&... args) {
  return new T(std::forward<Args>(args)...);
}

static auto sub_by_index(const QString &str, int begin, int end)->QString {
  return str.mid(begin, end - begin);
}

static auto out_sub_by_index(const QString &str, int begin, int end)->QString {
  return str.left(begin).append(str.right(str.size() - end));
}


/**
 * @brief get mid point of two integer
 * @param[in] lhs
 * @param[in] rhs
 * @return mid point of lhs and rhs
 * @exception noexcept
 */
static inline auto mid_value_of_integer(int lhs, int rhs) noexcept -> int {
  return ((lhs bitand rhs) + ((lhs xor rhs) >> 1));
}

}
#endif // TOOLS_H

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

}
#endif // TOOLS_H

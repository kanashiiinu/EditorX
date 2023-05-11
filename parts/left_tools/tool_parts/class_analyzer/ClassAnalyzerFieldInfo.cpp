#include "ClassAnalyzerFieldInfo.hpp"


auto to_str(FieldInfo field)->QString {
  auto front = field.access + ": ";
  for (auto &w : field.front_words) {
    front.append(w + " ");
  }
  front.append(field.type + " " + field.name);
  if (field.default_value.isEmpty()) {
    return front;
  }
  return front + " = " + field.default_value;
}

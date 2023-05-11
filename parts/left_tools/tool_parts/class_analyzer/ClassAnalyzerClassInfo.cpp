#include "ClassAnalyzerClassInfo.hpp"

auto to_str(ClassInfo c)->QString {
  auto cs = c.class_name + "\n";
  for (const auto &f : c.fields) {
    cs.append(to_str(f) + "\n");
  }
  for (const auto &f : c.functions) {
    cs.append(to_str(f) + "\n");
  }
  return cs;
}

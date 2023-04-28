#include "ClassAnalyzerFunctionInfo.hpp"


auto to_str(FunctionInfo func)->QString {
  auto front = func.access + ": ";
  for (auto &w : func.front_words) {
    front.append(w + " ");
  }
  front.append(func.type + " " + func.name + func.param + " ");
//  qDebug() << func.back_words;
  for (auto &w : func.back_words) {
    front.append(w + " ");
  }
  front.append(func.body);
  return front.trimmed();
}

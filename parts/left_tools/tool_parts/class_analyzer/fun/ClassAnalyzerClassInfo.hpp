#ifndef CLASSANALYZERCLASSINFO_HPP
#define CLASSANALYZERCLASSINFO_HPP

#include <QObject>
//self
#include "ClassAnalyzerFieldInfo.hpp"
#include "ClassAnalyzerFunctionInfo.hpp"

struct ClassInfo {
  QString class_name;
  QList<FieldInfo> fields;
  QList<FunctionInfo> functions;
};

auto to_str(ClassInfo c)->QString;

Q_DECLARE_METATYPE(ClassInfo)

#endif // CLASSANALYZERCLASSINFO_HPP

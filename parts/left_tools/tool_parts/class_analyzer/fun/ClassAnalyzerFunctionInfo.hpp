#ifndef CLASSANALYZERFUNCTIONINFO_HPP
#define CLASSANALYZERFUNCTIONINFO_HPP

#include <QObject>
#include <QString>
#include <QList>


struct FunctionInfo {
  QString access;
  QString type;
  QString name;
  QString param;
  QString body;
  QStringList front_words;
  QStringList back_words;
};

auto to_str(FunctionInfo func)->QString;

Q_DECLARE_METATYPE(FunctionInfo)


#endif // CLASSANALYZERFUNCTIONINFO_HPP

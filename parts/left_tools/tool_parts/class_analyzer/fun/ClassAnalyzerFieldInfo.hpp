#ifndef CLASSANALYZERFIELDINFO_HPP
#define CLASSANALYZERFIELDINFO_HPP

#include <QObject>
#include <QString>
#include <QList>

struct FieldInfo {
  QString access;
  QString type;
  QString name;
  QString default_value;
  QStringList front_words;
};

Q_DECLARE_METATYPE(FieldInfo)

auto to_str(FieldInfo field)->QString;

#endif // CLASSANALYZERFIELDINFO_HPP

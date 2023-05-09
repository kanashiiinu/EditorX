#ifndef THEMESMANAGER_HPP
#define THEMESMANAGER_HPP

#include <QObject>

#include <QHash>
#include <QString>
#include <vector>

class ThemesManager : public QObject
{
  Q_OBJECT
 public:
  explicit ThemesManager(QObject *parent = nullptr);
 public:
  QStringList get_theme_names() const;
  QString get_path(QString const &theme_name) const;
  bool add_theme(QString const &theme_name, QString const &theme_file_path);
  bool remove_theme_by_path(QString const &theme_file_path);
 private:
  QHash<QString, QString> m_names_to_paths;
  std::vector<QString> m_names;
 signals:

};

#endif // THEMESMANAGER_HPP

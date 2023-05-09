#include "ThemesManager.hpp"

#include "./parts/left_tools/tool_parts/theme/fun/ThemeOperation.hpp"


ThemesManager::ThemesManager(QObject *parent)
  : QObject(parent)
{
  auto qss_path = theme::get_project_default_theme_path();
  for (auto const &path : qss_path)
  {
    m_names_to_paths[theme::from_url_to_name(path)] = path;
  }
  m_names = m_names_to_paths.keys().toVector().toStdVector();
}

QStringList ThemesManager::get_theme_names() const
{
  QStringList names;
  for (auto const &name : m_names)
  {
    names.append(name);
  }
  return names;
}


QString ThemesManager::get_path(QString const &theme_name) const
{
  return m_names_to_paths.value(theme_name);
}

bool ThemesManager::add_theme(QString const &theme_name, QString const &theme_file_path)
{
  if (not m_names_to_paths.contains(theme_name))
  {
    m_names_to_paths[theme_name] = theme_file_path;
    m_names.push_back(theme_name);
    return true;
  }
  return false;
}
bool ThemesManager::remove_theme_by_path(QString const &theme_file_path)
{
  auto name = theme::from_url_to_name(theme_file_path);
  if (m_names_to_paths.value(name, "") == theme_file_path)
  {
    m_names_to_paths.remove(name);
    m_names.erase(std::find(m_names.begin(), m_names.end(), name));
    return true;
  }
  return false;
}

#pragma once

#include <QWidget>

class ThemesManager;

class QLineEdit;
class QPushButton;
class QListWidget;

class ThemeWidget: public QWidget
{
  Q_OBJECT
 public:
  ThemeWidget(QWidget *parent = nullptr);

 private:

  void _init_ui();

  void _init_text();

 private:

  ThemesManager *m_theme_manager;
 private:
  //ui
  QListWidget *m_theme_list;
  QPushButton *m_add_path_btn;
  QPushButton *m_remove_path_btn;
  QPushButton *m_cancel_btn;
  QPushButton *m_apply_btn;



};

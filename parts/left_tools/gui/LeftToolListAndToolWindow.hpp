#ifndef LEFTTOOLLISTANDTOOLWINDOW_HPP
#define LEFTTOOLLISTANDTOOLWINDOW_HPP

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QListWidget>
#include <QStackedWidget>
//self
//#include "FindAndReplaceWidget.hpp"
#include "./parts/left_tools/tool_parts/search/gui/FindAndReplaceWidget.hpp"

class LeftToolListAndToolWindow : public QWidget
{
  Q_OBJECT
 public:
  using DirPath = QString;
  explicit LeftToolListAndToolWindow(QWidget *parent = nullptr);
  auto open_directory(const DirPath &dir)->void;
 signals:

 public:
  QListWidget *m_list_widget;
  QStackedWidget *m_stacked_widget;
  FindAndReplaceWidget *m_find_and_replace_widget;
 private:
  QTreeView *m_res_manager;
  QFileSystemModel *m_file_sys_model;

};

#endif // LEFTTOOLLISTANDTOOLWINDOW_HPP

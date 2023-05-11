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
 public:
 signals:
  //
  signal_note_part_want_open_note(QString const &);
  //
  signal_code_analysis_part_want_current_file_path();
  signal_reponse_code_analysis_current_file_path(QString const &);
  //
  signal_code_format_part_want_current_file_path();
  signal_reponse_code_format_current_file_path(QString const &);
  signal_code_format_part_format_finish();

};

#endif // LEFTTOOLLISTANDTOOLWINDOW_HPP

//self
#include "LeftToolListAndToolWindow.hpp"
//#include "FindAndReplaceWidget.hpp"
#include "./parts/left_tools/tool_parts/search/gui/FindAndReplaceWidget.hpp"
//#include "MetaInfoWidget.hpp"
#include "./parts/left_tools/tool_parts/class_analyzer/gui/MetaInfoWidget.hpp"
#include "./parts/scatter/terminate/gui/ShellDemo.hpp"
#include "./parts/scatter/work_time_reminder/gui/TimeReminderWidget.hpp"
#include "./parts/left_tools/tool_parts/font_and_color/gui/FontAndColorWidget.hpp"
#include "./parts/scatter/screenshot/ScreenWidget.hpp"
#include "./parts/left_tools/tool_parts/compare_file_in_dir/gui/CompareFileWidget.hpp"
#include "./parts/left_tools/tool_parts/theme/gui/ThemeWidget.hpp"
#include "./parts/left_tools/tool_parts/note/gui/NoteWidget.hpp"
#include "./parts/left_tools/tool_parts/code_analysis/gui/CodeAnalysisWidget.hpp"
#include "./parts/left_tools/tool_parts/code_format/gui/CodeFormatWidget.hpp"

//QT
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QShortcut>
#include <QDebug>
#include <QDir>
#include <QString>
#include <QLabel>
#include <QGroupBox>
//std

LeftToolListAndToolWindow::LeftToolListAndToolWindow(QWidget *parent) : QWidget(parent)
{
  this->setContentsMargins(0, 0, 0, 0);
  //
  auto h_box = new QHBoxLayout(this);
  h_box->setContentsMargins(0, 0, 0, 0);
  this->setLayout(h_box);
  m_list_widget = new QListWidget(this);
  m_list_widget->setFrameShape(QFrame::NoFrame);
  h_box->addWidget((m_list_widget));
//  m_list_widget->addItem(QString("resource manager"));
  // folder
  m_list_widget->addItem(QString("folder"));
//  m_list_widget->setStyleSheet(R"(QListWidget{ margin: 0px; padding: 0px; border:10px; })");
  m_stacked_widget = new QStackedWidget(this);
  m_res_manager = new QTreeView(this);
  m_res_manager->setFrameShape(QFrame::NoFrame);
  m_file_sys_model = new QFileSystemModel(this);
  m_res_manager->setHeaderHidden(true);
  m_res_manager->setColumnHidden(1, true);
  m_res_manager->setColumnHidden(2, true);
  m_res_manager->setColumnHidden(3, true);
  m_stacked_widget->addWidget(m_res_manager);
  // search
  m_list_widget->addItem(QString("search"));
  m_find_and_replace_widget = new FindAndReplaceWidget(this);
  m_stacked_widget->addWidget(m_find_and_replace_widget);
  // meta
  m_list_widget->addItem(QString("meta"));
  m_stacked_widget->addWidget(new MetaInfoWidget(this));
  // terminate
  m_list_widget->addItem(QString("terminate"));
  m_stacked_widget->addWidget(new ShellDemo(this));
  // work_time_reminder
  m_list_widget->addItem(QString("time reminder"));
  m_stacked_widget->addWidget(new TimeReminderWidget(this));
  // font & color
  m_list_widget->addItem(QString("font&color"));
  m_stacked_widget->addWidget(new FontANdColorWidget(this));
  // font & color
  m_list_widget->addItem(QString("compare dir"));
  m_stacked_widget->addWidget(new CompareFileWidget(this));
  // themes
  m_list_widget->addItem(QString("themes"));
  m_stacked_widget->addWidget(new ThemeWidget(this));
  // notes
  m_list_widget->addItem(QString("notes"));
  auto note_widget = new NoteWidget(this);
  m_stacked_widget->addWidget(note_widget);
  connect(note_widget, &NoteWidget::signal_want_open_note,
          this, &LeftToolListAndToolWindow::signal_note_part_want_open_note);

  // cpp code error
  m_list_widget->addItem(QString("code analysis"));
  auto code_analysis_widget = new CodeAnalysisWidget(this);
  m_stacked_widget->addWidget(code_analysis_widget);

  connect(code_analysis_widget, &CodeAnalysisWidget::signal_analysis_wait_for_file,
          this, &LeftToolListAndToolWindow::signal_code_analysis_part_want_current_file_path);
  connect(this, &LeftToolListAndToolWindow::signal_reponse_code_analysis_current_file_path,
          code_analysis_widget, &CodeAnalysisWidget::slot_deal_analysis);

  // cpp code format
  m_list_widget->addItem(QString("code format"));
  auto code_format_widget = new CodeFormatWidget(this);
  m_stacked_widget->addWidget(code_format_widget);
  connect(code_format_widget, &CodeFormatWidget::signal_format_wait_for_file,
          this, &LeftToolListAndToolWindow::signal_code_format_part_want_current_file_path);
  connect(this, &LeftToolListAndToolWindow::signal_reponse_code_format_current_file_path,
          code_format_widget, &CodeFormatWidget::slot_deal_format);
  connect(code_format_widget, &CodeFormatWidget::signal_format_finish,
          this, &LeftToolListAndToolWindow::signal_code_format_part_format_finish);



  // 截屏快捷键
  auto shortcut = new QShortcut(QKeySequence("Ctrl+Alt+j"), this);
  connect(shortcut, &QShortcut::activated, this, []() {
    ScreenWidget::_()->showFullScreen();
  });

  //
  connect(m_list_widget, &QListWidget::currentRowChanged,
          m_stacked_widget, &QStackedWidget::setCurrentIndex);
  //
  h_box->addWidget(m_stacked_widget);
  // 布局比例
  h_box->setSpacing(0);
  h_box->setStretch(0, 1);
  h_box->setStretch(1, 3);
}

auto LeftToolListAndToolWindow::open_directory(const DirPath &dir)->void {
  qDebug() << dir;
  // 不知道为什么用原来的fileSystemModel运行会挂掉
  delete std::exchange(m_file_sys_model, new QFileSystemModel(this));
  m_file_sys_model->setRootPath(dir);
  m_res_manager->setModel(m_file_sys_model);
  m_res_manager->setRootIndex(m_file_sys_model->setRootPath(dir));
}

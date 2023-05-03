//self
#include "LeftToolListAndToolWindow.hpp"
//#include "FindAndReplaceWidget.hpp"
#include "./parts/left_tools/tool_parts/search/gui/FindAndReplaceWidget.hpp"
//#include "MetaInfoWidget.hpp"
#include "./parts/left_tools/tool_parts/class_analyzer/gui/MetaInfoWidget.hpp"
#include "./parts/scatter/terminate/gui/ShellDemo.hpp"
#include "./parts/scatter/work_time_reminder/gui/TimeReminderWidget.hpp"
//Qt
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QTreeView>
#include <QFileSystemModel>
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
  //
  connect(m_list_widget, &QListWidget::currentRowChanged, m_stacked_widget, &QStackedWidget::setCurrentIndex);
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

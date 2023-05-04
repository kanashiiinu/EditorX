//self
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "MainWindow.hpp"
#include "./tools/tools.hpp"
//Qt
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QLabel>
#include <QSpacerItem>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTabBar>
#include <QTabWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QMessageBox>
#include <QStatusBar>
#include <QSettings>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QScreen>
// std
#include <vector>

MainWindow::MainWindow(QMainWindow *parent)
  : QMainWindow(parent)
{
  this->setFont(QFont("Fira Code"));
  // 菜单栏
  m_menu_bar = new MenuBar(this);
  this->setMenuBar(m_menu_bar);
  this->set_up_actions_function();
  // 工具栏
//  this->addToolBar();
  // 状态栏
  this->setStatusBar(new QStatusBar(this));

  //
  auto container = new QWidget(this);
  this->setCentralWidget(container);
  // 分成左右
  auto h_box = new QHBoxLayout(container);
  h_box->setContentsMargins(0, 0, 0, 0);
  h_box->setSpacing(0);
  // left
  m_most_left_unknown_window = new MostLeftUnknownWindow(this);
  h_box->addWidget(m_most_left_unknown_window);
  m_most_left_unknown_window->hide();
  // mid
  // tools
  m_left_tool_list_and_tool_window = new LeftToolListAndToolWindow(this);
  h_box->addWidget(m_left_tool_list_and_tool_window);
  // right
  // 编辑区
  m_tab_and_text_area_etc = new TabAndTextAreaEtc(this);
  h_box->addWidget(m_tab_and_text_area_etc);

  h_box->setStretch(0, 1); // 左侧暂时不清楚能怎么用
  h_box->setStretch(1, 1); // 各种工具
  h_box->setStretch(2, 3); // 编辑区域
  //
  this->read_settings();
  QGuiApplication::setFallbackSessionManagementEnabled(false);
  connect(qApp, &QGuiApplication::commitDataRequest,
          this, &MainWindow::commit_data);

  this->setUnifiedTitleAndToolBarOnMac(true);   // 初始化

  // qss
  this->loadStyle(":/qss/blacksoft.css");
}
void MainWindow::loadStyle(const QString &qssFile)
{

  //加载样式表
  QString qss;
  QFile file(qssFile);
  if (file.open(QFile::ReadOnly)) {
    //用QTextStream读取样式文件不用区分文件编码 带bom也行
    QStringList list;
    QTextStream in(&file);
    //in.setCodec("utf-8");
    while (!in.atEnd()) {
      QString line;
      in >> line;
      list << line;
    }

    file.close();
    qss = list.join("\n");
    QString paletteColor = qss.mid(20, 7);
    qApp->setPalette(QPalette(paletteColor));
    //用时主要在下面这句
    qApp->setStyleSheet(qss);
  }
}

MainWindow::~MainWindow()
{
}


auto MainWindow::menu_bar_action_function_new_window()->void {
  (new MainWindow())->show();
}

auto MainWindow::menu_bar_action_function_new_file()->void {
  this->m_tab_and_text_area_etc->add_new_file();
}

auto MainWindow::menu_bar_action_function_open_file()->void {
  auto filePath = QFileDialog::getOpenFileName(this, "Open Document", QDir::currentPath());
  // 路径为空或不存在
  if (filePath.isEmpty()) {
    qDebug() << "file path empty";
    return;
  }
  if (not QFileInfo(filePath).exists()) {
    qDebug() << "file is not exists";
    return;
  }
  {
    QFile file(filePath);
    if (not file.open(QFile::ReadOnly | QFile::Text)) {
      QMessageBox::critical(this, "Error", "Can't open file.");
      return;
    }
  }
  m_tab_and_text_area_etc->open_file_by_path(filePath);
}

auto MainWindow::menu_bar_action_function_open_folder()->void {
  auto dir = QFileDialog::getExistingDirectory(this,
             "Open Directory",
             QDir::currentPath(),
             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);//folderPath为指定的路径
  if (dir.isEmpty()) { return; }
  this->m_left_tool_list_and_tool_window->open_directory(dir);
}

auto MainWindow::menu_bar_action_function_save()->void {
  auto tab_widget = m_tab_and_text_area_etc->m_tab_widget;
  auto current_file_index = tab_widget->currentIndex();
  auto code_edit_area = qobject_cast<CodeEditArea *>(tab_widget->widget(current_file_index));
  auto file_path = code_edit_area->m_file_path;
  if (file_path.isEmpty()) {
    file_path = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::currentPath());
    if (file_path.isEmpty()) { return; }
    QFileInfo file_info(file_path);
    code_edit_area->m_file_path = file_info.absoluteFilePath();
    code_edit_area->m_file_name = file_info.fileName();
    tab_widget->setTabText(current_file_index, code_edit_area->m_file_name);
    QFile file(file_path);
    file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    auto code_data = this->m_tab_and_text_area_etc->get_all_code_of_current_editor();
    file.write(code_data.toUtf8());
  } else {
    QFile file(file_path);
    file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    auto code_data = this->m_tab_and_text_area_etc->get_all_code_of_current_editor();
    file.write(code_data.toUtf8());
  }
}

auto MainWindow::menu_bar_action_function_save_as()->void {
  auto file_path = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::currentPath());
  // 点取消直接退出
  if (file_path.isEmpty()) { return; }

  auto code_data = this->m_tab_and_text_area_etc->get_all_code_of_current_editor();
  QFile file(file_path);
  if (not file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) { return; }
  qDebug() << file_path;
  file.write(code_data.toUtf8());
  auto tab_widget = this->m_tab_and_text_area_etc->m_tab_widget;
  QFileInfo file_info(file_path);
  tab_widget->setTabText(tab_widget->currentIndex(), file_info.fileName());
  file.close();
}

auto MainWindow::menu_bar_action_function_save_all()->void {
  auto tab_widget = this->m_tab_and_text_area_etc->m_tab_widget;
  auto total_tab = tab_widget->count();
  for (auto current_file_index = 0; current_file_index < total_tab; ++current_file_index) {
    //save的内容
    auto code_edit_area = qobject_cast<CodeEditArea *>(tab_widget->widget(current_file_index));
    auto file_path = code_edit_area->m_file_path;
    if (file_path.isEmpty()) {
      file_path = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::currentPath());
      if (file_path.isEmpty()) { break; }
      QFileInfo file_info(file_path);
      code_edit_area->m_file_path = file_info.absoluteFilePath();
      code_edit_area->m_file_name = file_info.fileName();
      tab_widget->setTabText(current_file_index, code_edit_area->m_file_name);
      QFile file(file_path);
      file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
      auto code_data = code_edit_area->toPlainText();
      file.write(code_data.toUtf8());
    } else {
      QFile file(file_path);
      file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
      auto code_data = code_edit_area->toPlainText();
      file.write(code_data.toUtf8());
    }
  }
}

auto MainWindow::menu_bar_action_function_close_tab()->void {
  auto tab_widget = this->m_tab_and_text_area_etc->m_tab_widget;
  auto current_file_index = tab_widget->currentIndex();
  auto code_edit_area = qobject_cast<CodeEditArea *>(tab_widget->widget(current_file_index));
  if (code_edit_area->m_file_path.isEmpty()) {
    // untitled
    auto command = QMessageBox::warning(this, "xxx", "是否保存当前文件", {QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel}, QMessageBox::Cancel);
    switch (command) {
      break; case QMessageBox::Yes: {
        auto file_path = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::currentPath());
        if (file_path.isEmpty()) { return; }
        QFile file(file_path);
        file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
        auto code_data = code_edit_area->toPlainText();
        file.write(code_data.toUtf8());
        m_tab_and_text_area_etc->close_tab(current_file_index);
      }
      break; case QMessageBox::No: {
        m_tab_and_text_area_etc->close_tab(current_file_index);
      }
      break; case QMessageBox::Cancel: {
        return;
      }
      break; default: {}
    }
  } else {
    // opened
    auto command = QMessageBox::warning(this, "xxx", "是否保存当前文件", {QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel}, QMessageBox::Cancel);
    switch (command) {
      break; case QMessageBox::Yes: {
        QFile file(code_edit_area->m_file_path);
        file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
        auto code_data = code_edit_area->toPlainText();
        file.write(code_data.toUtf8());
        m_tab_and_text_area_etc->close_tab(current_file_index);
      }
      break; case QMessageBox::No: {
        m_tab_and_text_area_etc->close_tab(current_file_index);
      }
      break; case QMessageBox::Cancel: {
        return;
      }
      break; default: {}
    }
  }

}

auto MainWindow::menu_bar_action_function_close_all_tabs()->void {
  auto tab_widget = this->m_tab_and_text_area_etc->m_tab_widget;
  const auto total_tab = tab_widget->count();
  // 先一个一个处理保存
  for (auto current_file_index = 0; current_file_index < total_tab; ++current_file_index) {
    auto code_edit_area = qobject_cast<CodeEditArea *>(tab_widget->widget(current_file_index));
    if (code_edit_area->m_file_path.isEmpty()) {
      // untitled
      auto command = QMessageBox::warning(this, "xxx", "是否保存当前文件", {QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel}, QMessageBox::Cancel);
      switch (command) {
        break; case QMessageBox::Yes: {
          auto file_path = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::currentPath());
          if (file_path.isEmpty()) { return; }
          QFile file(file_path);
          file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
          auto code_data = code_edit_area->toPlainText();
          file.write(code_data.toUtf8());
        }
        break; case QMessageBox::No: {
          break;
        }
        break; case QMessageBox::Cancel: {
          return;
        }
        break; default: {}
      }
    } else {
      // opened
      auto command = QMessageBox::warning(this, "xxx", "是否保存当前文件", {QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel}, QMessageBox::Cancel);
      switch (command) {
        break; case QMessageBox::Yes: {
          QFile file(code_edit_area->m_file_path);
          file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
          auto code_data = code_edit_area->toPlainText();
          file.write(code_data.toUtf8());
        }
        break; case QMessageBox::No: {
          break;
        }
        break; case QMessageBox::Cancel: {
          return;
        }
        break; default: {}
      }
    }
  }
  // 再全布关掉
  for (auto i = 0; i < total_tab; ++i) {
    m_tab_and_text_area_etc->close_tab(0);
  }
}

auto MainWindow::menu_bar_action_function_quit()->void {
  /*关闭其他内容*/
  /* TODO */
  this->close();
}

auto MainWindow::menu_bar_action_function_undo()->void {
  m_tab_and_text_area_etc->operate_current_area("Undo");
}

auto MainWindow::menu_bar_action_function_redo()->void {
  m_tab_and_text_area_etc->operate_current_area("Redo");
}

auto MainWindow::menu_bar_action_function_cut()->void {
  m_tab_and_text_area_etc->operate_current_area("Cut");
}

auto MainWindow::menu_bar_action_function_copy()->void {
  m_tab_and_text_area_etc->operate_current_area("Copy");
}

auto MainWindow::menu_bar_action_function_paste()->void {
  m_tab_and_text_area_etc->operate_current_area("Paste");
}

auto MainWindow::menu_bar_action_function_read_only()->void {
  m_tab_and_text_area_etc->set_current_area_read_only(true);
}

auto MainWindow::menu_bar_action_function_full_screen()->void {
  if
  (this->isMaximized()) { this->showNormal(); }
  else
    /*this->isNormal*/  { this->showMaximized(); }
}

auto MainWindow::menu_bar_action_function_increase_font_size()->void {
  if (not m_tab_and_text_area_etc->has_content()) { return;};
  auto tab_widget = this->m_tab_and_text_area_etc->m_tab_widget;
  auto code_area =  qobject_cast<CodeEditArea *>(tab_widget->widget(tab_widget->currentIndex()));
  auto code_area_font_info = code_area->fontInfo();
  code_area->setFont(QFont(code_area_font_info.family(), code_area->fontInfo().pointSize() + 5));
  //文本框使用以上设定
  code_area->setPlainText(code_area->toPlainText());
}

auto MainWindow::menu_bar_action_function_decrease_font_size()->void {
  if (not m_tab_and_text_area_etc->has_content()) { return;};
  auto tab_widget = this->m_tab_and_text_area_etc->m_tab_widget;
  auto code_area =  qobject_cast<CodeEditArea *>(tab_widget->widget(tab_widget->currentIndex()));
  auto code_area_font_info = code_area->fontInfo();
  auto font_size = code_area->fontInfo().pointSize() - 5;
  font_size = font_size > 5 ? font_size : 5;
  code_area->setFont(QFont(code_area_font_info.family(), font_size));
  //文本框使用以上设定
  code_area->setPlainText(code_area->toPlainText());
}

auto MainWindow::menu_bar_action_function_find_in_file()->void {
  m_left_tool_list_and_tool_window->m_list_widget->setCurrentRow(1);
  auto tab_widget = m_tab_and_text_area_etc->m_tab_widget;
  auto code_edit_area = qobject_cast<CodeEditArea *>(tab_widget->widget(tab_widget->currentIndex()));
  m_left_tool_list_and_tool_window->m_find_and_replace_widget->set_code_edit_area(code_edit_area);
}

auto MainWindow::menu_bar_action_function_replace_in_file()->void {
  m_left_tool_list_and_tool_window->m_list_widget->setCurrentRow(1);
}

auto MainWindow::menu_bar_action_function_rename()->void {
  /*TODO*/
}

auto MainWindow::menu_bar_action_function_xxx()->void {
  qDebug() << "暂时没有内容";
}

auto MainWindow::menu_bar_action_function_about()->void {
  QMessageBox::about(this, "about", "暂时没有内容");
}

auto MainWindow::set_up_actions_function()->void {

  // 菜单栏
  // 文件菜单项
//  auto new_window = new QAction("new window");
  m_menu_bar->set_action_function("new window", [this] () {
    this->menu_bar_action_function_new_window();
  } );
//  auto new_file = new QAction("new file");
  m_menu_bar->set_action_function("new file", [this]() {
    this->menu_bar_action_function_new_file();
  });
//  auto open_file = new QAction("open file");
  m_menu_bar->set_action_function("open file", [this]() {
    this->menu_bar_action_function_open_file();
  });
//  auto open_folder = new QAction("open folder");
  m_menu_bar->set_action_function("open folder", [this]() {
    this->menu_bar_action_function_open_folder();
  });
//  auto save = new QAction("save");
  m_menu_bar->set_action_function("save", [this]() {
    this->menu_bar_action_function_save();
  });
//  auto save_as = new QAction("save as");
  m_menu_bar->set_action_function("save as", [this]() {
    this->menu_bar_action_function_save_as();
  });
//  auto save_all = new QAction("save all");
  m_menu_bar->set_action_function("save all", [this]() {
    this->menu_bar_action_function_save();
  });
//  auto close_tab = new QAction("close tab");
  m_menu_bar->set_action_function("close tab", [this]() {
    this->menu_bar_action_function_close_tab();
  });
  // 未处理所有文件的一致
//  auto close_all_tabs = new QAction("close all tabs");
  m_menu_bar->set_action_function("close all tabs", [this]() {
    this->menu_bar_action_function_close_all_tabs();
  });
//  auto quit = new QAction("quit");
  // 未处理文件的保存
  m_menu_bar->set_action_function("quit", [this]() {
    this->menu_bar_action_function_quit();
  });
  // 编辑菜单项
//  auto undo = new QAction("undo");
  m_menu_bar->set_action_function("undo", [this]() {
    this->menu_bar_action_function_undo();
  });
//  auto redo = new QAction("redo");
  m_menu_bar->set_action_function("redo", [this]() {
    this->menu_bar_action_function_redo();
  });
//  auto cut = new QAction("cut");
  m_menu_bar->set_action_function("cut", [this]() {
    this->menu_bar_action_function_cut();
  });
//  auto copy = new QAction("copy");
  m_menu_bar->set_action_function("copy", [this]() {
    this->menu_bar_action_function_copy();
  });
//  auto paste = new QAction("paste");
  m_menu_bar->set_action_function("paste", [this]() {
    this->menu_bar_action_function_paste();
  });
//  auto read_only = new QAction("read only");
  m_menu_bar->set_action_function("read only", [this]() {
    this->menu_bar_action_function_read_only();
  });
  // 视图菜单项
//  auto full_screen = new QAction("full screen");
  m_menu_bar->set_action_function("full screen", [this]() {
    this->menu_bar_action_function_full_screen();
  });
//  auto increase_font_size = new QAction("increase font size");
  m_menu_bar->set_action_function("increase font size", [this]() {
    this->menu_bar_action_function_increase_font_size();
  });
//  auto decrease_font_size = new QAction("decrease font size");
  m_menu_bar->set_action_function("decrease font size", [this]() {
    this->menu_bar_action_function_decrease_font_size();
  });

  // 查找菜单项
  // 1.区分大小写;2.全字匹配;3.正则匹配
//  auto find_in_file = new QAction("find in file");
  m_menu_bar->set_action_function("find in file", [this]() {
    this->menu_bar_action_function_find_in_file();
  });
  // 1.替换;2.全部替换;3.保留大小写
//  auto replace_in_file = new QAction("replace in file");
  m_menu_bar->set_action_function("replace in file", [this]() {
    this->menu_bar_action_function_replace_in_file();
  });

  // 重构菜单项
//  auto rename = new QAction("rename");
  m_menu_bar->set_action_function("rename", [this]() {
    this->menu_bar_action_function_rename();
  });

  // 常规菜单项
//  auto preferences = new QAction("xxx");
  m_menu_bar->set_action_function("xxx", [this]() {
    this->menu_bar_action_function_xxx();
  });

  // 帮助菜单项
//  auto about = new QAction("about");
  m_menu_bar->set_action_function("about", [this]() {
    this->menu_bar_action_function_about();
  });
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (not m_tab_and_text_area_etc->is_any_file_modified())
  {
    this->write_settings();
    event->accept();
    return;
  }
  const QMessageBox::StandardButton ret = QMessageBox::warning(this
                                          , tr("xxx")
                                          , tr("是否要保存对当前文件的更改?")
                                          , QMessageBox::Save
                                          | QMessageBox::Discard
                                          | QMessageBox::Cancel
                                                              );
  switch (ret) {
    case QMessageBox::Save:
    {
      /*TODO*/
      return ;
    }
    case QMessageBox::Discard:
    {
      /*TODO*/
      this->write_settings();
      event->accept();
      return;
    }
    case QMessageBox::Cancel:
    {
      event->ignore();
      return;
    }
    default:
    {
      return;
    }
  }
  return;
}


//  记住退出前的设置 >>
void MainWindow::read_settings()
{
  QSettings settings(QDir::currentPath().append("/init.txt"), QCoreApplication::applicationName());
  const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
  if (geometry.isEmpty())
  {
//    const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
    const QRect availableGeometry = QGuiApplication::primaryScreen()->geometry();
    resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
    move((availableGeometry.width() - width()) / 2,
         (availableGeometry.height() - height()) / 2);
  }
  else
  {
    this->restoreGeometry(geometry);
  }
}

void MainWindow::write_settings()
{ // // 写入配置
  qDebug() << QDir::currentPath();
  QSettings settings(QDir::currentPath().append("/init.txt"), QCoreApplication::applicationName());
  settings.setValue("geometry", this->saveGeometry());
}

void MainWindow::commit_data(QSessionManager &manager)
{ // 会话管理
  if (manager.allowsInteraction())
  {
    if (m_tab_and_text_area_etc->is_any_file_modified())
    {
      manager.cancel();
    }
  }
  else /*Non-interactive: save without asking*/
  {
    if (m_tab_and_text_area_etc->is_any_file_modified())
    {
      this->menu_bar_action_function_save_all();
    }
  }
}

//  记住退出前的设置 <<

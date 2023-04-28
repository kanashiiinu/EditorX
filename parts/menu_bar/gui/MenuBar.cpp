#include "MenuBar.hpp"

#include <QMenu>
#include <QAction>
#include <QDebug>
#include <functional>

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{

  // 菜单栏
  // 文件菜单项
  auto file_menu = new QMenu("file", this);
  this->addMenu(file_menu);
  auto new_window = new QAction("new window", file_menu);
  auto new_file = new QAction("new file", file_menu);
  auto open_file = new QAction("open file", file_menu);
  auto open_folder = new QAction("open folder", file_menu);
  auto save = new QAction("save", file_menu);
  auto save_as = new QAction("save as", file_menu);
  auto save_all = new QAction("save all", file_menu);
  auto close_tab = new QAction("close tab", file_menu);
  auto close_all_tabs = new QAction("close all tabs", file_menu);
  auto quit = new QAction("quit", file_menu);
  m_names_to_actions["new window"] = new_window;
  m_names_to_actions["new file"] = new_file;
  m_names_to_actions["open file"] = open_file;
  m_names_to_actions["open folder"] = open_folder;
  m_names_to_actions["save"] = save;
  m_names_to_actions["save as"] = save_as;
  m_names_to_actions["save all"] = save_all;
  m_names_to_actions["close tab"] = close_tab;
  m_names_to_actions["close all tabs"] = close_all_tabs;
  m_names_to_actions["quit"] = quit;
  file_menu->addActions({new_window, new_file});
  file_menu->addSeparator();
  file_menu->addActions({open_file, open_folder});
  file_menu->addSeparator();
  file_menu->addActions({ save, save_as, save_all});
  file_menu->addSeparator();
  file_menu->addActions({close_tab, close_all_tabs});
  file_menu->addSeparator();
  file_menu->addActions({quit});


  // 编辑菜单项
  auto edit_menu = new QMenu("edit", this);
  this->addMenu(edit_menu);
  auto undo = new QAction("undo", edit_menu);
  auto redo = new QAction("redo", edit_menu);
  auto cut = new QAction("cut", edit_menu);
  auto copy = new QAction("copy", edit_menu);
  auto paste = new QAction("paste", edit_menu);
  auto read_only = new QAction("read only", edit_menu);
  m_names_to_actions["undo"] = undo;
  m_names_to_actions["redo"] = redo;
  m_names_to_actions["cut"] = cut;
  m_names_to_actions["copy"] = copy;
  m_names_to_actions["paste"] = paste;
  m_names_to_actions["read only"] = read_only;
  edit_menu->addActions({undo, redo});
  edit_menu->addSeparator();
  edit_menu->addActions({cut, copy, paste, read_only});
  edit_menu->addSeparator();
  edit_menu->addActions({read_only});

  // 视图菜单项
  auto veiw_menu = new QMenu("veiw", this);
  this->addMenu(veiw_menu);
  auto full_screen = new QAction("full screen", veiw_menu);
  auto increase_font_size = new QAction("increase font size", veiw_menu);
  auto decrease_font_size = new QAction("decrease font size", veiw_menu);
  m_names_to_actions["full screen"] = full_screen;
  m_names_to_actions["increase font size"] = increase_font_size;
  m_names_to_actions["decrease font size"] = decrease_font_size;
  veiw_menu->addActions({full_screen});
  veiw_menu->addSeparator();
  veiw_menu->addActions({increase_font_size, decrease_font_size});

  // 查找菜单项
  auto search_menu = new QMenu("search", this);
  auto find_in_file = new QAction("find in file", search_menu);
  auto replace_in_file = new QAction("replace in file", search_menu);
  m_names_to_actions["find in file"] = find_in_file;
  m_names_to_actions["replace in file"] = replace_in_file;
  this->addMenu(search_menu);
  search_menu->addActions({find_in_file});
  search_menu->addSeparator();
  search_menu->addActions({replace_in_file});

  // 重构菜单项
  auto refactor_menu = new QMenu("refactor", this);
  this->addMenu(refactor_menu);
  auto rename = new QAction("rename", refactor_menu);
  m_names_to_actions["rename"] = rename;
  refactor_menu->addActions({rename});

  // 常规菜单项
  auto preferences_menu = new QMenu("preferences", this);
  this->addMenu(preferences_menu);
  auto xxx = new QAction("xxx", preferences_menu);
  m_names_to_actions["xxx"] = xxx;
  preferences_menu->addActions({xxx});

  // 帮助菜单项
  auto help_menu = new QMenu("help", this);
  this->addMenu(help_menu);
  auto about = new QAction("about", help_menu);
  m_names_to_actions["about"] = about;
  auto shortcuts = new QAction("shortcuts", help_menu);
  m_names_to_actions["shortcuts"] = shortcuts;
  help_menu->addActions({about, shortcuts});

  // 将名称与菜单项绑定，connect菜单项与将要实现的功能
  for (auto x : m_names_to_actions.values()) {
    m_actions_to_funcions[x] = []() {qDebug() << "empty";};
    connect(x, &QAction::triggered, this, &MenuBar::run_action_function);
  }

}

auto MenuBar::run_action_function()->void {
  m_actions_to_funcions[qobject_cast<QAction *>(sender())]();
}

auto MenuBar::set_action_function(const QString &name, std::function<void ()> fun)-> void {
  qDebug() << name;
  m_actions_to_funcions[m_names_to_actions.value(name)] = fun;
}

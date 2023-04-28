#ifndef MENUBAR_HPP
#define MENUBAR_HPP

#include <QMenuBar>
#include <QHash>

#include <functional>
#include <unordered_map>

class MenuBar : public QMenuBar
{
  Q_OBJECT
 public:
  explicit MenuBar(QWidget *parent = nullptr);
 private slots:
  void run_action_function();

 signals:

 public:

  /**
  * @brief 为menu_bar中的action设置函数
  * @param name action的名字
  * @param fun 点击action希望触发的函数
  */
  auto set_action_function(const QString &name, std::function<void()> fun)-> void;

 private:

  QHash<QString, QAction *> m_names_to_actions;

  std::unordered_map<QAction *, std::function<void()>> m_actions_to_funcions;
};

#endif // MENUBAR_HPP

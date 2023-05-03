#ifndef SHELLDEMO_HPP
#define SHELLDEMO_HPP


#include <QPlainTextEdit>

/*
只能用一次=>未上锁，乱码(O), 与“\n”自动换行了(X)
*/
class QLineEdit;
class CommandRunner;
class ShellDemo: public QPlainTextEdit
{
  Q_OBJECT
 public:
  explicit ShellDemo(QWidget *parent = 0);
  virtual ~ShellDemo() = default;
  virtual QString run_command(const QString &cmd);
 protected:
  void resizeEvent(QResizeEvent *e) override ;
 private slots:
  void on_scroll_bar_value_changed();
  void on_edit_finished();
 private:
  void update_edit_position();

  QLineEdit *edit;
 private:
  auto _compute_edit_position()->QPoint;
  //
 private:
  CommandRunner *m_command_runner = nullptr;
};

#endif // SHELLDEMO_HPP

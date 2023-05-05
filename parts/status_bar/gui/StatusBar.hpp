#pragma once

#include <QStatusBar>
#include <QLabel>

class StatusBar : public QStatusBar
{
  Q_OBJECT
 public:
  StatusBar(QWidget *parent = nullptr);
  ~StatusBar() = default;

 public:

  void set_cursor_pos(int line, int column);

 private:

  QLabel *cursor_pos_label;

 public:
 signals:

  void signal_tab_width_change(const QString &);

  void signal_lang_change(const QString &);

 private slots:

  void slot_tab_width_change(const QString &);

  void slot_lang_change(const QString &);
};

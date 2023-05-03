#ifndef FRMMASKWIDGET_H
#define FRMMASKWIDGET_H

#include <QWidget>

#include <QLineEdit>
#include <QPushButton>
#include <QTimer>

class TimeReminderWidget : public QWidget
{
  Q_OBJECT

  QMetaObject::Connection m_connection;
 public:
  explicit TimeReminderWidget(QWidget *parent = 0);
  ~TimeReminderWidget();
 private:
  QTimer *m_timer;
  QLineEdit *m_time_interval_input;
  QPushButton *m_btn;
 private:
  auto init()-> void;
 private slots:
  void initForm();
  void slot_mask_widget_show();
  void slot_update_remind_time_interval();
};

#endif // FRMMASKWIDGET_H

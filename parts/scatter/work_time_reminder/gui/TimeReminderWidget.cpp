#include "TimeReminderWidget.hpp"
#include "./parts/scatter/work_time_reminder/fun/MaskWidget.hpp"
#include <QDialog>
#include <QTimer>
#include <QWindow>
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <thread>
#include <chrono>

TimeReminderWidget::TimeReminderWidget(QWidget *parent) : QWidget(parent)
{
  this->init();

  QTimer::singleShot(1000, this, SLOT(initForm()));

  m_timer = new QTimer(this);
  m_connection = connect(m_timer, &QTimer::timeout,
                         this, &TimeReminderWidget::slot_mask_widget_show);

//  timer->start(1 * 60 * 60 * 1000);
  m_timer->start(3000);

  connect(m_btn, &QPushButton::clicked,
          this, &TimeReminderWidget::slot_update_remind_time_interval);
}

TimeReminderWidget::~TimeReminderWidget()
{
}

auto TimeReminderWidget::init()->void
{
  auto layout = new QHBoxLayout();
  this->setLayout(layout);
  m_time_interval_input = new QLineEdit("1", this);
  layout->addWidget(m_time_interval_input);
  m_btn = new QPushButton("ok", this);
  layout->addWidget(m_btn);
}


void TimeReminderWidget::initForm()
{
  QWidget *pWidget = this->topLevelWidget();
  QWindow *pWin = pWidget->windowHandle();
//  pWin->setFlags(pWidget->windowFlags() | Qt::WindowStaysOnTopHint);
  qDebug() << this->topLevelWidget();
  MaskWidget::Instance()->set_main_widget(this->topLevelWidget());
  MaskWidget::Instance()->set_dialog_names(QStringList() << "TimerReminder");
}

void TimeReminderWidget::slot_mask_widget_show()
{
  QDialog d;
  d.setObjectName("TimerReminder");
  d.setWindowTitle("TimerReminder");
  d.resize(400, 300);
//  d.exec();
  d.show();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  d.close();
}


void TimeReminderWidget::slot_update_remind_time_interval()
{
  auto text = this->m_time_interval_input->text();
  auto num_is_int = false;
  if (not num_is_int)
  {
    return;
  }
  auto num = text.toInt(&num_is_int);
  if (num > 8)
  {
    return;
  }
  auto second_num = num * 60 * 60 * 1000;
  m_timer->setInterval(second_num);
  disconnect(this->m_connection);
  this->m_connection = connect(m_timer, &QTimer::timeout,
                               this, &TimeReminderWidget::slot_mask_widget_show);
}

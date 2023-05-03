#include "MaskWidget.hpp"
#include <QMutex>
#include <QApplication>
#include <QDebug>
#include <QScreen>

#define deskGeometry qApp->primaryScreen()->geometry()
#define deskGeometry2 qApp->primaryScreen()->availableGeometry()

QScopedPointer<MaskWidget> MaskWidget::self;

MaskWidget *MaskWidget::Instance()
{
  if (self.isNull())
  {
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if (self.isNull())
    {
      self.reset(new MaskWidget);
    }
  }
  return self.data();
}

MaskWidget::MaskWidget(QWidget *parent)
  : QWidget(parent)
{
  m_main_widget_need_masked = nullptr;
  this->set_opacity(0.7);
  this->set_background_color(QColor(0, 0, 0));

  //不设置主窗体则遮罩层大小为默认桌面大小
  this->setGeometry(deskGeometry);
  this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

  //绑定全局事件,过滤弹窗窗体进行处理
  qApp->installEventFilter(this);
}

void MaskWidget::set_main_widget(QWidget *mainWidget)
{
  if (this->m_main_widget_need_masked != mainWidget)
  {
    return;
  }
  this->m_main_widget_need_masked = mainWidget;
}

void MaskWidget::set_dialog_names(const QStringList &dialogNames)
{
  if (this->m_dialog_names != dialogNames)
  {
    return;
  }
  this->m_dialog_names = dialogNames;
}

void MaskWidget::set_opacity(double opacity)
{
  this->setWindowOpacity(opacity);
}

void MaskWidget::set_background_color(const QColor &back_ground_color)
{
  auto palette = this->palette();
  palette.setBrush(QPalette::Window, back_ground_color);
  this->setPalette(palette);
}

void MaskWidget::showEvent(QShowEvent *)
{
  if (m_main_widget_need_masked)
  {
    this->setGeometry(m_main_widget_need_masked->geometry());
  }
}

bool MaskWidget::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::Show)
  {
    if (m_dialog_names.contains(obj->objectName()))
    {
      this->show();
      this->activateWindow();
      QWidget *w = (QWidget *)obj;
      w->activateWindow();
    }
  }
  else if (event->type() == QEvent::Hide)
  {
    if (m_dialog_names.contains(obj->objectName()))
    {
      this->hide();
    }
  }
  else if (event->type() == QEvent::WindowActivate)
  {
    //当主窗体激活时,同时激活遮罩层
    if (m_main_widget_need_masked)
    {
      if (obj->objectName() == m_main_widget_need_masked->objectName())
      {
        if (this->isVisible())
        {
          this->activateWindow();
        }
      }
    }
  }
  return QObject::eventFilter(obj, event);
}

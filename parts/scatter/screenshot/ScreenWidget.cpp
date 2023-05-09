#include "ScreenWidget.hpp"
#include "qmutex.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qfiledialog.h"
#include "qevent.h"
#include "qdatetime.h"
#include "qstringlist.h"
#include "qdebug.h"
#include "qscreen.h"

#define deskGeometry qApp->primaryScreen()->geometry()
#define deskGeometry2 qApp->primaryScreen()->availableGeometry()

#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))

Screen::Screen(QSize size)
{
  m_max_width = size.width();
  m_max_height = size.height();

  m_start_pos = QPoint(-1, -1);
  m_end_pos = m_start_pos;
  m_left_up_pos = m_start_pos;
  m_right_down_pos = m_start_pos;
  m_status = SELECT;
}

int Screen::width()
{
  return m_max_width;
}

int Screen::height()
{
  return m_max_height;
}

Screen::MouseStatus Screen::get_status()
{
  return m_status;
}

void Screen::set_status(MouseStatus status)
{
  this->m_status = status;
}

void Screen::set_end(QPoint pos)
{
  m_end_pos = pos;
  m_left_up_pos = m_start_pos;
  m_right_down_pos = m_end_pos;
  compare_point(m_left_up_pos, m_right_down_pos);
}

void Screen::set_start(QPoint pos)
{
  m_start_pos = pos;
}

QPoint Screen::get_end()
{
  return m_end_pos;
}

QPoint Screen::get_start()
{
  return m_start_pos;
}

QPoint Screen::get_left_up()
{
  return m_left_up_pos;
}

QPoint Screen::get_right_down()
{
  return m_right_down_pos;
}

bool Screen::is_in_area(QPoint pos)
{
  if (pos.x() > m_left_up_pos.x() && pos.x() < m_right_down_pos.x() && pos.y() > m_left_up_pos.y() && pos.y() < m_right_down_pos.y()) {
    return true;
  }

  return false;
}

void Screen::move(QPoint p)
{
  int lx = m_left_up_pos.x() + p.x();
  int ly = m_left_up_pos.y() + p.y();
  int rx = m_right_down_pos.x() + p.x();
  int ry = m_right_down_pos.y() + p.y();

  if (lx < 0) {
    lx = 0;
    rx -= p.x();
  }

  if (ly < 0) {
    ly = 0;
    ry -= p.y();
  }

  if (rx > m_max_width)  {
    rx = m_max_width;
    lx -= p.x();
  }

  if (ry > m_max_height) {
    ry = m_max_height;
    ly -= p.y();
  }

  m_left_up_pos = QPoint(lx, ly);
  m_right_down_pos = QPoint(rx, ry);
  m_start_pos = m_left_up_pos;
  m_end_pos = m_right_down_pos;
}

void Screen::compare_point(QPoint &leftTop, QPoint &rightDown)
{
  QPoint l = leftTop;
  QPoint r = rightDown;

  if (l.x() <= r.x()) {
    if (l.y() <= r.y()) {
    } else {
      leftTop.setY(r.y());
      rightDown.setY(l.y());
    }
  } else {
    if (l.y() < r.y()) {
      leftTop.setX(r.x());
      rightDown.setX(l.x());
    } else {
      QPoint tmp;
      tmp = leftTop;
      leftTop = rightDown;
      rightDown = tmp;
    }
  }
}

QScopedPointer<ScreenWidget> ScreenWidget::self;
ScreenWidget *ScreenWidget::_()
{
  if (self.isNull()) {
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if (self.isNull()) {
      self.reset(new ScreenWidget);
    }
  }

  return self.data();
}

ScreenWidget::ScreenWidget(QWidget *parent) : QWidget(parent)
{
  //this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);

  m_menu = new QMenu(this);
  m_menu->addAction("保存当前截图", this, SLOT(save_screen()));
  m_menu->addAction("保存全屏截图", this, SLOT(save_full_screen()));
  m_menu->addAction("截图另存为", this, SLOT(save_screen_other()));
  m_menu->addAction("全屏另存为", this, SLOT(save_full_other()));
  m_menu->addAction("退出截图", this, SLOT(hide()));

  //取得屏幕大小
  m_screen = new Screen(deskGeometry.size());
  //保存全屏图像
  m_full_screen = new QPixmap();
}

void ScreenWidget::paintEvent(QPaintEvent *)
{
  int x = m_screen->get_left_up().x();
  int y = m_screen->get_left_up().y();
  int w = m_screen->get_right_down().x() - x;
  int h = m_screen->get_right_down().y() - y;

  QPainter painter(this);

  QPen pen;
  pen.setColor(Qt::blue);
  pen.setWidth(2);
//  pen.setStyle(Qt::DotLine);
  painter.setPen(pen);
  painter.drawPixmap(0, 0, *m_background_screen);

  if (w != 0 && h != 0) {
    painter.drawPixmap(x, y, m_full_screen->copy(x, y, w, h));
  }

  painter.drawRect(x, y, w, h);

  pen.setColor(Qt::yellow);
  painter.setPen(pen);
//  painter.drawText(x + 2, y - 8, tr("截图范围：( %1 x %2 ) - ( %3 x %4 )  图片大小：( %5 x %6 )")
//                   .arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}

void ScreenWidget::showEvent(QShowEvent *)
{
  QPoint point(-1, -1);
  m_screen->set_start(point);
  m_screen->set_end(point);

  QScreen *pscreen = QApplication::primaryScreen();
  *m_full_screen = pscreen->grabWindow(0, 0, 0, m_screen->width(), m_screen->height());

  //设置透明度实现模糊背景
  QPixmap pix(m_screen->width(), m_screen->height());
  pix.fill((QColor(160, 160, 160, 200)));
  m_background_screen = new QPixmap(*m_full_screen);
  QPainter p(m_background_screen);
  p.drawPixmap(0, 0, pix);
}

void ScreenWidget::save_screen()
{
  int x = m_screen->get_left_up().x();
  int y = m_screen->get_left_up().y();
  int w = m_screen->get_right_down().x() - x;
  int h = m_screen->get_right_down().y() - y;

  QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
  m_full_screen->copy(x, y, w, h).save(fileName, "png");
  close();
}

void ScreenWidget::save_full_screen()
{
  QString fileName = QString("%1/full_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
  m_full_screen->save(fileName, "png");
  close();
}

void ScreenWidget::save_screen_other()
{
  QString name = QString("%1.png").arg(STRDATETIME);
  QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
  if (!fileName.endsWith(".png")) {
    fileName += ".png";
  }

  if (fileName.length() > 0) {
    int x = m_screen->get_left_up().x();
    int y = m_screen->get_left_up().y();
    int w = m_screen->get_right_down().x() - x;
    int h = m_screen->get_right_down().y() - y;
    m_full_screen->copy(x, y, w, h).save(fileName, "png");
    close();
  }
}

void ScreenWidget::save_full_other()
{
  QString name = QString("%1.png").arg(STRDATETIME);
  QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
  if (!fileName.endsWith(".png")) {
    fileName += ".png";
  }

  if (fileName.length() > 0) {
    m_full_screen->save(fileName, "png");
    close();
  }
}

void ScreenWidget::mouseMoveEvent(QMouseEvent *e)
{
  if (m_screen->get_status() == Screen::SELECT) {
    m_screen->set_end(e->pos());
  } else if (m_screen->get_status() == Screen::MOV) {
    QPoint p(e->x() - m_mov_pos.x(), e->y() - m_mov_pos.y());
    m_screen->move(p);
    m_mov_pos = e->pos();
  }

  this->update();
}

void ScreenWidget::mousePressEvent(QMouseEvent *e)
{
  int status = m_screen->get_status();

  if (status == Screen::SELECT) {
    m_screen->set_start(e->pos());
  } else if (status == Screen::MOV) {
    if (m_screen->is_in_area(e->pos()) == false) {
      m_screen->set_start(e->pos());
      m_screen->set_status(Screen::SELECT);
    } else {
      m_mov_pos = e->pos();
      this->setCursor(Qt::SizeAllCursor);
    }
  }

  this->update();
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *)
{
  if (m_screen->get_status() == Screen::SELECT) {
    m_screen->set_status(Screen::MOV);
  } else if (m_screen->get_status() == Screen::MOV) {
    this->setCursor(Qt::ArrowCursor);
  }
}

void ScreenWidget::contextMenuEvent(QContextMenuEvent *)
{
  this->setCursor(Qt::ArrowCursor);
  m_menu->exec(cursor().pos());
}

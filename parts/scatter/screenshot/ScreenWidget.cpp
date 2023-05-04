#pragma execution_character_set("utf-8")

#include "ScreenWidget.hpp"
#include "qmutex.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qfiledialog.h"
#include "qevent.h"
#include "qdatetime.h"
#include "qstringlist.h"
#include "qdebug.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include "qscreen.h"
#define deskGeometry qApp->primaryScreen()->geometry()
#define deskGeometry2 qApp->primaryScreen()->availableGeometry()
#else
#include "qdesktopwidget.h"
#define deskGeometry qApp->desktop()->geometry()
#define deskGeometry2 qApp->desktop()->availableGeometry()
#endif

#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))

Screen::Screen(QSize size)
{
  maxWidth = size.width();
  maxHeight = size.height();

  startPos = QPoint(-1, -1);
  endPos = startPos;
  leftUpPos = startPos;
  rightDownPos = startPos;
  status = SELECT;
}

int Screen::width()
{
  return maxWidth;
}

int Screen::height()
{
  return maxHeight;
}

Screen::STATUS Screen::get_status()
{
  return status;
}

void Screen::set_status(STATUS status)
{
  this->status = status;
}

void Screen::set_end(QPoint pos)
{
  endPos = pos;
  leftUpPos = startPos;
  rightDownPos = endPos;
  compare_point(leftUpPos, rightDownPos);
}

void Screen::set_start(QPoint pos)
{
  startPos = pos;
}

QPoint Screen::get_end()
{
  return endPos;
}

QPoint Screen::get_start()
{
  return startPos;
}

QPoint Screen::get_left_up()
{
  return leftUpPos;
}

QPoint Screen::get_right_down()
{
  return rightDownPos;
}

bool Screen::is_in_area(QPoint pos)
{
  if (pos.x() > leftUpPos.x() && pos.x() < rightDownPos.x() && pos.y() > leftUpPos.y() && pos.y() < rightDownPos.y()) {
    return true;
  }

  return false;
}

void Screen::move(QPoint p)
{
  int lx = leftUpPos.x() + p.x();
  int ly = leftUpPos.y() + p.y();
  int rx = rightDownPos.x() + p.x();
  int ry = rightDownPos.y() + p.y();

  if (lx < 0) {
    lx = 0;
    rx -= p.x();
  }

  if (ly < 0) {
    ly = 0;
    ry -= p.y();
  }

  if (rx > maxWidth)  {
    rx = maxWidth;
    lx -= p.x();
  }

  if (ry > maxHeight) {
    ry = maxHeight;
    ly -= p.y();
  }

  leftUpPos = QPoint(lx, ly);
  rightDownPos = QPoint(rx, ry);
  startPos = leftUpPos;
  endPos = rightDownPos;
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
ScreenWidget *ScreenWidget::Instance()
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

  menu = new QMenu(this);
  menu->addAction("保存当前截图", this, SLOT(save_screen()));
  menu->addAction("保存全屏截图", this, SLOT(save_full_screen()));
  menu->addAction("截图另存为", this, SLOT(save_screen_other()));
  menu->addAction("全屏另存为", this, SLOT(save_full_other()));
  menu->addAction("退出截图", this, SLOT(hide()));

  //取得屏幕大小
  screen = new Screen(deskGeometry.size());
  //保存全屏图像
  fullScreen = new QPixmap();
}

void ScreenWidget::paintEvent(QPaintEvent *)
{
  int x = screen->get_left_up().x();
  int y = screen->get_left_up().y();
  int w = screen->get_right_down().x() - x;
  int h = screen->get_right_down().y() - y;

  QPainter painter(this);

  QPen pen;
  pen.setColor(Qt::green);
  pen.setWidth(2);
  pen.setStyle(Qt::DotLine);
  painter.setPen(pen);
  painter.drawPixmap(0, 0, *bgScreen);

  if (w != 0 && h != 0) {
    painter.drawPixmap(x, y, fullScreen->copy(x, y, w, h));
  }

  painter.drawRect(x, y, w, h);

  pen.setColor(Qt::yellow);
  painter.setPen(pen);
  painter.drawText(x + 2, y - 8, tr("截图范围：( %1 x %2 ) - ( %3 x %4 )  图片大小：( %5 x %6 )")
                   .arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}

void ScreenWidget::showEvent(QShowEvent *)
{
  QPoint point(-1, -1);
  screen->set_start(point);
  screen->set_end(point);

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  QScreen *pscreen = QApplication::primaryScreen();
  *fullScreen = pscreen->grabWindow(0, 0, 0, screen->width(), screen->height());
#else
  *fullScreen = fullScreen->grabWindow(0, 0, 0, screen->width(), screen->height());
#endif

  //设置透明度实现模糊背景
  QPixmap pix(screen->width(), screen->height());
  pix.fill((QColor(160, 160, 160, 200)));
  bgScreen = new QPixmap(*fullScreen);
  QPainter p(bgScreen);
  p.drawPixmap(0, 0, pix);
}

void ScreenWidget::save_screen()
{
  int x = screen->get_left_up().x();
  int y = screen->get_left_up().y();
  int w = screen->get_right_down().x() - x;
  int h = screen->get_right_down().y() - y;

  QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
  fullScreen->copy(x, y, w, h).save(fileName, "png");
  close();
}

void ScreenWidget::save_full_screen()
{
  QString fileName = QString("%1/full_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
  fullScreen->save(fileName, "png");
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
    int x = screen->get_left_up().x();
    int y = screen->get_left_up().y();
    int w = screen->get_right_down().x() - x;
    int h = screen->get_right_down().y() - y;
    fullScreen->copy(x, y, w, h).save(fileName, "png");
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
    fullScreen->save(fileName, "png");
    close();
  }
}

void ScreenWidget::mouseMoveEvent(QMouseEvent *e)
{
  if (screen->get_status() == Screen::SELECT) {
    screen->set_end(e->pos());
  } else if (screen->get_status() == Screen::MOV) {
    QPoint p(e->x() - movPos.x(), e->y() - movPos.y());
    screen->move(p);
    movPos = e->pos();
  }

  this->update();
}

void ScreenWidget::mousePressEvent(QMouseEvent *e)
{
  int status = screen->get_status();

  if (status == Screen::SELECT) {
    screen->set_start(e->pos());
  } else if (status == Screen::MOV) {
    if (screen->is_in_area(e->pos()) == false) {
      screen->set_start(e->pos());
      screen->set_status(Screen::SELECT);
    } else {
      movPos = e->pos();
      this->setCursor(Qt::SizeAllCursor);
    }
  }

  this->update();
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *)
{
  if (screen->get_status() == Screen::SELECT) {
    screen->set_status(Screen::MOV);
  } else if (screen->get_status() == Screen::MOV) {
    this->setCursor(Qt::ArrowCursor);
  }
}

void ScreenWidget::contextMenuEvent(QContextMenuEvent *)
{
  this->setCursor(Qt::ArrowCursor);
  menu->exec(cursor().pos());
}

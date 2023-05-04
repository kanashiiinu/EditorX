#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

/**
 * 全局截屏控件 作者:feiyangqingyun(QQ:517216493) 2016-11-11
 * 1. 鼠标右键弹出菜单。
 * 2. 支持全局截屏。
 * 3. 支持局部截屏。
 * 4. 支持截图区域拖动。
 * 5. 支持图片另存为。
 */

#include <QWidget>
#include <QMenu>
#include <QPoint>
#include <QSize>

class Screen
{
 public:
  enum STATUS
  {
    SELECT,
    MOV,
    SET_W_H
  };
  Screen() {}
  Screen(QSize size);

  void set_start(QPoint pos);
  void set_end(QPoint pos);
  QPoint get_start();
  QPoint get_end();

  QPoint get_left_up();
  QPoint get_right_down();

  STATUS get_status();
  void set_status(STATUS status);

  int width();
  int height();

  //检测坐标点是否在截图区域内
  bool is_in_area(QPoint pos);
  //按坐标移动截图区域
  void move(QPoint p);

 private:
  //记录 截图区域 左上角、右下角
  QPoint leftUpPos, rightDownPos;
  //记录 鼠标开始位置、结束位置
  QPoint startPos, endPos;
  //记录屏幕大小
  int maxWidth, maxHeight;
  //三个状态: 选择区域、移动区域、设置width height
  STATUS status;

  //比较两位置，判断左上角、右下角
  void compare_point(QPoint &s, QPoint &e);
};

#ifdef quc
class Q_DECL_EXPORT ScreenWidget : public QWidget
#else
class ScreenWidget : public QWidget
#endif

{
  Q_OBJECT
 public:
  static ScreenWidget *Instance();
  explicit ScreenWidget(QWidget *parent = 0);

 private:
  static QScopedPointer<ScreenWidget> self;
  QMenu *menu;            //右键菜单对象
  Screen *screen;         //截屏对象
  QPixmap *fullScreen;    //保存全屏图像
  QPixmap *bgScreen;      //模糊背景图
  QPoint movPos;          //坐标

 protected:
  void contextMenuEvent(QContextMenuEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void paintEvent(QPaintEvent *);
  void showEvent(QShowEvent *);

 private slots:
  void save_screen();
  void save_full_screen();
  void save_screen_other();
  void save_full_other();
};

#endif // SCREENWIDGET_H


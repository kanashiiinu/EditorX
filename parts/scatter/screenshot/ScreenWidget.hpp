#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QPoint>
#include <QSize>

class Screen
{
 public:
  enum MouseStatus
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

  MouseStatus get_status();
  void set_status(MouseStatus m_status);

  int width();
  int height();

  //检测坐标点是否在截图区域内
  bool is_in_area(QPoint pos);
  //按坐标移动截图区域
  void move(QPoint p);

 private:
  //记录 截图区域 左上角、右下角
  QPoint m_left_up_pos;
  QPoint m_right_down_pos;
  //记录 鼠标开始位置、结束位置
  QPoint m_start_pos;
  QPoint m_end_pos;
  //记录屏幕大小
  int m_max_width;
  int m_max_height;
  //三个状态: 选择区域、移动区域、设置width height
  MouseStatus m_status;

  //比较两位置，判断左上角、右下角
  void compare_point(QPoint &s, QPoint &e);
};

class ScreenWidget : public QWidget
{
  Q_OBJECT
 public:
  static ScreenWidget *_();
  explicit ScreenWidget(QWidget *parent = 0);

 private:
  static QScopedPointer<ScreenWidget> self;
  QMenu *m_menu;            //右键菜单对象
  Screen *m_screen;         //截屏对象
  QPixmap *m_full_screen;    //保存全屏图像
  QPixmap *m_background_screen;      //模糊背景图
  QPoint m_mov_pos;          //坐标

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


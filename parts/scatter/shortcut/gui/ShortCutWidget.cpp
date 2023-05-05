#include "ShortCutWidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QTableView>
#include <QTableWidget>
#include <QDebug>
#include <QResizeEvent>
#include <QMap>
#include <QHeaderView>

static QMap<QString, QString> g_data =
{
  { "left", "h"},
  { "down", "k"},
  { "up", "j"},
  { "right", "l"},
  { "Close Tab", "Ctrl + W" },
  { "Save", "Ctrl + S" },
  { "SaveAs", "Ctrl + Shift + S" },
  { "Delete", "Del" },
  { "Cut", "Ctrl + X" },
  { "Copy", "Ctrl + C" },
  { "Paste", "Ctrl + V" },
  { "Undo", "Ctrl + Z" },
  { "Redo", "Ctrl + Shift + Z" },
  { "Increase Font Size", "Ctrl + Plus" },
  { "Decrease Font Size", "Ctrl + Minus" },
  { "AddTab", "Ctrl + T" },
  { "Find", "Ctrl + F" },
  { "Find Previous", "Shift + F3" },
  { "Replace", "Ctrl + H" },
  { "Move To Next Char", "Right" },
  { "Move To Previous Char", "Left" },
  { "Move To Next Word", "Ctrl + Right" },
  { "Move To Previous Word", "Ctrl + Left" },
  { "Move To Next Line", "Down" },
  { "Move To Previous Line", "Up" },
  { "Move To Start Of Line", "PgDown" },
  { "Move To End Of Line", "PgUp" },
  { "Move To Start Of Document", "Ctrl + Home" },
  { "Move To End Of Document", "Ctrl + End" },
  { "Select Next Char", "Shift + Right" },
  { "Select Previous Char", "Shift + Left" },
  { "Select Next Word", "Ctrl + Shift + Right" },
  { "Select Previous Word", "Ctrl + Shift + Left" },
  { "Select Next Line", "Shift + Down" },
  { "Select Previous Line", "Shift + Up" },
  { "Select Start Of Line", "Shift + PgDown" },
  { "Select End Of Line", "Shift + PgUp" }
};

ShortCutWidget::ShortCutWidget(QWidget *parent)
  : QWidget(parent)
{
  this->setContentsMargins(0, 0, 0, 0);
  this->setStyleSheet("padding:0;");
  m_data = std::make_shared<ShortCutData>();
  //
  auto total_layout = new QVBoxLayout();
  total_layout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(total_layout);

  auto data_layout = new QHBoxLayout();
  data_layout->setContentsMargins(0, 0, 0, 0);
  total_layout->addLayout(data_layout);

  m_shortcut_table_view = new QTableView(this);
  data_layout->addWidget(m_shortcut_table_view);

  this->setWindowTitle("Shortcuts");
  model = new ShortcutsModel(this);
  model->set_core_data(m_data);
  {

  }
  m_shortcut_table_view->setModel(model);
  // 设置可编辑
  m_shortcut_table_view->setEditTriggers(QAbstractItemView::DoubleClicked); // 双击进入编辑模式
  // 设置头部数据
  m_shortcut_table_view->horizontalHeader()->hide();
  m_shortcut_table_view->verticalHeader()->hide();

  //
  auto controll_layout = new QHBoxLayout();
  controll_layout->setContentsMargins(0, 0, 0, 0);
  total_layout->addLayout(controll_layout);

  QScrollBar *scrollBar = new QScrollBar(Qt::Horizontal, this);
  auto min_value = 0;
  auto max_value = m_data->get_max_page() - 1;
  qDebug() << max_value;
  scrollBar->setRange(min_value, max_value);
  scrollBar->setValue(min_value);
  controll_layout->addWidget(scrollBar);
  // 连接滚动条的信号到槽函数
  connect(scrollBar, &QScrollBar::valueChanged, [this](int value) {
    qDebug() << "Current page:" << value;
    // 更新页面内容
    this->m_data->set_page(value);
    m_shortcut_table_view->update();
  });

}

void ShortCutWidget::resizeEvent(QResizeEvent *event)
{
  m_shortcut_table_view->setColumnWidth(0, this->width() / 2);
  m_shortcut_table_view->setColumnWidth(1, this->width() / 2);
  QWidget::resizeEvent(event);
}

void  ShortCutWidget::keyPressEvent(QKeyEvent *event)
{
  QKeySequence keySeq(event->modifiers() + event->key()); // 将按键组合转换为QKeySequence对象
  QString keyStr = keySeq.toString(); // 将QKeySequence对象转换为字符串
  auto index = m_shortcut_table_view->currentIndex();
  qDebug() << index;
  if (index.isValid())
  {
    model->setData(index, keyStr);
  }
  qDebug() << "Pressed key: " << keyStr;
}

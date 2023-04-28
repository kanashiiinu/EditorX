#include "MainWindow.hpp"

#include <QApplication>
#include <QString>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  auto style_qss = R"(
QWidget {
    background-color: #272727;
    color: #ffffff;
}

QFrame {
  border: 1px solid lightblue;
}

QPushButton {
    background-color: #007acc;
    color: #ffffff;
    padding: 5px 10px;
    border: none;
    border-radius: 3px;
}

QPushButton:hover {
    background-color: #0099ff;
}

QPushButton:pressed {
    background-color: #005580;
}

QLineEdit {
    background-color: #ffffff;
    color: #000000;
    padding: 5px;
    border: none;
    border-radius: 3px;
}

QLineEdit:hover {
    background-color: #f0f0f0;
}

QScrollBar:vertical {
    width: 10px;
    background-color: #303030;
}

QScrollBar::handle:vertical {
    background-color: #808080;
}

QScrollBar::handle:vertical:hover {
    background-color: #a0a0a0;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 10px;
    background-color: #303030;
}

QScrollBar::add-line:vertical:hover, QScrollBar::sub-line:vertical:hover {
    background-color: #a0a0a0;
}

QCheckBox, QRadioButton {
    color: #ffffff;
}

QCheckBox::indicator, QRadioButton::indicator {
    width: 15px;
    height: 15px;
}

QCheckBox::indicator:checked, QRadioButton::indicator:checked {
    background-color: #007acc;
    border: none;
}

QLabel {
    color: #ffffff;
}
QTabWidget {
border:0;
}
QTabBar::tab{
    background-color: #272727;
    border-color:blue;
    font-size:8pt;
    color:#ced4da;    /*设置tab中的文本的颜色*/
    border-top-left-radius: 5px;    /*设置tab的边框的圆角（左上圆角）*/
    border-top-right-radius: 5px;    /*设置tab的边框的圆角（右上圆角）*/
    min-width: 8px;
    padding: 5px;
}
)";
//  w.setStyleSheet(style_qss);
    Q_UNUSED(style_qss);
    w.show();
    w.setFont(QFont("Fira Code"));
    return a.exec();
}

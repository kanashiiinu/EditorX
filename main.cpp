#include "MainWindow.hpp"

#include <QApplication>
#include <QString>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  w.setFont(QFont("Fira Code"));
  return a.exec();
}

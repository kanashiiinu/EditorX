#include "./gui/ShellDemo.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ShellDemo w;
  w.show();
  return a.exec();
}

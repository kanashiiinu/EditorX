#ifndef COLORDIALOG_HPP
#define COLORDIALOG_HPP

#include <QWidget>

class ColorDialog : public QWidget
{
    Q_OBJECT

public:
    ColorDialog(QWidget *parent = nullptr);
    ~ColorDialog();
};
#endif // COLORDIALOG_HPP

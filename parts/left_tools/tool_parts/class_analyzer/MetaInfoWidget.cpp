#include "MetaInfoWidget.hpp"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>

#include "ClassAnalyzer.hpp"

MetaInfoWidget::MetaInfoWidget(QWidget *parent) : QWidget(parent)
{
  auto v_layout = new QVBoxLayout();
  this->setLayout(v_layout);
  QFont font = QFont("Fira Code", 12);
  this->setFont(font);
  font.setBold(true);
  auto class_text_editor = new QTextEdit(this);
  class_text_editor->setFont(font);
  class_text_editor->setTextColor(Qt::white);
  auto output_editor = new QTextEdit(this);
  output_editor->setFont(font);
  auto btn = new QPushButton("detect", this);
  v_layout->addWidget(class_text_editor);
  v_layout->addWidget(btn);
  v_layout->addWidget(output_editor);

  m_class_analyzer = new ClassAnalyzer(this);
  connect(btn, &QPushButton::clicked, this, [this, class_text_editor, output_editor]() {
    QString test_script = R"(
          require_(func_is_virtual(destructor(c)), "interface destructor need virtual");
          require_(func_is_noexcept(destructor(c)), "interface destructor need noexcept");
          require_(variables_is_empty(c), "interface can't contain data members");
          functions(c) do:(f) = {
            if normal_fun(f, c) {
              require_(access_is_public(f), "interface functions must be public");
              require_(func_is_virtual(f), "interface functions must be virtual");
              require_(func_is_pure(f), "interface functions must be pure");
            }
          }
      )";
    const auto path = R"(../X/meta_class_scirpts/interface.class_description)";
    QFile fl(path);
    if (fl.open(QIODevice::ReadOnly)) {
      test_script = fl.readAll();
    }
    auto test_class = QString();
    test_class = class_text_editor->toPlainText();
    output_editor->setText(m_class_analyzer->detect_class_info(test_script, test_class));
  });
}

/*

class Shape {
public:
    virtual int area() const = 0;
    virtual void scale_by(double factor) = 0;
    virtual ~Shape() noexcept { print(); };
};

*/

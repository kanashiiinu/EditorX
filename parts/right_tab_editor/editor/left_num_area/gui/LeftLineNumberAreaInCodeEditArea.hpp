#ifndef LEFTLINENUMBERAREAINCODEEIDTAREA_HPP
#define LEFTLINENUMBERAREAINCODEEIDTAREA_HPP

#include <QWidget>

class CodeEditArea;

/** 实现编辑区左侧的行号
 * @brief The LeftLineNumberAreaInCodeEditArea class
 */
class LeftLineNumberAreaInCodeEditArea : public QWidget
{
  Q_OBJECT
 public:
  /**
   * @brief Constructor.
   * @param parent Pointer to parent QTextEdit widget.
   */
  explicit LeftLineNumberAreaInCodeEditArea(CodeEditArea *parent = nullptr);

  // Disable copying
  LeftLineNumberAreaInCodeEditArea(const LeftLineNumberAreaInCodeEditArea &) = delete;
  LeftLineNumberAreaInCodeEditArea &operator=(const LeftLineNumberAreaInCodeEditArea &) = delete;

  /** getting line number area size
   * @brief Overridden method for getting line number area size.
   */
  QSize sizeHint() const override;

 protected:
  void paintEvent(QPaintEvent *event) override;

 private:
  //借用
  CodeEditArea *m_code_edit_area;
};

#endif // LEFTLINENUMBERAREAINCODEEIDTAREA_HPP

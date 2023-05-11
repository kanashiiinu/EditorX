#ifndef CLASSANALYZER_H
#define CLASSANALYZER_H
#pragma once

#include <QObject>
#include <QString>
#include <QList>
#include <QVariant>
#include <functional>
//self
#include "ClassAnalyzerFieldInfo.hpp"


using Param = QVariant;
class ClassAnalyzer : public QObject
{
  Q_OBJECT
 public:

  static QMap<QString, Param> name_to_value;

  static QMap<QString, std::function<Param(Param, Param) >> name_to_func;

 public:
  explicit ClassAnalyzer(QObject *parent = nullptr);
  ~ClassAnalyzer() = default;
 public:

  /**
   * @brief 根据输入的script的规则检测class的内容,如 检测一个类是不是interface
   * @param test_script
   * @param test_class
   * @return 检测信息
   */
  auto detect_class_info(const QString &test_script, const QString &test_class)->QString;

  QString m_buffer;

  QString m_sample;

  QString m_simple_sample;
 signals:
  // 语法处理部分
 private:

  struct NodeData {
    QString name;
    QString type;
  };

  struct Node {
    NodeData data;
    Node *left = nullptr;
    Node *right = nullptr;
  };

  auto first_print(Node *root) ;
// name func

  /**
   * @brief 自底向上遍历树
   * @param root
   * @return
   */
  auto run_tree_by_from_bottom_order(Node *root)->Param ;

  /**
   * @brief 将一段代码分割成blocks
   * @param text_
   * @return
   */
  auto get_blocks(QString const &text_)->QStringList ;

  /**
   * @brief 处理一个block
   * @param block
   * @return
   */
  auto do_a_block(QString const &block)->Param;

  /**
   * @brief 对do_a_fun_call的输入remove_space
   * @param line
   * @return
   */
  auto remove_space_for_fun_call(QString const &line)->QString ;

  /**
   * @brief 处理调用函数
   * @param line_param
   * @return
   */
  auto do_a_fun_call(QString const &line_param)->Param ;

  /**
   * @brief 处理if
   * @param condition_and_lines
   * @return
   */
  auto do_a_if(QString const &condition_and_lines)->Param ;

  /**
   * @brief 处理for
   * @param claim_and_lines
   * @return
   */
  auto do_a_for(QString const &claim_and_lines)->Param ;

  /**
   * @brief 对形如一个class定义的代码进行处理
   * @param code_
   * @param class_
   */
  auto do_a_piece_of_code(QString const &code_, QString const &class_)->void ;

};

#endif // CLASSANALYZER_H

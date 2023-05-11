#ifndef CLASSANALYZERTOOLS_HPP
#define CLASSANALYZERTOOLS_HPP

#include <QString>
#include <QRegularExpression>
// self
#include "ClassAnalyzerClassInfo.hpp"

using OrignalText = QString;
using ClassText = QString;
auto class_text(OrignalText const &text, int from_index = 0, QVector<QString> class_list = {})->QVector<QString> ;

using ClassName = QString;

auto class_name(OrignalText const &text)-> ClassName ;

auto remove_comment(OrignalText const &text)-> QString ;

auto class_inner_str(QString const &text) ;

auto split_to_lines(QString class_str)-> QStringList ;

auto analyze_line(const QString &line)->QString ;

//field: [access][static][constexpr][const]name[= default_value];
//funciotn: [access][friend][static|virturl][inline][static][constexpr][return_type]func_name([param])[const][override;

auto analyze_field(QString const &line, QString const &access = "private")->FieldInfo ;

//funciotn: [access][friend][static|virturl][inline][static][constexpr][return_type]func_name([param])[const][override;
auto analyze_function(QString const &line, QString const &access = "private")->FunctionInfo ;

auto class_total_info(const QString &s)->ClassInfo ;

#endif // CLASSANALYZERTOOLS_HPP

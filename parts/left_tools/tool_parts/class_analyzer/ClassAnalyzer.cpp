#include "ClassAnalyzer.hpp"
//Qt
#include <QDebug>
#include <vector>
#include <string>
#include <QVector>
#include <QSet>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStringList>
#include <QVariant>
//std
#include <stack>
//self
#include "ClassAnalyzerTools.hpp"

QMap<QString, Param> ClassAnalyzer::name_to_value = QMap<QString, Param>();

QMap<QString, std::function<Param(Param, Param) >> ClassAnalyzer::name_to_func = QMap<QString, std::function<Param(Param, Param) >>();

//func
ClassAnalyzer::ClassAnalyzer(QObject *parent)
  : QObject(parent)
{

//  auto has_access = [](Param m, Param)->Param {
//    if (m.canConvert<FieldInfo>()) {
//      return not m.value<FieldInfo>().access.isEmpty();
//    }
//    if (m.canConvert<FunctionInfo>()) {
//      return not m.value<FunctionInfo>().access.isEmpty();
//    }
//    return -1;
//  };
  auto access_is_ = [](QString access)->std::function<Param(Param, Param)> {
    return [access](Param m, Param)->Param {
      if (m.canConvert<FieldInfo>()) {
        return (m.value<FieldInfo>().access == access);
      }
      if (m.canConvert<FunctionInfo>()) {
        return (m.value<FunctionInfo>().access == access);
      }
      return -1;
    };
  };
  auto access_is_private = access_is_("private");
  auto access_is_protected = access_is_("protected");
  auto access_is_public = access_is_("public");
  auto make_access_ = [](QString access)->std::function<Param(Param, Param)>  {
    return [access](Param m, Param)->Param {
      if (m.canConvert<FieldInfo>()) {
        return m.value<FieldInfo>().access = access;
      }
      if (m.canConvert<FunctionInfo>()) {
        return m.value<FunctionInfo>().access = access;
      }
      return -1;
    };
  };
  auto make_private = make_access_("private");
  auto make_protected =  make_access_("protected");
  auto make_public =  make_access_("public");

  auto val_is_ = [](QString word)->std::function<Param(Param, Param)>  {
    return [word](Param v, Param)->Param{
      if (v.canConvert<FieldInfo>()) {
        FieldInfo val = v.value<FieldInfo>();
        if (val.front_words.contains(word)) { return true; }
        if (val.type == word) { return true; }
        return false;
      }
      return -1;
    };
  };
  auto val_is_constexpr = val_is_("constexpr");
  auto val_is_static = val_is_("static");
  auto has_default_val = [](Param val_, Param)->Param {
    FieldInfo v = val_.value<FieldInfo>();
    return not v.default_value.isEmpty();
  };
  auto type_of = [](Param val_, Param)->Param {
    FieldInfo v = val_.value<FieldInfo>();
    return v.type;
  };
  auto func_is_ = [](QString word)->std::function<Param(Param, Param)> {
    return [word](Param f_, Param)->Param {
//        qDebug() << word;
      FunctionInfo f = f_.value<FunctionInfo>();
      if (f.front_words.contains(word)) { return true; }
      if (f.back_words.contains(word)) { return true; }
      return false;
    };
  };
  auto func_is_virtual = func_is_("virtual");
  auto func_is_static = func_is_("static");
  auto func_is_constexpr = func_is_("constexpr");
  auto func_is_friend = func_is_("friend");
  auto func_is_const = func_is_("const");
  auto func_is_pure = func_is_("0");
  auto func_is_noexcept = func_is_("noexcept");
  auto func_is_override = func_is_("override");

  auto functions = [](Param class_info_, Param)->Param {
    QVariant v;
    v.setValue(class_info_.value<ClassInfo>().functions);
    return v;
  };

  auto destructor = [](Param class_info_, Param)->Param {
    ClassInfo class_info = class_info_.value<ClassInfo>();
    auto name = "~" + class_info.class_name;
    for (auto &f : class_info.functions) {
      if (f.name == name) {
        QVariant v;
        v.setValue(f);
        return v;
      }
    }
    FunctionInfo f;
    f.name = name;
    f.access = "public";
    f.param = "()";
    f.type = "";
    QVariant v;
    v.setValue(f);
    return v;
  };

  auto normal_fun = [](Param f_, Param c_)->Param {
//    qDebug() << to_str(ClassAnalyzer::name_to_value["c"].value<ClassInfo>());
//    qDebug() << to_str(ClassAnalyzer::name_to_value["f"].value<FunctionInfo>());
    FunctionInfo f = f_.value<FunctionInfo>();
    ClassInfo c = c_.value<ClassInfo>();
    if (f.name == c.class_name) { return false;}
    if (f.name == "~" + c.class_name) { return false;}
    if (f.name == "operator") { return false;}
    if (f.type == "operator") { return false;}
    return true;
  };

  auto variables_is_empty = [](Param class_info_, Param)->Param  {
    ClassInfo c = class_info_.value<ClassInfo>();
    return c.fields.isEmpty();
  };
  auto require = [](Param false_then_break, Param tips)->Param {
    auto b = false_then_break.toBool();
    if (not b) {
      qDebug() << tips.toString();
    }
    return false_then_break;
  };

  auto require_ = [this](Param false_then_break, Param tips)->Param {
    auto b = false_then_break.toBool();
    if (not b) {
      this->m_buffer.append(tips.toString() + "\n");
    }
    return false_then_break;
  };

  name_to_func = {
    {"variables_is_empty", variables_is_empty},
    {"functions", functions},
    {"normal_fun", normal_fun},
    {"access_is_public", access_is_public},
    {"func_is_noexcept", func_is_noexcept},
    {"func_is_virtual", func_is_virtual},
    {"func_is_pure", func_is_pure},
    {"destructor", destructor},
    {"require",  require   },
    {"require_",  require_   },
    {"bool_true", [](Param, Param)->Param { return true; } },
    {"bool_false", [](Param, Param)->Param { return false; } },
    {"do_twice", [](Param, Param)->Param { QVariant p; p.setValue(QList<Param>{"", ""}); return p; } },
    //
    {"has_default_val", has_default_val},
    {"type_of", type_of},
  };

  QString ori_text = R"(
aer;
//awera;ahwerh
;
class P {
public:
    /**
     * @ constructor
     */
    P();
    virtual ~P() noexcept;
    int f();
private:
    inline int detail_of_f();
    int x;
    int y;
    const int* v;
    constexpr auto c = 1;
    static constexpr auto s_c = 1;
};
)";

  QString interface_shape = R"(
class Shape {
  public:
    virtual int area() const = 0;
    virtual void scale_by(double factor) = 0;
    virtual ~Shape() noexcept { print(); };
};
)";
  QString interface_shape_destructor_pure = R"(
class Shape {
  public:
    virtual int area() const = 0;
    virtual void scale_by(double factor) = 0;
    virtual ~Shape() noexcept = 0;
};
)";
  QString interface_shape_own_int = R"(
class Shape {
  public:
    int i;
    virtual int area() const = 0;
    virtual void scale_by(double factor) = 0;
    virtual ~Shape() noexcept { print(); };
};
)";

  QString enum_a = R"(
class EnumA {
  public:
    static constexpr int i = 1;
    static constexpr int j = 2;
    static constexpr int k = 3;
    int val;
    operator int();
};
)";


QString test_str = R"(require(bool_false(), "if condition is false then tips given");)";
//  do_a_line(test_str);
  QString test_if = QString("if bool_true() {\nrequire(bool_false(), \"1\");\n require(bool_false(), \"2\");\n}\n");
//  do_a_if(test_if);
  QString test_for = "do_twice() do:(f)={\nrequire(bool_false(), \"1\");\nrequire(bool_false(), \"2\");\n}\n";
//  do_a_for(test_for);

  QString test_whole = R"(
    require(func_is_virtual(destructor(c)), "interface destructor need virtual");
    require(func_is_noexcept(destructor(c)), "interface destructor need noexcept");
    require(variables_is_empty(c), "interface can't contain data members");
    functions(c) do:(f) = {
      if normal_fun(f, c) {
        require(access_is_public(f), "interface functions must be public");
        require(func_is_virtual(f), "interface functions must be virtual");
        require(func_is_pure(f), "interface functions must be pure");
      }
    }
)";

QString test_interface = R"(class Shape {
      public:
        virtual int area() const = 0;
        virtual void scale_by(double factor) = 0;
        virtual ~Shape() noexcept { print(); };
    };
  )";
//  do_a_piece_of_code(test_whole, test_interface);
  auto is_interface_class_version = [&](ClassInfo c_) {
    QVariant e;
    e.setValue(-1);
    QVariant c;
    c.setValue(c_);
    require(func_is_virtual(destructor(c, e), e), QString("interface destructor need virtual"));
    require(func_is_noexcept(destructor(c, e), e), "interface destructor need noexcept");
    require(variables_is_empty(c, e), "interface can't contain data members");
    for (auto f_ : c_.functions) {
      QVariant f;
      f.setValue(f_);
      if (normal_fun(f, c).toBool()) {
        require(access_is_public(f, e), "interface functions must be public");
        require(func_is_virtual(f, e), "interface functions must be virtual");
        require(func_is_pure(f, e), "interface functions must be pure");
      }
    }
  };
  Q_UNUSED(is_interface_class_version);
  /*
    class_name:xxx,
    inher: [access]name
    field: [access][static][constexpr][const]name[= default_value];
    funciotn: [access][friend][static|virturl][inline][static][constexpr][return_type]func_name([param])[const][override;
    inner_class:
    operator int() {}
    [return_type]  operator +=(){}
    inner_enum:
    inner_enum_class:
  */
  /*
  i:int = 10; => int i = 10;$
  f:(i:int, inout x:int, ->:int)={}; => int f(int i, int& x){}
  for xs :(x)={} = for (auto x : xs){}
  x|round|add_2; = add_2(round(x));$
  x:= f(_, _, 3); => auto x = [&,p_3 = 3](auto p_1, auto p_2){
      return f(p_1, p_2, p_3);
  };
  res = s | transform(f); => transform(s.begin(), s.end(), res.begin(), f);$
  */
}


auto ClassAnalyzer::detect_class_info(const QString &test_script, const QString &test_class)->QString {
  m_buffer.clear();
  do_a_piece_of_code(test_script, test_class);
  m_buffer.append("finished");
  return m_buffer;
}


auto ClassAnalyzer::first_print(Node *root) {
  if (not root) { return; }
  qDebug() << root->data.name << ", " << root->data.type;
  first_print(root->left);
  first_print(root->right);
}

// name func
auto ClassAnalyzer::run_tree_by_from_bottom_order(Node *root)->Param
{
  if (not root) { return -1; }
  auto &node = root->data;
  if (root->data.type == "value")
  {
    if (ClassAnalyzer::name_to_value.contains(node.name))
    {
      return ClassAnalyzer::name_to_value[node.name];
    }
    return root->data.name;
  }
  if (root->data.type == "function")
  {
    return ClassAnalyzer::name_to_func[root->data.name](run_tree_by_from_bottom_order(root->left), run_tree_by_from_bottom_order(root->right));
  }
  return -2; // 没有任何意义的返回值
}

auto ClassAnalyzer::get_blocks(QString const &text_)->QStringList {
  auto list = text_.split("\n");
  auto left = 0;
  auto blocks = QStringList();
  auto block = QString();
  for (const auto &l : list) {
    if (not l.isEmpty()) {
      if (l.contains('{')) {
        block.append(l + "\n");
        ++left;
      } else if (l.contains('}')) {
        block.append(l + "\n");
        --left;
        if (left == 0) {
          blocks.append(block);
          block.clear();
        }
      } else if (left) {
        block.append(l + "\n");
      } else {
        blocks.append(l);
      }
    }
  }
  return blocks;
}

auto ClassAnalyzer::remove_space_for_fun_call(QString const &line)->QString {
  QString res;
  auto left = 0;
  for (int i = 0; i < line.size(); ++i) {
    if (line.at(i) == ' ') {
      if (left % 2 != 0) {
        res.push_back(line.at(i));
      }
      continue;
    }
    if (line.at(i) == '\"') {
      ++left;
    }
    res.push_back(line.at(i));
  }
  return res;
}

auto ClassAnalyzer::do_a_fun_call(QString const &line_param)->Param {
  auto line = line_param;
  line = remove_space_for_fun_call(line);
  int word_begin_index = 0;
  int word_end_index = 0;
  auto word = QString();
  auto root = new Node();
  std::stack<Node *> roots = std::stack<Node *>();
  roots.push(root);
  for (int len = line.size(); word_end_index < line.size(); ++word_end_index) {
    if (line.at(word_end_index) == '(') {
      word = line.mid(word_begin_index, word_end_index - word_begin_index);
      auto word_node = new Node();
      word_node->data.name = word;
      word_node->data.type = "function";
      if (not roots.top()->left) {
        roots.top()->left = word_node;
      } else {
        roots.top()->right = word_node;
      }
      roots.push(word_node);
      word_begin_index = word_end_index + 1;
    }
    if (line.at(word_end_index) == ')') {
      word = line.mid(word_begin_index, word_end_index - word_begin_index);
      if (not word.isEmpty()) {
        word.remove("\"");
        auto word_node = new Node();
        word_node->data.name = word;
        word_node->data.type = "value";
        if (not roots.top()->left) {
          roots.top()->left = word_node;
        } else {
          roots.top()->right = word_node;
        }
      }
      roots.pop();
      ++word_end_index;
      for (; word_end_index < len; ++word_end_index) {
        if (line.at(word_end_index) == ')') {
          roots.pop();
          continue;
        }
        if (line.at(word_end_index) == ',') {
          continue;
        }
        word_begin_index = word_end_index;
        --word_end_index;
        break;
      }
      continue;
    }
    if (line.at(word_end_index) == ',') {
      word = line.mid(word_begin_index, word_end_index - word_begin_index);
      auto word_node = new Node();
      word_node->data.name = word;
      word_node->data.type = "value";
      if (not roots.top()->left) {
        roots.top()->left = word_node;
      } else {
        roots.top()->right = word_node;
      }
      word_begin_index = word_end_index + 1;
    }
  }
  return run_tree_by_from_bottom_order(root->left);
}

auto ClassAnalyzer::do_a_if(QString const &condition_and_lines)->Param
{
  auto condition_begin_index = condition_and_lines.indexOf("if");
  auto condition_end_index = condition_and_lines.indexOf("{");
  auto condition = condition_and_lines.mid(condition_begin_index + 2, condition_end_index - condition_begin_index - 2);
  auto line_begin_index = condition_and_lines.indexOf("{");
  auto line_end_index = condition_and_lines.lastIndexOf("}");
  auto lines = condition_and_lines.mid(line_begin_index + 1, line_end_index - line_begin_index - 1);
  auto flag = do_a_fun_call(condition.trimmed()).toBool();
  if (flag)
  {
    for (const auto &b : get_blocks(lines))
    {
      do_a_block(b);
    }
  }
  return -2;
}

// name
auto ClassAnalyzer::do_a_for(QString const &claim_and_lines)->Param {
  auto front_begin_index = 0;
  auto front_end_index = claim_and_lines.indexOf(" do:");
  auto front = claim_and_lines.mid(front_begin_index, front_end_index - front_begin_index);
  auto value_name_begin_index = claim_and_lines.indexOf('(', front_end_index);
  auto value_name_end_index = claim_and_lines.indexOf(')', front_end_index);
  auto value_name = claim_and_lines.mid(value_name_begin_index + 1, value_name_end_index - value_name_begin_index - 1);
  auto line_begin_index = claim_and_lines.indexOf("{");
  auto line_end_index = claim_and_lines.lastIndexOf("}");
  auto lines = claim_and_lines.mid(line_begin_index + 1, line_end_index - line_begin_index - 1);
  lines = lines.trimmed();
  for (const auto &f : do_a_fun_call(front).value<QList<Param>>()) {
    QVariant v;
    v.setValue(f);
    ClassAnalyzer::name_to_value[value_name] = v;
    for (const auto &b : get_blocks(lines)) {
      do_a_block(b);
    }
  }
  ClassAnalyzer::name_to_value.remove(value_name);
  return -2;
}


auto ClassAnalyzer::do_a_block(QString const &block)->Param {
  auto if_index = block.indexOf("if ");
  auto for_index = block.indexOf(" do:");
  if (if_index != -1 and for_index != -1) {
    if (if_index < for_index) {
      return do_a_if(block);
    } else {
      return do_a_for(block);
    }
  }
  if (if_index != -1) {
    return do_a_if(block);
  }
  if (for_index != -1) {
    return do_a_for(block);
  }
  return do_a_fun_call(block);
  return -1;
}

//name
auto ClassAnalyzer::do_a_piece_of_code(QString const &code_, QString const &class_)->void
{
  auto c = class_total_info(class_);
  QVariant v;
  v.setValue(c);
  ClassAnalyzer::name_to_value["c"] = v;
  for (const auto &b : get_blocks(code_))
  {
    do_a_block(b);
  }
  qDebug() << "finish";
}


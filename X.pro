QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    parts/left_tools/gui/LeftToolListAndToolWindow.cpp \
    parts/left_tools/tool_parts/class_analyzer/fun/ClassAnalyzer.cpp \
    parts/left_tools/tool_parts/class_analyzer/fun/ClassAnalyzerClassInfo.cpp \
    parts/left_tools/tool_parts/class_analyzer/fun/ClassAnalyzerFieldInfo.cpp \
    parts/left_tools/tool_parts/class_analyzer/fun/ClassAnalyzerFunctionInfo.cpp \
    parts/left_tools/tool_parts/class_analyzer/fun/ClassAnalyzerTools.cpp \
    parts/left_tools/tool_parts/class_analyzer/gui/MetaInfoWidget.cpp \
    parts/left_tools/tool_parts/code_analysis/gui/CodeAnalysisWidget.cpp \
    parts/left_tools/tool_parts/compare_file_in_dir/fun/FileMd5Worker.cpp \
    parts/left_tools/tool_parts/compare_file_in_dir/gui/CompareFileWidget.cpp \
    parts/left_tools/tool_parts/font_and_color/fun/SimpleTextColorScheme.cpp \
    parts/left_tools/tool_parts/font_and_color/gui/ColorSelectWidget.cpp \
    parts/left_tools/tool_parts/font_and_color/gui/FontAndColorWidget.cpp \
    parts/left_tools/tool_parts/font_and_color/gui/FontSelectWidget.cpp \
    parts/left_tools/tool_parts/note/gui/NoteWidget.cpp \
    parts/left_tools/tool_parts/search/gui/FindAndReplaceWidget.cpp \
    parts/left_tools/tool_parts/theme/fun/ThemeOperation.cpp \
    parts/left_tools/tool_parts/theme/fun/ThemesManager.cpp \
    parts/left_tools/tool_parts/theme/gui/ThemeWidget.cpp \
    parts/menu_bar/gui/MenuBar.cpp \
    parts/right_tab_editor/editor/gui/CodeEditArea.cpp \
    parts/right_tab_editor/editor/left_num_area/gui/LeftLineNumberAreaInCodeEditArea.cpp \
    parts/right_tab_editor/editor/syntax_highlight/fun/MyHighlighter.cpp \
    parts/right_tab_editor/gui/TabAndTextAreaEtc.cpp \
    parts/scatter/gui/MostLeftUnknownWindow.cpp \
    parts/scatter/screenshot/ScreenWidget.cpp \
    parts/scatter/shortcut/fun/ShortCutData.cpp \
    parts/scatter/shortcut/fun/ShortCutModel.cpp \
    parts/scatter/shortcut/gui/ShortCutWidget.cpp \
    parts/scatter/work_time_reminder/fun/MaskWidget.cpp \
    parts/scatter/work_time_reminder/gui/TimeReminderWidget.cpp \
    parts/status_bar/gui/StatusBar.cpp

HEADERS += \
    MainWindow.hpp \
    parts/left_tools/gui/LeftToolListAndToolWindow.hpp \
    parts/left_tools/tool_parts/class_analyzer/fun/ClassAnalyzer.hpp \
    parts/left_tools/tool_parts/class_analyzer/fun/ClassAnalyzerClassInfo.hpp \
    parts/left_tools/tool_parts/class_analyzer/fun/ClassAnalyzerFieldInfo.hpp \
    parts/left_tools/tool_parts/class_analyzer/fun/ClassAnalyzerFunctionInfo.hpp \
    parts/left_tools/tool_parts/class_analyzer/fun/ClassAnalyzerTools.hpp \
    parts/left_tools/tool_parts/class_analyzer/gui/MetaInfoWidget.hpp \
    parts/left_tools/tool_parts/code_analysis/gui/CodeAnalysisWidget.hpp \
    parts/left_tools/tool_parts/compare_file_in_dir/fun/FileMd5Worker.hpp \
    parts/left_tools/tool_parts/compare_file_in_dir/gui/CompareFileWidget.hpp \
    parts/left_tools/tool_parts/font_and_color/fun/SimpleTextColorScheme.hpp \
    parts/left_tools/tool_parts/font_and_color/gui/ColorSelectWidget.hpp \
    parts/left_tools/tool_parts/font_and_color/gui/FontAndColorWidget.hpp \
    parts/left_tools/tool_parts/font_and_color/gui/FontSelectWidget.hpp \
    parts/left_tools/tool_parts/note/gui/NoteWidget.hpp \
    parts/left_tools/tool_parts/search/gui/FindAndReplaceWidget.hpp \
    parts/left_tools/tool_parts/theme/fun/ThemeOperation.hpp \
    parts/left_tools/tool_parts/theme/fun/ThemesManager.hpp \
    parts/left_tools/tool_parts/theme/gui/ThemeWidget.hpp \
    parts/menu_bar/gui/MenuBar.hpp \
    parts/right_tab_editor/editor/gui/CodeEditArea.hpp \
    parts/right_tab_editor/editor/left_num_area/gui/LeftLineNumberAreaInCodeEditArea.hpp \
    parts/right_tab_editor/editor/syntax_highlight/fun/MyHighlighter.hpp \
    parts/right_tab_editor/gui/TabAndTextAreaEtc.hpp \
    parts/scatter/gui/MostLeftUnknownWindow.hpp \
    parts/scatter/screenshot/ScreenWidget.hpp \
    parts/scatter/shortcut/fun/ShortCutData.hpp \
    parts/scatter/shortcut/fun/ShortCutModel.hpp \
    parts/scatter/shortcut/gui/ShortCutWidget.hpp \
    parts/scatter/work_time_reminder/fun/MaskWidget.hpp \
    parts/scatter/work_time_reminder/gui/TimeReminderWidget.hpp \
    parts/status_bar/gui/StatusBar.hpp \
    tools/tools.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


include($$PWD/parts/scatter/terminate/terminate.pri)

RESOURCES += \
    qss.qrc

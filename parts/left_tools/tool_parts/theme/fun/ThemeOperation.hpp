#pragma once

#include <QString>
#include <QList>

namespace theme {

QStringList get_project_default_theme_path();

QString from_url_to_name(QString const &path);

}


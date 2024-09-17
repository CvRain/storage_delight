//
// Created by cvrain on 24-9-17.
//

#include "theme_provider.hpp"

ThemeProvider::ThemeProvider(QObject *parent)
    : QObject(parent) {
    load_theme(QString{":/theme/CatppuccinLatte.json"});
}

ThemeProvider* ThemeProvider::create(QQmlEngine *engine, QJSEngine *scriptEngine) {
    qDebug() << "ThemeProvider::create";
    return new ThemeProvider(engine);
}

void ThemeProvider::load_theme(const QString &theme_path) {
    const auto load_result = Theme::load_theme_from_json(theme_path);
    if (load_result.base == QString{} || load_result.base.isEmpty()) {
        qDebug() << "load_theme_from_json failed";
        return;
    }
    current_theme = Theme::CatppuccinThemes{load_result};
    emit theme_changed();
}

Theme::CatppuccinThemes ThemeProvider::get_current_theme() {
    return current_theme;
}

QString ThemeProvider::color_base() const {
    return current_theme.base;
}

QString ThemeProvider::color_peach() const {
    return current_theme.peach;
}

//
// Created by cvrain on 24-9-17.
//

#ifndef THEME_PROVIDER_HPP
#define THEME_PROVIDER_HPP

#include <qqmlengine.h>
#include <qqmlintegration.h>
#include <QObject>

#include "schema/theme.hpp"

class ThemeProvider : public QObject {
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT

public:
    Q_PROPERTY(Theme::CatppuccinThemes current_theme READ get_current_theme NOTIFY theme_selected);

    explicit ThemeProvider(QObject *parent = nullptr);

    static ThemeProvider* create(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE void load_theme(const QString &theme_path);

    Q_INVOKABLE Theme::CatppuccinThemes get_current_theme();

signals:
    void theme_changed();

    void theme_selected();

private:
    Theme::CatppuccinThemes current_theme;
};


#endif //THEME_PROVIDER_HPP

//
// Created by cvrain on 24-9-17.
//

#ifndef THEME_HPP
#define THEME_HPP

#include <QObject>
#include <QColor>
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <nlohmann/json.hpp>

namespace Theme {
    class CatppuccinThemes : public QObject {
        Q_OBJECT

    public:
        explicit CatppuccinThemes(QObject *parent = nullptr) : QObject(parent) {}

        ~CatppuccinThemes() override = default;

        CatppuccinThemes(const CatppuccinThemes &other) {
            rosewater = other.rosewater;
            flagingo = other.flagingo;
            pink = other.pink;
            mauve = other.mauve;
            red = other.red;
            maroon = other.maroon;
            peach = other.peach;
            yellow = other.yellow;
            green = other.green;
            teal = other.teal;
            sky = other.sky;
            sapphire = other.sapphire;
            blue = other.blue;
            lavender = other.lavender;
            text = other.text;
            subtext1 = other.subtext1;
            subtext0 = other.subtext0;
            overlay2 = other.overlay2;
            overlay1 = other.overlay1;
            overlay0 = other.overlay0;
            surface2 = other.surface2;
            surface1 = other.surface1;
            surface0 = other.surface0;
            base = other.base;
            mantle = other.mantle;
            crust = other.crust;
        }

        CatppuccinThemes &operator=(const CatppuccinThemes &other) {
            rosewater = other.rosewater;
            flagingo = other.flagingo;
            pink = other.pink;
            mauve = other.mauve;
            red = other.red;
            maroon = other.maroon;
            peach = other.peach;
            yellow = other.yellow;
            green = other.green;
            teal = other.teal;
            sky = other.sky;
            sapphire = other.sapphire;
            blue = other.blue;
            lavender = other.lavender;
            text = other.text;
            subtext1 = other.subtext1;
            subtext0 = other.subtext0;
            overlay2 = other.overlay2;
            overlay1 = other.overlay1;
            overlay0 = other.overlay0;
            surface2 = other.surface2;
            surface1 = other.surface1;
            surface0 = other.surface0;
            base = other.base;
            mantle = other.mantle;
            crust = other.crust;
            return *this;
        }

        QString rosewater{};
        QString flagingo{};
        QString pink{};
        QString mauve{};
        QString red{};
        QString maroon{};
        QString peach{};
        QString yellow{};
        QString green{};
        QString teal{};
        QString sky{};
        QString sapphire{};
        QString blue{};
        QString lavender{};
        QString text{};
        QString subtext1{};
        QString subtext0{};
        QString overlay2{};
        QString overlay1{};
        QString overlay0{};
        QString surface2{};
        QString surface1{};
        QString surface0{};
        QString base{};
        QString mantle{};
        QString crust{};
    };

    inline QString read_file(const QString &url) {
        qDebug() << "Try to open: " << url;
        // 创建 QFile 对象
        QFile file{url};

        // 尝试打开文件
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "Error opening theme file: " << file.errorString();
            return {}; // 返回空字符串如果文件无法打开
        }

        // 使用 QTextStream 读取文件内容
        QTextStream in(&file);
        QString content = in.readAll();

        // 关闭文件
        file.close();

        return content; // 返回读取的内容
    }

    inline CatppuccinThemes load_theme_from_json(const QString &json_path) {
        const auto file = read_file(json_path);
        nlohmann::json theme_json;
        try {
            theme_json = nlohmann::json::parse(file.toLocal8Bit().toStdString());
        } catch (const nlohmann::json::exception& e) {
            qDebug() << "Error while parsing theme JSON file: " << e.what();
            return CatppuccinThemes{};
        }

        CatppuccinThemes theme;
        theme.rosewater = QString::fromStdString(theme_json["rosewater"].get<std::string>());
        theme.flagingo = QString::fromStdString(theme_json["flagingo"].get<std::string>());
        theme.pink = QString::fromStdString(theme_json["pink"].get<std::string>());
        theme.mauve = QString::fromStdString(theme_json["mauve"].get<std::string>());
        theme.red = QString::fromStdString(theme_json["red"].get<std::string>());
        theme.maroon = QString::fromStdString(theme_json["maroon"].get<std::string>());
        theme.peach = QString::fromStdString(theme_json["peach"].get<std::string>());
        theme.yellow = QString::fromStdString(theme_json["yellow"].get<std::string>());
        theme.green = QString::fromStdString(theme_json["green"].get<std::string>());
        theme.teal = QString::fromStdString(theme_json["teal"].get<std::string>());
        theme.sky = QString::fromStdString(theme_json["sky"].get<std::string>());
        theme.sapphire = QString::fromStdString(theme_json["sapphire"].get<std::string>());
        theme.blue = QString::fromStdString(theme_json["blue"].get<std::string>());
        theme.lavender = QString::fromStdString(theme_json["lavender"].get<std::string>());
        theme.text = QString::fromStdString(theme_json["text"].get<std::string>());
        theme.subtext1 = QString::fromStdString(theme_json["subtext1"].get<std::string>());
        theme.subtext0 = QString::fromStdString(theme_json["subtext0"].get<std::string>());
        theme.overlay2 = QString::fromStdString(theme_json["overlay2"].get<std::string>());
        theme.overlay1 = QString::fromStdString(theme_json["overlay1"].get<std::string>());
        theme.overlay0 = QString::fromStdString(theme_json["overlay0"].get<std::string>());
        theme.surface2 = QString::fromStdString(theme_json["surface2"].get<std::string>());
        theme.surface1 = QString::fromStdString(theme_json["surface1"].get<std::string>());
        theme.surface0 = QString::fromStdString(theme_json["surface0"].get<std::string>());
        theme.base = QString::fromStdString(theme_json["base"].get<std::string>());
        theme.mantle = QString::fromStdString(theme_json["mantle"].get<std::string>());
        theme.crust = QString::fromStdString(theme_json["crust"].get<std::string>());
        return theme;
    }
}

#endif //THEME_HPP

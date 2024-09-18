#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QDir>
#include <QDirIterator>

#include "src/theme_provider.hpp"
#include "src/schema/theme.hpp"

int main(int argc, char *argv[]) {
    const QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const auto connect_result = QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    Q_ASSERT(connect_result);

    qmlRegisterType<Theme::CatppuccinThemes>("Theme", 1, 0, "CatppuccinThemes");
    qmlRegisterSingletonType<ThemeProvider>("Theme", 1, 0, "ThemeProvider", ThemeProvider::create);

    //遍历qrc下的所有目录文件,并输出
    QDirIterator it(":/qt", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
    }


    const QUrl app_entry_path{QStringLiteral("qrc:/qt/qml/storage_delight_compass/ui/Main.qml")};
    engine.load(app_entry_path);

    return QGuiApplication::exec();
}

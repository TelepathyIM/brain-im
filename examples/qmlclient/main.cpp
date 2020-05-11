#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QtQml/qqml.h>

#ifdef USE_TELEPATHY
#include <TelepathyQt/Types>

#include "AccountsModel.hpp"
#include "AccountParameterModel.hpp"
#include "ConnectionManagersModel.hpp"
#include "ProtocolsModel.hpp"
#endif

#include "Event.hpp"
#include "Types.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}

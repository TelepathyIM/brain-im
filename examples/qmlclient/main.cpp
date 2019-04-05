#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QtQml/qqml.h>

#ifdef USE_TELEPATHY
#include "TelepathyMessagesModel.hpp"

#include <TelepathyQt/Types>

#include "AccountsModel"
#include "AccountParameterModel"
#include "ConnectionManagersModel"
#include "ContactsModel.hpp"
#include "ProtocolsModel.hpp"
#endif
#include "MessagesModel.hpp"
#include "Event.hpp"
#include "Types.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    BrainIM::MessagesModel model;

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}

//#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include <QTableView>
#include <TelepathyQt/Types>

#include "AccountsModel"
#include "ConnectionManagersModel"
#include "ProtocolsModel"
#include "AccountParameterModel"

int main(int argc, char *argv[])
{
//    QApplication app(argc, argv);
    QGuiApplication app(argc, argv);

    qmlRegisterType<AccountsModel>("BrainIM", 0, 1, "AccountsModel");
    qmlRegisterType<ConnectionManagersModel>("BrainIM", 0, 1, "ConnectionManagersModel");
    qmlRegisterType<ProtocolsModel>("BrainIM", 0, 1, "ProtocolsModel");
    qmlRegisterType<AccountParameterModel>("BrainIM", 0, 1, "AccountParameterModel");

    qmlRegisterUncreatableType<Tp::Account>("TelepathyQt", 0, 1, "Account", "The class can be created only from C++");
//    qmlRegisterUncreatableType<Tp::Profile>("TelepathyQt", 0, 1, "Profile", "The class can be created only from C++");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("main.qml")));

    Tp::registerTypes();

//    AccountsModel model;

//    QTableView view;
//    view.setModel(&model);
//    view.show();


    return app.exec();
}

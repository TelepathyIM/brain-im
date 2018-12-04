#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>

#ifdef USE_TELEPATHY
#include "AccountsModel"
#include "AccountParameterModel"
#include "ConnectionManagersModel"
#include "ProtocolsModel.hpp"
#endif
#include "Event.hpp"

static QObject *brain_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    BrainIM::Brain *brain = new BrainIM::Brain;
    return brain;
}

class BrainIMQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    BrainIMQmlPlugin(QObject *parent = nullptr) :
        QQmlExtensionPlugin(parent)
    {
        qRegisterMetaType<BrainIM::Peer>("BrainIM::Peer");
    }

    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QByteArray(uri) == QByteArray("BrainIM"));

#ifdef USE_TELEPATHY
        qmlRegisterType<AccountsModel>(uri, 0, 1, "AccountsModel");
        qmlRegisterType<ConnectionManagersModel>(uri, 0, 1, "ConnectionManagersModel");
        qmlRegisterType<ProtocolsModel>(uri, 0, 1, "ProtocolsModel");
        qmlRegisterType<AccountParameterModel>(uri, 0, 1, "AccountParameterModel");
#endif

        const QString cppOnlyClass = QStringLiteral("The class can be created only from C++");

        qmlRegisterSingletonType<BrainIM::Brain>(uri, 0, 1, "Brain", &brain_provider);

        qmlRegisterUncreatableMetaObject(BrainIM::staticMetaObject,
                                         uri, 0, 1,                 // major and minor version of the import
                                         "BrainIM",            // name in QML
                                         QLatin1String("Error: only enums")); // error in case someone tried to create the 'namespace' object
    }
};

#include "plugin.moc"

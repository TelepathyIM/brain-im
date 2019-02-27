#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>

#ifdef USE_TELEPATHY
#include "AccountsModel"
#include "AccountParameterModel"
#include "ConnectionManagersModel"
#include "ProtocolsModel.hpp"
#endif
#include "ContactsModel.hpp"
#include "MessagesModel.hpp"
#include "Event.hpp"

#include "Types.hpp"

class MessageSender : public QObject
{
    Q_OBJECT
    Q_PROPERTY(BrainIM::Peer peer READ peer WRITE setPeer NOTIFY peerChanged)
//    Q_PROPERTY(Telegram::Client::DeclarativeClient *target READ target WRITE setTarget NOTIFY targetChanged)
//    Q_PROPERTY(Telegram::MessageReference messageRef)
public:
    explicit MessageSender(QObject *parent = nullptr) :
        QObject(parent)
    {
    }

    BrainIM::Peer peer() const { return m_peer; }
    void setPeer(const BrainIM::Peer peer)
    {
        if (m_peer == peer) {
            return;
        }
        m_peer = peer;
        // TODO: Monitor the peer dialog draft changed signal
        emit peerChanged(peer);
    }

public slots:
    void setText(const QString &text)
    {
        m_text = text;
    }
    void setGeoPoint(double latitude, double longitude)
    {
        qWarning() << Q_FUNC_INFO << latitude << longitude;
    }
    void setContact()
    {

    }
    void setMedia()
    {

    }
    void setWebUrl()
    {

    }
    void setSticker()
    {

    }

    void sendMessage()
    {
        emit messageSent(m_text, m_peer);
    }

signals:
    void peerChanged(BrainIM::Peer peer);
    void messageSent(const QString &message, const BrainIM::Peer peer);
//    void draftChanged(const QString &message, const Telegram::Peer peer);

protected:
    BrainIM::Peer m_peer;
    QString m_text;
};

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
        qmlRegisterType<MessageSender>(uri, 0, 1, "MessageSender");
        qmlRegisterType<BrainIM::ContactsModel>(uri, 0, 1, "ContactsModel");
        qmlRegisterType<BrainIM::MessagesModel>(uri, 0, 1, "MessagesModel");

        const QString cppOnlyClass = QStringLiteral("The class can be created only from C++");

        //qmlRegisterType<BrainIM::MessageEntity>(uri, 0, 1, "MessageEntity");
        qmlRegisterSingletonType<BrainIM::Brain>(uri, 0, 1, "Brain", &brain_provider);

        qmlRegisterUncreatableMetaObject(BrainIM::staticMetaObject,
                                         uri, 0, 1,                 // major and minor version of the import
                                         "BrainIM",            // name in QML
                                         QLatin1String("Error: only enums")); // error in case someone tried to create the 'namespace' object

        qmlRegisterUncreatableType<BrainIM::PeerEnums>(uri, 0, 1, "Peer", cppOnlyClass);
        qmlRegisterUncreatableType<BrainIM::EventEnums>(uri, 0, 1, "Event", cppOnlyClass);
        qmlRegisterUncreatableType<BrainIM::MessageEnums>(uri, 0, 1, "Message", cppOnlyClass);
        qmlRegisterUncreatableType<BrainIM::ServiceActionEnums>(uri, 0, 1, "ServiceAction", cppOnlyClass);
    }
};

#include "plugin.moc"

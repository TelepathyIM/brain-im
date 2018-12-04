#include "DBusServiceHelper.hpp"

#include <QDBusInterface>
#include <QDBusConnectionInterface>
#include <QDBusPendingReply>

#include <QDebug>

DBusServiceHelper::DBusServiceHelper(QObject *parent, const QDBusConnection &dbusConnection)
    : QObject(parent),
      m_connection(dbusConnection),
      m_initialized(false)
{
    if (!m_connection.isConnected()) {
        qDebug() << "Failed to connect to DBus";
        return;
    }

    m_connection.connect(QStringLiteral("org.freedesktop.DBus"),
                       QStringLiteral("/org/freedesktop/DBus"),
                       QStringLiteral("org.freedesktop.DBus"),
                       QStringLiteral("NameOwnerChanged"),
                       QStringList(), QString(),
                       this, SLOT(onServiceOwnerChanged(QString, QString, QString)));

    QDBusPendingCallWatcher *callWatcher = nullptr;

    QDBusPendingCall asyncListNames = QDBusConnection::sessionBus().interface()->asyncCall(QLatin1String("ListNames"));
    callWatcher = new QDBusPendingCallWatcher(asyncListNames, this);
    connect(callWatcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(onNameListReceived(QDBusPendingCallWatcher*)));

    QDBusPendingCall asyncListActivatableNames = QDBusConnection::sessionBus().interface()->asyncCall(QLatin1String("ListActivatableNames"));
    callWatcher = new QDBusPendingCallWatcher(asyncListActivatableNames, this);
    connect(callWatcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(onActivatableNameListReceived(QDBusPendingCallWatcher*)));
}

bool DBusServiceHelper::isInitialized() const
{
    return !m_activeNames.isEmpty() && !m_activatableNames.isEmpty();
}

QStringList DBusServiceHelper::listNames()
{
    if (!isInitialized())
        return QStringList();

    return m_activeNames;
}

QStringList DBusServiceHelper::listActivatableNames()
{
    if (!isInitialized())
        return QStringList();

    return m_activatableNames;
}

void DBusServiceHelper::onNameListReceived(QDBusPendingCallWatcher *callWatcher)
{
    QDBusPendingReply<QStringList> reply = *callWatcher;
    if (reply.isError()) {
        qDebug() << Q_FUNC_INFO << reply.error();
        return;
    }

    callWatcher->deleteLater();

//    QDBusConnectionInterface *interface = QDBusConnection::sessionBus().interface();

    m_activeNames = reply.value();

//    Q_FOREACH (const QString &service, reply.value()) {
//        if (!service.startsWith(s_telepathyServicePrefix)) {
//            continue;
//        }

//        const QDBusReply<QString> owner = interface->serviceOwner(service);

//        onServiceOwnerChanged(service, /* old owner */ QString(), owner);
//    }

    if (isInitialized()) {
        emit initialized();
    }
}

void DBusServiceHelper::onActivatableNameListReceived(QDBusPendingCallWatcher *callWatcher)
{
    QDBusPendingReply<QStringList> reply = *callWatcher;
    if (reply.isError()) {
        qDebug() << Q_FUNC_INFO << reply.error();
        return;
    }

    callWatcher->deleteLater();

//    QDBusConnectionInterface *interface = QDBusConnection::sessionBus().interface();

    m_activatableNames = reply.value();

    if (isInitialized()) {
        emit initialized();
    }
}

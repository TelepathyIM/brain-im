#ifndef BRAIN_IM_DBUS_SERVICE_HELPER
#define BRAIN_IM_DBUS_SERVICE_HELPER

#include <QDBusConnection>
#include <QDBusPendingCallWatcher>
#include <QStringList>

class DBusServiceHelper : public QObject
{
    Q_OBJECT
public:
    DBusServiceHelper(QObject *parent = nullptr,
                      const QDBusConnection &dbusConnection = QDBusConnection::sessionBus());

    bool isInitialized() const;
    QStringList listNames();
    QStringList listActivatableNames();

Q_SIGNALS:
    void initialized();

protected Q_SLOTS:
    void onNameListReceived(QDBusPendingCallWatcher *callWatcher);
    void onActivatableNameListReceived(QDBusPendingCallWatcher *callWatcher);

protected:
    QDBusConnection m_connection;
    QStringList m_activeNames;
    QStringList m_activatableNames;
    bool m_initialized;
};

#endif // BRAIN_IM_DBUS_SERVICE_HELPER

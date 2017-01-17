#pragma once

#include <TelepathyQt/PendingOperation>

#include <QDBusConnection>
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

signals:
    void initialized();

protected slots:
    void onNameListReceived(QDBusPendingCallWatcher *callWatcher);
    void onActivatableNameListReceived(QDBusPendingCallWatcher *callWatcher);

protected:
    QDBusConnection m_connection;
    QStringList m_activeNames;
    QStringList m_activatableNames;
    bool m_initialized;
};

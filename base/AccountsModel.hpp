#pragma once

#include <QAbstractTableModel>

#include <TelepathyQt/Types>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/AccountManager>

class AccountsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Role {
        AccountObject,
        DisplayName,
        Enabled,
        CmName,
        ProtocolName,
        ServiceName,
        UniqueIdentifier,
        RolesCount,
        InvalidRole
    };

    explicit AccountsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void createAccount(const QString &connectionManager,
                       const QString &protocol, const QString &displayName,
                       const QVariantMap &parameters,
                       const QVariantMap &properties = QVariantMap());

protected:
    static Role getRealRole(const QModelIndex index, int role);

    QVariant getData(int index, Role role) const;

protected slots:
    void onAMReady(Tp::PendingOperation *operation);

protected:
    Tp::AccountManagerPtr m_manager;

};

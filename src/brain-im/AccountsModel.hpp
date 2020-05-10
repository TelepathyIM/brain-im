#ifndef BRAIN_IM_ACCOUNTS_MODEL
#define BRAIN_IM_ACCOUNTS_MODEL

#include "brain_im_global.h"

#include <QAbstractTableModel>

#include <TelepathyQt/Types>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/AccountManager>

class BRAIN_IM_EXPORT AccountsModel : public QAbstractTableModel
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
    enum class Column {
        Name,
        Enabled,
        Count,
        Invalid,
    };

    explicit AccountsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;

public slots:
    void createAccount(const QString &connectionManager,
                       const QString &protocol, const QString &displayName,
                       const QVariantMap &parameters,
                       const QVariantMap &properties = QVariantMap());

protected:
    static Role intToRole(int value);
    virtual Role indexToRole(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant getData(int index, Role role) const;
    bool setData(int index, Role role, const QVariant &value);

protected slots:
    void onAMReady(Tp::PendingOperation *operation);
    void onNewAccount(const Tp::AccountPtr &account);

protected:
    Tp::AccountManagerPtr m_manager;
    QList<Tp::AccountPtr> m_accounts;

};

#endif // BRAIN_IM_ACCOUNTS_MODEL

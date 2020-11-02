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
    enum class Role {
        AccountId,
        DisplayName,
        AccountEnabled,
        AccountValid,
        CmName,
        ProtocolName,
        ServiceName,
        AccountObject,
        RolesCount,
        Invalid
    };
    Q_ENUM(Role)
    enum class Column {
        UniqueIdentifier,
        DisplayName,
        Enabled,
        Valid,
        Invalid,
    };
    Q_ENUM(Column)

    explicit AccountsModel(QObject *parent = nullptr);

    void init(const Tp::AccountManagerPtr &accountManager = Tp::AccountManagerPtr());
    void invalidateFilter();

    QList<Tp::AccountPtr> accounts() const;

    QVector<Column> columns() const;
    void setColumns(const QVector<Column> &columns);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant getData(int index, Role role) const;
    bool setData(int index, Role role, const QVariant &value);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE Tp::AccountPtr getAccount(const QString &identifier) const;

public slots:
    void createAccount(const QString &connectionManager,
                       const QString &protocol, const QString &displayName,
                       const QVariantMap &parameters,
                       const QVariantMap &properties = QVariantMap());
    void removeAccount(const QString &accountId);
    void setAccountEnabled(const QString &accountId, bool enabled);

protected:
    void setAccountManager(const Tp::AccountManagerPtr &accountManager);
    void setAccounts(const QList<Tp::AccountPtr> &accounts);

    void setAccountEnabled(const Tp::AccountPtr &account, bool enabled);

    static Role intToRole(int value);
    static int roleToInt(Role role);
    static int roleToUserRole(Role role);
    virtual Role indexToRole(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void updateAccountData(const Tp::AccountPtr &account, Role role);

    virtual bool filterAcceptAccount(const Tp::AccountPtr &account) const;
    virtual void sortAccounts(QList<Tp::AccountPtr> *accounts) const;

protected slots:
    void onAMReady(Tp::PendingOperation *operation);
    void onNewAccount(const Tp::AccountPtr &account);

    void onAccountRemoved();
    void onAccountStateChanged();
    void onAccountValidityChanged();

    void trackAccount(const Tp::AccountPtr &account);
    void stopTrackingAccount(const Tp::AccountPtr &account);

    void onAccountOperationFinished(Tp::PendingOperation *operation);

protected:
    Tp::AccountManagerPtr m_manager;
    QList<Tp::AccountPtr> m_accounts;
    QVector<Column> m_columns;

};

#endif // BRAIN_IM_ACCOUNTS_MODEL

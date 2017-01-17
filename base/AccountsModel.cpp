#include "AccountsModel.hpp"

#include <TelepathyQt/Account>
#include <TelepathyQt/PendingAccount>

#include <QDebug>

static const int BaseRole = Qt::UserRole + 1;

AccountsModel::AccountsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_manager = Tp::AccountManager::create(Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                                      Tp::Account::FeatureCore));

    connect(m_manager->becomeReady(), &Tp::PendingOperation::finished, this, &AccountsModel::onAMReady);
}

int AccountsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    const QList<Tp::AccountPtr> accounts = m_manager->allAccounts();

    return accounts.count();
}

int AccountsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return RolesCount;
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << index << role;
    if (!index.isValid()) {
        return QVariant();
    }

    Role realRole = InvalidRole;

    if (role >= BaseRole) {
        realRole = static_cast<Role>(role - BaseRole);
    } else {
        switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            realRole = static_cast<Role>(index.column());
            break;
        default:
            return QVariant();
        }
    }

    return getData(index.row(), realRole);
}

QVariant AccountsModel::getData(int index, Role role) const
{
//    qDebug() << index << role;
    const QList<Tp::AccountPtr> accounts = m_manager->allAccounts();
    if (accounts.count() <= index) {
        return QVariant();
    }

    const Tp::AccountPtr account = accounts.at(index);

    switch (role) {
    case AccountObject:
        return QVariant::fromValue<QObject*>(account.data());
    case CmName:
        return account->cmName();
    case Enabled:
        return account->isEnabled();
    case DisplayName:
        return account->displayName();
    case ProtocolName:
        return account->protocolName();
    case ServiceName:
        return account->serviceName();
    case UniqueIdentifier:
//        return account->uniqueIdentifier();
        return account->objectPath();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> AccountsModel::roleNames() const
{
    static const QHash<int, QByteArray> extraRoles {
        { BaseRole + AccountObject,"accountObject" },
        { BaseRole + DisplayName,  "displayName" },
        { BaseRole + Enabled,      "enabled" },
        { BaseRole + CmName,       "cmName" },
        { BaseRole + ProtocolName, "protocolName" },
        { BaseRole + ServiceName,  "serviceName" },
        { BaseRole + UniqueIdentifier,  "uniqueIdentifier" },
    };
//    qDebug() << extraRoles;
    return extraRoles;
}

void AccountsModel::createAccount(const QString &connectionManager, const QString &protocol, const QString &displayName, const QVariantMap &parameters, const QVariantMap &properties)
{
    qDebug() << Q_FUNC_INFO << connectionManager << protocol;
    Tp::PendingAccount *account = m_manager->createAccount(connectionManager, protocol, displayName, parameters, properties);
    connect(account, &Tp::PendingOperation::finished, [account]() {
        qDebug() << account->errorName() << account->errorMessage();
    });
}

void AccountsModel::onAMReady(Tp::PendingOperation *operation)
{
    qDebug() << Q_FUNC_INFO;

    if (operation->isError()) {
        qWarning() << "ERROR!" << operation->errorName() << operation->errorMessage();
    } else {
        qDebug() << "Account manager is ready.";
    }
    beginResetModel();
    endResetModel();
}

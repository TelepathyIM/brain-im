#include "AccountsModel.hpp"

#include <TelepathyQt/Account>
#include <TelepathyQt/PendingAccount>

#include <QDebug>

static const int UserRoleOffset = Qt::UserRole + 1;

AccountsModel::AccountsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_manager = Tp::AccountManager::create(Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                                      Tp::Account::FeatureCore));

    connect(m_manager->becomeReady(), &Tp::PendingOperation::finished, this, &AccountsModel::onAMReady);
    connect(m_manager.data(), &Tp::AccountManager::newAccount, this, &AccountsModel::onNewAccount);
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
    return static_cast<int>(Column::Count);
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    return getData(index.row(), indexToRole(index, role));
}

bool AccountsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return setData(index.row(), indexToRole(index, role), value);
}

Qt::ItemFlags AccountsModel::flags(const QModelIndex &index) const
{
    // We have no role here, but it is OK because declarative viewes ignore it eitherway.
    // This method makes sense only for roles selected by column
    const Role realRole = indexToRole(index, Qt::DisplayRole);
    switch (realRole) {
    case Enabled:
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    default:
        break;
    }
    return QAbstractTableModel::flags(index);
}

QVariant AccountsModel::getData(int index, Role role) const
{
    if (role == InvalidRole) {
        return QVariant();
    }
    if (m_accounts.count() <= index) {
        return QVariant();
    }

    const Tp::AccountPtr account = m_accounts.at(index);

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
        //return account->uniqueIdentifier();
        return account->objectPath();
    default:
        return QVariant();
    }
}

bool AccountsModel::setData(int index, AccountsModel::Role role, const QVariant &value)
{
    if (role == InvalidRole) {
        return false;
    }
    qWarning() << Q_FUNC_INFO << index << role << value;

    const QList<Tp::AccountPtr> accounts = m_manager->allAccounts();
    if (accounts.count() <= index) {
        return false;
    }
    const Tp::AccountPtr account = accounts.at(index);

    switch (role) {
    case Enabled:
        if (account->isEnabled() == value.toBool()) {
            // Not changed
            return false;
        } else {
            Tp::PendingOperation *enablement = account->setEnabled(value.toBool());
            Q_UNUSED(enablement)
            return true;
        }
    default:
        break;
    }
    return false;
}

QHash<int, QByteArray> AccountsModel::roleNames() const
{
    static const QHash<int, QByteArray> extraRoles {
        { UserRoleOffset + AccountObject,
                    "accountObject" },
        { UserRoleOffset + DisplayName,
                    "displayName" },
        { UserRoleOffset + Enabled,
                    "enabled" },
        { UserRoleOffset + CmName,
                    "cmName" },
        { UserRoleOffset + ProtocolName,
                    "protocolName" },
        { UserRoleOffset + ServiceName,
                    "serviceName" },
        { UserRoleOffset + UniqueIdentifier,
                    "uniqueIdentifier" },
    };
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

AccountsModel::Role AccountsModel::intToRole(int value)
{
    if (value < 0 || value > static_cast<int>(Role::RolesCount)) {
        return Role::InvalidRole;
    }
    return static_cast<Role>(value);
}

AccountsModel::Role AccountsModel::indexToRole(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    if (role >= UserRoleOffset) {
        return intToRole(role - UserRoleOffset);
    }

    Column column = static_cast<Column>(index.column());
    switch (column) {
    case Column::Name:
        return Role::DisplayName;
    case Column::Enabled:
        return Role::Enabled;
    case Column::Count:
    case Column::Invalid:
        // Invalid call
        return Role::InvalidRole;
    }

//    return Role::InvalidRole;

//    Role realRole = InvalidRole;

//    if (role >= BaseRole) {
//        realRole = static_cast<Role>(role - BaseRole);
//    } else {
//        switch (role) {
//        case Qt::DisplayRole:
//        case Qt::EditRole:
//            realRole = static_cast<Role>(index.column());
//            break;
//        default:
//            break;
//        }
//    }
//    return realRole;
    Q_UNREACHABLE();
    return Role::InvalidRole;
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
    m_accounts = m_manager->allAccounts();
    endResetModel();
}

void AccountsModel::onNewAccount(const Tp::AccountPtr &account)
{
    // TODO: beginInsertRows()
    beginResetModel();
    m_accounts = m_manager->allAccounts();
    endResetModel();
}

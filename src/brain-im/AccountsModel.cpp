#include "AccountsModel.hpp"

#include <TelepathyQt/Account>
#include <TelepathyQt/PendingAccount>

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcAccountsModel, "app.accountsModel", QtDebugMsg)

static const int UserRoleOffset = Qt::UserRole + 1;

/**
 * \class AccountsModel
 *
 * Default implementation currently provides read-only access to the accounts.
 *
 * Usage:
 * -# Call setColumns() to customize the wanted columns. (Only account ID column exposed by default)
 * -# Call init(). Pass Tp::AccountManager to reuse an existing manager or give no arguments so
 *    the model construct its own AccountManager.
 *
 * Subclassing:
 * -# Override sortAccounts() to customize the accounts order.
 * -# Override filterAcceptAccount() to filter out certain accounts.
 */

AccountsModel::AccountsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_columns = {
        Column::UniqueIdentifier,
    };
}

void AccountsModel::init(const Tp::AccountManagerPtr &accountManager)
{
    if (accountManager) {
        setAccountManager(accountManager);
    } else {
        const Tp::Features accountFeatures = Tp::Account::FeatureCore | Tp::Account::FeatureProtocolInfo;
        Tp::AccountFactoryPtr accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                                          accountFeatures);
        setAccountManager(Tp::AccountManager::create(accountFactory));
    }
}

void AccountsModel::invalidateFilter()
{
    if (!m_manager) {
        return;
    }
    setAccounts(m_manager->allAccounts());
}

QList<Tp::AccountPtr> AccountsModel::accounts() const
{
    return m_accounts;
}

void AccountsModel::setAccounts(const QList<Tp::AccountPtr> &accounts)
{
    QList<Tp::AccountPtr> newAccounts;
    for (const Tp::AccountPtr &account : accounts) {
        if (!filterAcceptAccount(account)) {
            continue;
        }
        newAccounts << account;
    }
    sortAccounts(&newAccounts);

    if (m_accounts == newAccounts) {
        return;
    }

    for (const Tp::AccountPtr &trackedAccount : m_accounts) {
        if (!newAccounts.contains(trackedAccount)) {
            stopTrackingAccount(trackedAccount);
        }
    }
    for (const Tp::AccountPtr &newAccount : newAccounts) {
        if (!m_accounts.contains(newAccount)) {
            trackAccount(newAccount);
        }
    }

    if (m_accounts.isEmpty()) {
        // Fast path to add all accounts
        beginInsertRows(QModelIndex(), 0, newAccounts.count() - 1);
        m_accounts = newAccounts;
        endInsertRows();
        return;
    } else if (newAccounts.isEmpty()) {
        // Fast path to remove all accounts
        beginRemoveRows(QModelIndex(), 0, m_accounts.count() - 1);
        m_accounts.clear();
        endRemoveRows();
        return;
    }

    for (int accountIndex = m_accounts.count() - 1; accountIndex > 0; --accountIndex) {
        const Tp::AccountPtr &account = m_accounts.at(accountIndex);
        if (!newAccounts.contains(account)) {
            beginRemoveRows(QModelIndex(), accountIndex, accountIndex);
            m_accounts.removeAt(accountIndex);
            endRemoveRows();
        }
    }

    for (int accountIndex = 0; accountIndex < newAccounts.count(); ++accountIndex) {
        const Tp::AccountPtr &account = newAccounts.at(accountIndex);
        int currentIndex = m_accounts.indexOf(account);
        if (currentIndex != accountIndex) {
            if (currentIndex < 0) {
                beginInsertRows(QModelIndex(), accountIndex, accountIndex);
                m_accounts.insert(accountIndex, account);
                endInsertRows();
            } else {
                beginMoveRows(QModelIndex(), currentIndex, currentIndex,
                              QModelIndex(), accountIndex);
                m_accounts.move(currentIndex, accountIndex);
                endMoveRows();
            }
        }
    }
}

QVector<AccountsModel::Column> AccountsModel::columns() const
{
    return m_columns;
}

void AccountsModel::setColumns(const QVector<AccountsModel::Column> &columns)
{
    m_columns = columns;
}

int AccountsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_accounts.count();
}

int AccountsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_columns.count();
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    return getData(index.row(), indexToRole(index, role));
}

QVariant AccountsModel::getData(int index, Role role) const
{
    if (role == Role::Invalid) {
        return QVariant();
    }
    if (m_accounts.count() <= index) {
        return QVariant();
    }

    const Tp::AccountPtr account = m_accounts.at(index);

    switch (role) {
    case Role::AccountObject:
        return QVariant::fromValue<QObject*>(account.data());
    case Role::CmName:
        return account->cmName();
    case Role::AccountEnabled:
        return account->isEnabled();
    case Role::AccountValid:
        return account->isValidAccount();
    case Role::DisplayName:
        return account->displayName();
    case Role::ProtocolName:
        return account->protocolName();
    case Role::ServiceName:
        return account->serviceName();
    case Role::AccountId:
        return account->uniqueIdentifier();
    case Role::RolesCount:
    case Role::Invalid:
        return QVariant();
    }

    return QVariant();
}

QHash<int, QByteArray> AccountsModel::roleNames() const
{
    static const QHash<int, QByteArray> extraRoles {
        { roleToUserRole(Role::AccountId), "accountId" },
        { roleToUserRole(Role::DisplayName), "displayName" },
        { roleToUserRole(Role::AccountEnabled), "accountEnabled" },
        { roleToUserRole(Role::AccountValid), "accountValid" },
        { roleToUserRole(Role::CmName), "managerName" },
        { roleToUserRole(Role::ProtocolName), "protocolName" },
        { roleToUserRole(Role::ServiceName), "serviceName" },
        { roleToUserRole(Role::AccountObject), "accountObject" },
    };
    return extraRoles;
}

Tp::AccountPtr AccountsModel::getAccount(const QString &identifier) const
{
    for (const Tp::AccountPtr &account : m_accounts) {
        if (account->uniqueIdentifier() == identifier) {
            return account;
        }
    }

    return Tp::AccountPtr();
}

void AccountsModel::createAccount(const QString &connectionManager,
                                  const QString &protocol,
                                  const QString &displayName,
                                  const QVariantMap &parameters,
                                  const QVariantMap &properties)
{
    qDebug() << Q_FUNC_INFO << connectionManager << protocol;
    Tp::PendingAccount *account = m_manager->createAccount(connectionManager, protocol,
                                                           displayName, parameters, properties);
    connect(account, &Tp::PendingOperation::finished, [account]() {
        qDebug() << account->errorName() << account->errorMessage();
    });
}

void AccountsModel::setAccountEnabled(const QString &accountId, bool enabled)
{
    Tp::AccountPtr account = getAccount(accountId);
    if (!account) {
        return;
    }
    setAccountEnabled(account, enabled);
}

void AccountsModel::setAccountManager(const Tp::AccountManagerPtr &accountManager)
{
    m_manager = accountManager;
    connect(m_manager->becomeReady(), &Tp::PendingOperation::finished, this, &AccountsModel::onAMReady);
    connect(m_manager.data(), &Tp::AccountManager::newAccount, this, &AccountsModel::onNewAccount);
}

AccountsModel::Role AccountsModel::intToRole(int value)
{
    if (value < 0 || value > static_cast<int>(Role::RolesCount)) {
        return Role::Invalid;
    }
    return static_cast<Role>(value);
}

int AccountsModel::roleToInt(AccountsModel::Role role)
{
    return static_cast<int>(role);
}

int AccountsModel::roleToUserRole(AccountsModel::Role role)
{
    return roleToInt(role) + UserRoleOffset;
}

AccountsModel::Role AccountsModel::indexToRole(const QModelIndex &index, int role) const
{
    if (role >= UserRoleOffset) {
        return intToRole(role - UserRoleOffset);
    }

    if (index.column() >= m_columns.count()) {
        return Role::Invalid;
    }

    Column column = m_columns.at(index.column());

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        break;
    default:
        return Role::Invalid;
    }

    switch (column) {
    case Column::UniqueIdentifier:
        return Role::AccountId;
    case Column::DisplayName:
        return Role::DisplayName;
    case Column::Enabled:
        return Role::AccountEnabled;
    case Column::Valid:
        return Role::AccountValid;
    case Column::Invalid:
        // Invalid call
        return Role::Invalid;
    }

    Q_UNREACHABLE();
    return Role::Invalid;
}

void AccountsModel::updateAccountData(const Tp::AccountPtr &account, AccountsModel::Role role)
{
    const int row = m_accounts.indexOf(account);

    const QModelIndex left = index(row, 0);
    const QModelIndex right = index(row, columnCount() - 1);
    emit dataChanged(left, right, { Qt::DisplayRole, roleToUserRole(role) } );
}

bool AccountsModel::filterAcceptAccount(const Tp::AccountPtr &) const
{
    // Base model accepts all accounts
    return true;
}

void AccountsModel::sortAccounts(QList<Tp::AccountPtr> *accounts) const
{
    const auto comparator = [](const Tp::AccountPtr &left, const Tp::AccountPtr &right) {
        return left->displayName().compare(right->displayName(), Qt::CaseInsensitive) < 0;
    };
    std::sort(accounts->begin(), accounts->end(), comparator);
}

void AccountsModel::onAMReady(Tp::PendingOperation *operation)
{
    qDebug() << Q_FUNC_INFO;

    if (operation->isError()) {
        qWarning() << "ERROR!" << operation->errorName() << operation->errorMessage();
    } else {
        qDebug() << "Account manager is ready.";
    }

    setAccounts(m_manager->allAccounts());
}

void AccountsModel::onNewAccount(const Tp::AccountPtr &account)
{
    Q_UNUSED(account)
    setAccounts(m_manager->allAccounts());
}

void AccountsModel::onAccountRemoved()
{
    Tp::Account *account = qobject_cast<Tp::Account *>(sender());
    if (!account) {
        qCWarning(lcAccountsModel) << __func__ << "Invalid call";
        return;
    }

    int accountIndex = m_accounts.indexOf(Tp::AccountPtr(account));
    if (accountIndex < 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), accountIndex, accountIndex);
    m_accounts.removeAt(accountIndex);
    endRemoveRows();
}

void AccountsModel::onAccountStateChanged()
{
    Tp::Account *account = qobject_cast<Tp::Account *>(sender());
    if (!account) {
        qCWarning(lcAccountsModel) << __func__ << "Invalid call";
        return;
    }

    qCDebug(lcAccountsModel) << __func__ << account->uniqueIdentifier()
                             << "enabled:" << account->isEnabled();

    updateAccountData(Tp::AccountPtr(account), Role::AccountEnabled);
}

void AccountsModel::onAccountValidityChanged()
{
    Tp::Account *account = qobject_cast<Tp::Account *>(sender());
    if (!account) {
        qCWarning(lcAccountsModel) << __func__ << "Invalid call";
        return;
    }

    qCDebug(lcAccountsModel) << __func__ << account->uniqueIdentifier()
                             << "valid:" << account->isValidAccount();

    updateAccountData(Tp::AccountPtr(account), Role::AccountValid);
}

void AccountsModel::trackAccount(const Tp::AccountPtr &account)
{
    connect(account.data(), &Tp::Account::removed,
            this, &AccountsModel::onAccountRemoved);
    connect(account.data(), &Tp::Account::stateChanged,
            this, &AccountsModel::onAccountStateChanged);
    connect(account.data(), &Tp::Account::validityChanged,
            this, &AccountsModel::onAccountValidityChanged);
}

void AccountsModel::stopTrackingAccount(const Tp::AccountPtr &account)
{
    disconnect(account.data(), &Tp::Account::removed,
               this, &AccountsModel::onAccountRemoved);
    disconnect(account.data(), &Tp::Account::stateChanged,
               this, &AccountsModel::onAccountStateChanged);
    disconnect(account.data(), &Tp::Account::validityChanged,
               this, &AccountsModel::onAccountValidityChanged);
}

#include "AccountParameterModel.hpp"

#include <TelepathyQt/PendingStringList>

static const int BaseRole = Qt::UserRole + 1;

AccountParameterModel::AccountParameterModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_resetInProgress(false),
      m_creationMode(false)
{
}

QHash<int, QByteArray> AccountParameterModel::roleNames() const
{
    static const QHash<int, QByteArray> extraRoles {
        { BaseRole + Name,                    "name" },
        { BaseRole + Value,                   "value" },
        { BaseRole + Signature,               "signature" },
        { BaseRole + Type,                    "type" },
        { BaseRole + DefaultValue,            "defaultValue" },
        { BaseRole + Required,                "required" },
        { BaseRole + Secret,                  "secret" },
        { BaseRole + RequiredForRegistration, "registration" }
    };

    return extraRoles;
}

int AccountParameterModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || m_protocol.isEmpty() || m_manager.isEmpty()) {
        // || m_managerObject.isNull() || !m_managerObject->supportedProtocols().contains(m_protocol)) {
        return 0;
    }
    return m_protocolParams.count();
}

int AccountParameterModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return RolesCount;
}

QVariantMap AccountParameterModel::getVariantMap() const
{
    qDebug() << Q_FUNC_INFO << m_values.keys();
    qDebug() << m_values;
    return m_values;
}

QVariant AccountParameterModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << index << role;
    if (!index.isValid()) {
        return QVariant();
    }

    Role realRole = getRealRole(index, role);
    if (realRole == InvalidRole) {
        return QVariant();
    }

    return getData(index.row(), realRole);
}

QVariant AccountParameterModel::getData(int index, Role role) const
{
    if ((index < 0) || index >= m_protocolParams.count()) {
        qWarning() << Q_FUNC_INFO << "Index is out of range";
        return QVariant();
    }

    switch (role) {
    case Name:
        return m_protocolParams.at(index).name();
    case Value:
        return m_values.value(m_protocolParams.at(index).name());
    case Signature:
        return m_protocolParams.at(index).dbusSignature().signature();
    case Type:
        return static_cast<int>(m_protocolParams.at(index).type());
    case DefaultValue:
        return m_protocolParams.at(index).defaultValue();
    case Required:
        return m_protocolParams.at(index).isRequired();
    case Secret:
        return m_protocolParams.at(index).isSecret();
    default:
        break;
    }

    return QVariant();
}

bool AccountParameterModel::setData(const QString &paramName, const QVariant &value)
{
    for (int i = 0; i < m_protocolParams.count(); ++i) {
        if (m_protocolParams.at(i).name() == paramName) {
            return setData(i, value);
        }
    }

    return false;
}

bool AccountParameterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    return false;
}

bool AccountParameterModel::setData(int index, const QVariant &value)
{
    if (index < 0 || index >= m_protocolParams.count()) {
        qDebug() << Q_FUNC_INFO << "The data index is out of range";
        return false;
    }

    const QVariant tVar = getData(index, Type);
    const int type = tVar.toInt();
    if (!value.canConvert(type)) {
        qWarning() << Q_FUNC_INFO << "Can't convert the value to the right type" << type;
        return false;
    }
    QVariant typed = value;
    if (!typed.convert(type)) {
        qWarning() << Q_FUNC_INFO << "Unable to cast the value to the right type" << type;
        return false;
    }

    m_values.insert(getData(index, Name).toString(), typed);

    return false;
}

AccountParameterModel::Role AccountParameterModel::getRealRole(const QModelIndex index, int role)
{
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
            break;
        }
    }

    return realRole;
}

void AccountParameterModel::newAccount(const QString &manager, const QString &protocol)
{
    beginResetModel();
    m_resetInProgress = true;
    m_values.clear();
    m_account.reset();
    m_managerObject = Tp::ConnectionManager::create(QDBusConnection::sessionBus(), manager);
    connect(m_managerObject->becomeReady(), SIGNAL(finished(Tp::PendingOperation*)), this, SLOT(onReady(Tp::PendingOperation*)));
    setCreationMode(true);
    setManager(manager);
    setProtocol(protocol);
}

void AccountParameterModel::setAccount(const QString &uniqueIdentifier)
{
    beginResetModel();
    m_resetInProgress = true;
    m_values.clear();
    m_managerObject.reset();
    m_account = Tp::Account::create(TP_QT_ACCOUNT_MANAGER_BUS_NAME, uniqueIdentifier);
    connect(m_account->becomeReady(Tp::Account::FeatureCore|Tp::Account::FeatureProtocolInfo|Tp::Account::FeatureCapabilities|Tp::Account::FeatureProfile), SIGNAL(finished(Tp::PendingOperation*)), this, SLOT(onReady(Tp::PendingOperation*)));
    setCreationMode(false);
}

bool AccountParameterModel::submit()
{
    if (m_account.isNull()) {
        return false;
    }

    qDebug() << "Submit:" << getVariantMap();
    Tp::PendingStringList *submitOperation = m_account->updateParameters(m_values, QStringList());
    connect(submitOperation, &Tp::PendingStringList::finished, [=](Tp::PendingOperation *operation) {
        if (operation->isError()) {
            qWarning() << "AccountParameterModel::submit(): Operation failed:" << operation->errorName() << operation->errorMessage();
        } else {
            qDebug() << "AccountParameterModel::submit(): Operation succeeded";
        }
    });
    return true;
}

void AccountParameterModel::onReady(Tp::PendingOperation *operation)
{
    qDebug() << Q_FUNC_INFO << operation->isValid();
    if (operation->isValid()) {
        reinit();
    }

    m_resetInProgress = false;
    endResetModel();
    qDebug() << Q_FUNC_INFO << "params:" << m_protocolParams.count() << "rows:" << rowCount();
}

void AccountParameterModel::setManager(const QString &manager)
{
    if (m_manager == manager) {
        return;
    }

    m_manager = manager;
    emit managerChanged(manager);
}

void AccountParameterModel::setProtocol(const QString &protocol)
{
    if (m_protocol == protocol) {
        return;
    }

    m_protocol = protocol;
    emit protocolChanged(protocol);
}

void AccountParameterModel::setProtocolParams(const Tp::ProtocolParameterList &params)
{
    m_protocolParams = params;
}

void AccountParameterModel::setDisplayName(const QString &displayName)
{
    if (m_displayName == displayName) {
        return;
    }

    m_displayName = displayName;
    emit displayNameChanged(displayName);
}

void AccountParameterModel::setCreationMode(bool creationMode)
{
    if (m_creationMode == creationMode) {
        return;
    }

    m_creationMode = creationMode;
    emit creationModeChanged(creationMode);
}

void AccountParameterModel::reinit()
{
    if (!m_managerObject.isNull() && m_managerObject->isReady()) {
        const Tp::ProtocolInfo protocolInfo = m_managerObject->protocol(m_protocol);
        if (!protocolInfo.isValid()) {
            qDebug() << Q_FUNC_INFO << "Protocol info is not valid";
        }
        setProtocolParams(protocolInfo.parameters());
    } else if (!m_account.isNull() && m_account->isReady()) {
        setProtocolParams(m_account->protocolInfo().parameters());
        m_values = m_account->parameters();
        setManager(m_account->cmName());
        setProtocol(m_account->protocolName());
        setDisplayName(m_account->displayName());
    }
}

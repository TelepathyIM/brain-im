#include "ProtocolsModel.hpp"

static const int BaseRole = Qt::UserRole + 1;

//static DBusServiceHelper *helper = nullptr;

ProtocolsModel::ProtocolsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
//    Tp::ConnectionManager
}

QHash<int, QByteArray> ProtocolsModel::roleNames() const
{
    static const QHash<int, QByteArray> extraRoles {
        { BaseRole + Name, "name" },
        { BaseRole + CmName, "cmName" },
    };
    qDebug() << extraRoles;
    return extraRoles;
}

int ProtocolsModel::rowCount(const QModelIndex &parent) const
{
//    if (!helper->isInitialized()) {
//        return 0;
//    }

    if (parent.isValid()) {
        return 0;
    }

//    qDebug() << Q_FUNC_INFO << m_protocols.count();
    return m_protocols.count();
}

int ProtocolsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return RolesCount;
}

QVariant ProtocolsModel::data(const QModelIndex &index, int role) const
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

QVariant ProtocolsModel::getData(int index, ProtocolsModel::Role role) const
{
    if (index < 0) {
        return QVariant();
    }
    if (m_protocols.count() <= index) {
        return QVariant();
    }

    switch (role) {
    case Name:
        return m_protocols.at(index).name();
    case CmName:
        return m_protocols.at(index).cmName();
    default:
        break;
    }

    return QVariant();
}

void ProtocolsModel::onReady()
{
    beginResetModel();
    m_protocols.clear();
    foreach (const Tp::ConnectionManagerPtr &manager, m_managers2) {
        const Tp::ProtocolInfoList protocols = manager->protocols();
        m_protocols.append(protocols);
    }
    endResetModel();
}

void ProtocolsModel::setManagers(const QStringList &managers)
{
    if (m_managers == managers) {
        return;
    }
    m_managers = managers;
    emit managersChanged(managers);

    beginResetModel();
    foreach (const QString &m, managers) {
        Tp::ConnectionManagerPtr manager = Tp::ConnectionManager::create(QDBusConnection::sessionBus(), m);
        connect(manager->becomeReady(), SIGNAL(finished(Tp::PendingOperation*)), this, SLOT(onReady()));
        m_managers2.append(manager);

//        const Tp::ProtocolInfoList protocols = manager->protocols();
//        m_protocols.append(protocols);
    }
    endResetModel();
}

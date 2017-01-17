#include "ConnectionManagersModel.hpp"

#include "DBusServiceHelper"

#include <TelepathyQt/ConnectionManager>

#include <QDebug>

static const int BaseRole = Qt::UserRole + 1;

static DBusServiceHelper *helper = nullptr;

ConnectionManagersModel::ConnectionManagersModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    helper = new DBusServiceHelper(this);

    connect(helper, SIGNAL(initialized()), this, SLOT(onReady()));
}

QHash<int, QByteArray> ConnectionManagersModel::roleNames() const
{
    static const QHash<int, QByteArray> extraRoles {
        { BaseRole + Name,"name" },
    };
    qDebug() << extraRoles;
    return extraRoles;
}

int ConnectionManagersModel::rowCount(const QModelIndex &parent) const
{
    if (!helper->isInitialized()) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }

//    qDebug() << Q_FUNC_INFO << m_managers.count();
    return m_managers.count();
}

int ConnectionManagersModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return RolesCount;
}

QVariant ConnectionManagersModel::data(const QModelIndex &index, int role) const
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

QVariant ConnectionManagersModel::getData(int index, ConnectionManagersModel::Role role) const
{
    if (index < 0) {
        return QVariant();
    }
    if (m_managers.count() <= index) {
        return QVariant();
    }

    switch (role) {
    case Name:
        return m_managers.at(index);
    default:
        break;
    }

    return QVariant();
}

void ConnectionManagersModel::onReady()
{
    qDebug() << Q_FUNC_INFO;
    beginResetModel();
    for (const QString &name: helper->listNames()) {
        if (name.startsWith(TP_QT_CONNECTION_MANAGER_BUS_NAME_BASE)) {
            const QString shortName = name.mid(strlen(TP_QT_CONNECTION_MANAGER_BUS_NAME_BASE.data()));
            m_managers.append(shortName);
        }
    }

    for (const QString &name: helper->listActivatableNames()) {
        if (name.startsWith(TP_QT_CONNECTION_MANAGER_BUS_NAME_BASE)) {
            const QString shortName = name.mid(strlen(TP_QT_CONNECTION_MANAGER_BUS_NAME_BASE.data()));
            if (!m_managers.contains(shortName)) {
                m_managers.append(shortName);
            }
        }
    }
    endResetModel();
    emit managersChanged(m_managers);
}

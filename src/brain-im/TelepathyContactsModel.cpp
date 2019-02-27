#include "ContactsModel.hpp"

#ifdef USE_TELEPATHY
#include "Core.hpp"

#include <TelepathyQt/AccountManager>
#include <TelepathyQt/ContactManager>
#include <TelepathyQt/PendingReady>
#endif // USE_TELEPATHY

#include <QDateTime>

#include <QDebug>

namespace BrainIM {

static const int UserRoleOffset = Qt::UserRole + 1;

ContactsModel::ContactsModel(QObject *parent) :
    PeersModel(parent)
{
    Tp::AccountManagerPtr manager = accountManager();
    connect(manager->becomeReady(), &Tp::PendingOperation::finished, this, &ContactsModel::onAMReady);
}

QHash<int, QByteArray> ContactsModel::roleNames() const
{
    static const QHash<int, QByteArray> extraRoles {
        { UserRoleOffset + static_cast<int>(Role::Identifier), "identifier" },
        { UserRoleOffset + static_cast<int>(Role::Peer), "peer" },
        { UserRoleOffset + static_cast<int>(Role::Alias), "alias" },
    };

    return extraRoles;
}

bool ContactsModel::hasPeer(const Peer peer) const
{
    if (peer.type != Peer::Type::Contact) {
        return false;
    }
    return indexOfContact(peer.id) >= 0;
}

QString ContactsModel::getName(const Peer peer) const
{
    if (peer.type != Peer::Type::Contact) {
        return QString();
    }
    const int i = indexOfContact(peer.id);
    if (i < 0) {
        return QString();
    }
    return getData(i, Role::Alias).toString();
}

int ContactsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    if (!m_contacts2.isEmpty()) {
        return  m_contacts2.count();
    }
    return m_contacts.count();
}

QVariant ContactsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    const Column column = intToColumn(section);

    switch (column) {
    case Column::Id:
        return tr("Id");
    case Column::Phone:
        return tr("Phone");
    case Column::UserName:
        return tr("Username");
    case Column::FullName:
        return tr("Full name");
    case Column::Status:
        return tr("Status");
    case Column::TypingStatus:
        return tr("Typing status");
    case Column::Avatar:
        return tr("Avatar");
    case Column::Blocked:
        return tr("Blocked");
    default:
        break;
    }

    return QVariant();
}

QVariant ContactsModel::data(const QModelIndex &index, int role) const
{
    const Role r = indexToRole(index, role);
    return getData(index.row(), r);
}

QVariant ContactsModel::getData(int index, Role role) const
{
    if (index < 0) {
        return QVariant();
    }

    if (!m_contacts2.isEmpty()) {
        Contact contact = m_contacts2.at(index);

        switch (role) {
        case Role::Identifier:
            return contact.id();
        case Role::Alias:
            return contact.alias();
        case Role::Peer:
            return QVariant::fromValue(Peer(contact.id(), Peer::Type::Contact));
        default:
            return QVariant();
        }
    }
    Tp::ContactPtr contact = m_contacts.at(index);

    switch (role) {
    case Role::Identifier:
        return contact->id();
    case Role::Alias:
        return contact->alias();
    case Role::Peer:
        return QVariant::fromValue(Peer(contact->id(), Peer::Type::Contact));
    //    return m_contacts.at(contactIndex).id();
    //case Phone:
    //    return m_contacts.at(contactIndex).phone();
    //case UserName:
    //    return m_contacts.at(contactIndex).userName();
    //case FullName:
    //    return m_contacts.at(contactIndex).firstName() + QLatin1Char(' ') + m_contacts.at(contactIndex).lastName();
    //case Status:
    //    return contactStatusStr(m_contacts.at(contactIndex));
    //case TypingStatus:
    //    return m_contacts.at(contactIndex).typing;
    //case Blocked:
    //    return m_contacts.at(contactIndex).blocked;
    //case Avatar:
    //    return m_contacts.at(contactIndex).m_picture.pixmap;
    default:
        return QVariant();
    }
}

void ContactsModel::addContactId(quint32 id)
{
    //const Peer peer(id, Peer::Contact);
    //m_contacts.append(SContact());
    //m_backend->getUserInfo(&m_contacts.last(), id);
    //m_contacts.last().m_picture = getPeerPictureNowOrLater(peer);
    //qDebug() << Q_FUNC_INFO << peer.id << m_contacts.last().m_picture.token;
}

void ContactsModel::addContact(quint32 id)
{
    if (hasContact(id)) {
        return;
    }
    qDebug() << Q_FUNC_INFO << id;

    beginInsertRows(QModelIndex(), m_contacts.count(), m_contacts.count());
    addContactId(id);
    endInsertRows();
}

bool ContactsModel::removeContact(quint32 id)
{
//    int contactIndex = indexOfContact(id);

//    if (contactIndex < 0) {
//        return false;
//    }

//    beginRemoveRows(QModelIndex(), contactIndex, contactIndex);
//    m_contacts.removeAt(contactIndex);
//    endRemoveRows();

    return true;
}

void ContactsModel::setContactList(const QVector<quint32> &newContactList)
{
    beginResetModel();
    m_contacts.clear();
    for (quint32 userId : newContactList) {
        addContactId(userId);
    }
    endResetModel();
}

void ContactsModel::setTypingStatus(Peer id, MessageAction action)
{
//    int index = indexOfContact(id);

//    if (index < 0) {
//        return;
//    }

//    m_contacts[index].typing = action;

//    QModelIndex modelIndex = createIndex(index, TypingStatus);
//    emit dataChanged(modelIndex, modelIndex);
}

void ContactsModel::clear()
{
    beginResetModel();
    m_contacts.clear();
    endResetModel();
}

void ContactsModel::populate()
{
    beginResetModel();
    m_contacts.clear();
    {
        Contact c(QStringLiteral("daniel"));
        c.setId(QStringLiteral("id1"));
        m_contacts2.append(c);
    }
    endResetModel();
}

void ContactsModel::onContactProfileChanged(quint32 id)
{
//    int index = indexOfContact(id);
//    if (index < 0) {
//        return;
//    }

//    m_backend->getUserInfo(&m_contacts[index], id);
//    QModelIndex modelIndexFirst = createIndex(index, UserName);
//    QModelIndex modelIndexLast = createIndex(index, FullName);
//    emit dataChanged(modelIndexFirst, modelIndexLast);
//    emit nameChanged(Telegram::Peer(id, Peer::Contact));
}

void ContactsModel::onContactStatusChanged(quint32 id)
{
//    int index = indexOfContact(id);
//    if (index < 0) {
//        return;
//    }

//    m_backend->getUserInfo(&m_contacts[index], id);
//    QModelIndex modelIndex = createIndex(index, Status);
    //    emit dataChanged(modelIndex, modelIndex);
}

void ContactsModel::onAMReady()
{
    Tp::AccountManagerPtr manager = accountManager();
    if (!manager) {
        qWarning() << Q_FUNC_INFO << "no account manager";
        return;
    }
    QList<Tp::AccountPtr> accounts = manager->allAccounts();
    if (accounts.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "no accounts";
        return;
    }
    const int onlineIndex = [](const QList<Tp::AccountPtr> &accounts) {
        for (int i = 0; i < accounts.count(); ++i) {
            if (accounts.at(i).data()->isOnline()) {
                return i;
            }
        }
        return -1;
    } (accounts);
    if (onlineIndex < 0) {
        qWarning() << Q_FUNC_INFO << "no online account";
        return;
    }

    Tp::AccountPtr acc = accounts.at(onlineIndex);
    Tp::ConnectionPtr conn = acc->connection();
    if (!conn) {
        qWarning() << Q_FUNC_INFO << "no account connection";
        QTimer::singleShot(200, this, &ContactsModel::onAMReady);
        return;
    }
    Tp::ContactManagerPtr connContacts = conn->contactManager();
    if (!connContacts) {
        qWarning() << Q_FUNC_INFO << "no account contact manager";
        return;
    }

    beginResetModel();
    const auto contacts = connContacts->allKnownContacts();
    m_contacts.reserve(contacts.count());
    for (const auto c : contacts) {
        m_contacts.append(c);
    }
    endResetModel();
}

ContactsModel::Role ContactsModel::intToRole(int value)
{
    if (value < 0 || value > static_cast<int>(Role::Count)) {
        return Role::Invalid;
    }
    return static_cast<Role>(value);
}

ContactsModel::Column ContactsModel::intToColumn(int value)
{
    if (value < 0 || value > static_cast<int>(Column::Count)) {
        return Column::Invalid;
    }
    return static_cast<Column>(value);
}

ContactsModel::Role ContactsModel::indexToRole(const QModelIndex &index, int role)
{
    if (role >= UserRoleOffset) {
        return intToRole(role - UserRoleOffset);
    }

    const Column section = intToColumn(index.column());

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (section) {
        case Column::Id:
            return Role::Identifier;
        case Column::FullName:
            return Role::Alias;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return Role::Invalid;
}

//void ContactsModel::updatePeerPicture(const PeerPicture &picture)
//{
//    for (int i = 0; i < m_contacts.count(); ++i) {
//        if (m_contacts.at(i).m_picture.token != picture.token) {
//            continue;
//        }
//        m_contacts[i].m_picture = picture;
//        const QModelIndex modelIndex = createIndex(i, Avatar);
//        emit dataChanged(modelIndex, modelIndex);
//        emit pictureChanged(Telegram::Peer(m_contacts.at(i).id(), Peer::Contact));
//    }
//}

int ContactsModel::indexOfContact(const QString &id) const
{
//    for (int i = 0; i < m_contacts.count(); ++i) {
//        if (m_contacts.at(i).id() == id) {
//            return i;
//        }
//    }

    return -1;
}

int ContactsModel::indexOfContact(const Peer phone) const
{
//    for (int i = 0; i < m_contacts.count(); ++i) {
//        if (m_contacts.at(i).phone() == phone) {
//            return i;
//        }
//    }

    return -1;
}

bool ContactsModel::hasContact(quint32 contactId) const
{
    return false;
}

Tp::ContactPtr ContactsModel::contactAt(int index) const
{
    if ((index < 0) || (index >= m_contacts.count())) {
        return Tp::ContactPtr();
    }

    qWarning() << Q_FUNC_INFO << "return" << index << m_contacts.at(index).data() << m_contacts.at(index).data()->alias();
    return m_contacts.at(index);
}

//const SContact *ContactsModel::getContact(quint32 id) const
//{
//    int index = indexOfContact(id);
//    if (index < 0) {
//        return nullptr;
//    }

//    return contactAt(index);
//}

QVector<Tp::ContactPtr> ContactsModel::contacts() const
{
    return m_contacts;
}

QString ContactsModel::contactAt(int index, bool addName) const
{
//    if ((index < 0) || (index >= m_contacts.count())) {
        return QString();
//    }

//    if (addName) {
//        return m_contacts.at(index).phone() + QLatin1Char(' ') + data(index, FullName).toString();
//    } else {
//        return m_contacts.at(index).phone();
        //    }
}

int ContactsModel::indexOfPeer(const Peer peer) const
{
    if (peer.type != Peer::Type::Contact) {
        return -1;
    }
    for (int i = 0; i < m_contacts.count(); ++i) {
        if (m_contacts.at(i)->id() == peer.id) {
            return i;
        }
    }
    return -1;
}

//QString ContactsModel::formatName(const Telegram::UserInfo &info)
//{
//    QString name = info.firstName() + QLatin1Char(' ') + info.lastName();
//    name = name.simplified();
//    if (!name.isEmpty()) {
//        return name;
//    }
//    return info.userName();
//}

//QString ContactsModel::getContactName(const Telegram::UserInfo &contact)
//{
//    if (!contact.userName().isEmpty()) {
//        return contact.userName();
//    }

//    QString name = contact.firstName() + QLatin1Char(' ') + contact.lastName();
//    if (!name.simplified().isEmpty()) {
//        return name;
//    }

//    return tr("Unknown name");
//}

//QString ContactsModel::getContactIdentifier(const Telegram::UserInfo &contact)
//{
//    if (!contact.phone().isEmpty()) {
//        return contact.phone();
//    }

//    return QStringLiteral("user") + QString::number(contact.id());
//}

QString ContactsModel::contactStatusStr(const SContact &contact) const
{
//    switch (contact.status()) {
//    case TelegramNamespace::ContactStatusOnline:
//        return tr("Online");
//    case TelegramNamespace::ContactStatusOffline:
//        if (contact.wasOnline() > 0) {
//            const QDateTime wasOnline = QDateTime::fromMSecsSinceEpoch(quint64(contact.wasOnline()) * 1000);
//            const QDate currentDate = QDateTime::currentDateTime().date();
//            if (wasOnline.date().daysTo(currentDate) > 7) {
//                return tr("Offline (since %1)").arg(wasOnline.toString(QLatin1String("yyyy.MM.dd")));
//            } else if (wasOnline.date() == currentDate) {
//                return tr("Offline (since %1)").arg(wasOnline.toString(QLatin1String("HH:mm")));
//            } else {
//                return tr("Offline (since %1)").arg(wasOnline.toString(QLatin1String("yyyy.MM.dd HH:mm")));
//            }
//        } else  {
//            return tr("Offline");
//        }
//    case TelegramNamespace::ContactStatusUnknown:
//    default:
        return tr("Unknown");
//    }
}

} // BrainIM namespace

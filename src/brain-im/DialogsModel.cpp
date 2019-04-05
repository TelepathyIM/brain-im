#include "DialogsModel.hpp"

#include <QDateTime>
#include <QTimer>

#include <QDebug>

//#define CONTACTLIST_AS_DIALOGS

namespace BrainIM {

static const int UserRoleOffset = Qt::UserRole + 1;

DialogsModel::DialogsModel(QObject *parent) :
    PeersModel(parent)
{
    QTimer::singleShot(200, this, &DialogsModel::populate);
}

QHash<int, QByteArray> DialogsModel::roleNames() const
{
    static const QHash<int, QByteArray> extraRoles {
        { UserRoleOffset + static_cast<int>(Role::Peer), "peer" },
        { UserRoleOffset + static_cast<int>(Role::DisplayName), "displayName" },
        { UserRoleOffset + static_cast<int>(Role::ChatType), "chatType" },
        { UserRoleOffset + static_cast<int>(Role::IsPinned), "isPinned" },
        { UserRoleOffset + static_cast<int>(Role::UnreadMessageCount), "unreadMessageCount" },
        { UserRoleOffset + static_cast<int>(Role::LastMessage), "lastMessage" },
        { UserRoleOffset + static_cast<int>(Role::FormattedLastMessage), "formattedLastMessage" },
    };

    return extraRoles;
}

bool DialogsModel::hasPeer(const Peer peer) const
{
    return true;
}

QString DialogsModel::getName(const Peer peer) const
{
//    if (peer.type == Peer::Type::Contact) {
//        Telegram::UserInfo info;
//        if (client->dataStorage()->getUserInfo(&info, peer.id)) {
//            return info.getBestDisplayName();
//        }
//    } else {
//        Telegram::ChatInfo info;
//        if (client->dataStorage()->getChatInfo(&info, peer.id)) {
//            return info.title();
//        }
//    }
    return peer.toString();
}

int DialogsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(Column::Count);
}

int DialogsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_dialogs.count();
}

QVariant DialogsModel::data(const QModelIndex &index, int role) const
{
    const Role r = indexToRole(index, role);
    return getData(index.row(), r);
}

QVariant DialogsModel::getData(int index, DialogsModel::Role role) const
{
    if (index < 0 || index >= m_dialogs.count()) {
        return QVariant();
    }
    const ChatEntry dialog = m_dialogs.at(index);

    switch (role) {
    case Role::Peer:
        return QVariant::fromValue(dialog.peer);
    case Role::ChatType:
        return QVariant::fromValue(dialog.chatType);
        return static_cast<int>(dialog.chatType);
    case Role::DisplayName:
        return dialog.name;
//    case Role::UnreadMessageCount:
//        return dialog.internal->unreadCount;
    case Role::FormattedLastMessage:
        return dialog.formattedLastMessage;
    case Role::LastMessage:
        return getDialogLastMessageData(dialog);
    case Role::Picture:
    case Role::MuteUntil:
    case Role::MuteUntilDate:
        // invalid roles
    case Role::Count:
    case Role::Invalid:
        return QVariant();
    default:
        return QVariant();
    }
    Q_UNREACHABLE();
    return QVariant();
}

QVariantMap DialogsModel::getDialogLastMessageData(const ChatEntry &dialog) const
{
//    if (dialog.lastChatMessage.id == 0) {
        return {};
//    }
//    const Telegram::Message &lastChatMessage = dialog.lastChatMessage;
//    QString text;
//    if (lastChatMessage.type == TelegramNamespace::MessageTypeText) {
//        text = lastChatMessage.text;
//    } else {
//        Telegram::MessageMediaInfo info;
//        client()->dataStorage()->getMessageMediaInfo(&info, dialog.internal->peer, lastChatMessage.id);
//        switch (lastChatMessage.type) {
//        case TelegramNamespace::MessageTypeWebPage:
//            text = lastChatMessage.text;
//            //text = info.url();
//            break;
//        case TelegramNamespace::MessageTypeSticker:
//            text = info.alt();
//            break;
//        case TelegramNamespace::MessageTypeDocument:
//            text = info.documentFileName();
//            break;
//        default:
//            text = info.caption();
//            break;
//        }
//    }

//    Telegram::UserInfo userInfo;
//    client()->dataStorage()->getUserInfo(&userInfo, lastChatMessage.fromId);

//    return {
//        { "type", static_cast<int>(lastChatMessage.type) },
//        { "text", text },
//        { "senderFirstName", userInfo.firstName() },
//        { "timestamp", QDateTime::fromSecsSinceEpoch(lastChatMessage.timestamp) },
//        { "flags", static_cast<int>(lastChatMessage.flags / 2) },
//    };
}

void DialogsModel::populate()
{
    beginResetModel();
    m_dialogs.clear();
    {
        ChatEntry c;
        c.name = QLatin1String("Telepathy IM dev");
        c.peer = Peer::fromRoomId(QLatin1String("123"));
        c.chatType = ChatTypeGroup;
        m_dialogs.append(c);
    }
    endResetModel();
}

void DialogsModel::onListChanged(const PeerList &added, const PeerList &removed)
{
    if (!removed.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Unimplemented argument";
    }
    if (!added.isEmpty()) {
        beginInsertRows(QModelIndex(), m_dialogs.count(), m_dialogs.count() + added.count() - 1);
        for (const Peer &p : added) {
            addPeer(p);
        }
        endInsertRows();
    }
}

void DialogsModel::addPeer(const Peer &peer)
{
//    Client *c = client();
    ChatEntry d;
//    DataInternalApi *internalApi = DataInternalApi::get(client()->dataStorage());
//    UserDialog *dialogData = internalApi->getDialog(peer);
//    if (!dialogData) {
//        qWarning() << Q_FUNC_INFO << "Unknown dialog";
//        return;
//    }
//    d.internal = dialogData;
//    d.chatType = getChatType(peer);
//    d.name = getPeerAlias(peer, c);
//    c->dataStorage()->getMessage(&d.lastChatMessage, peer, dialogData->topMessage);

    m_dialogs << d;
}

DialogsModel::ChatType DialogsModel::getChatType(const Peer &peer) const
{
    if (peer.type == Peer::Type::Contact) {
//        if (peer.id == client()->dataStorage()->selfUserId()) {
//            return DialogsModel::ChatTypeSelfChat;
//        }
        return DialogsModel::ChatTypeDialog;
    }
//    if (peer.type == Peer::Channel) {
//        ChatInfo info;
//        if (client()->dataStorage()->getChatInfo(&info, peer)) {
//            if (info.broadcast()) {
//                return DialogsModel::ChatTypeBroadcast;
//            }
//        }
        return DialogsModel::ChatTypeGroup;
//    }
//    return DialogsModel::ChatTypeMegaGroup;
}

DialogsModel::Role DialogsModel::intToRole(int value)
{
    if (value < 0 || value > static_cast<int>(Role::Count)) {
        return Role::Invalid;
    }
    return static_cast<Role>(value);
}

DialogsModel::Column DialogsModel::intToColumn(int value)
{
    if (value < 0 || value > static_cast<int>(Column::Count)) {
        return Column::Invalid;
    }
    return static_cast<Column>(value);
}

DialogsModel::Role DialogsModel::indexToRole(const QModelIndex &index, int role)
{
    if (role >= UserRoleOffset) {
        return intToRole(role - UserRoleOffset);
    }
    //const Column section = intToColumn(index.column());
    return Role::Invalid;
}

} // BrainIM namespace

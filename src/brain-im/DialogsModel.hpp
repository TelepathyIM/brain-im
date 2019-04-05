#ifndef BRAIN_IM_DIALOGS_MODEL_HPP
#define BRAIN_IM_DIALOGS_MODEL_HPP

#include "global.h"

#include <QList>
#include <QStringList>
#include <QSet>

#include "PeersModel.hpp"

namespace BrainIM {

class BRAIN_IM_EXPORT DialogsModel : public PeersModel
{
    Q_OBJECT
public:
    enum class Column {
        PeerType,
        PeerId,
        PeerName,
        ChatType,
        IsPinned,
        Picture, // Photo (in terms of Telegram)
        FormattedLastMessage,
        MuteUntil,
        MuteUntilDate,
        Count,
        Invalid
    };

    enum class Role {
        Peer,
        DisplayName,
        ChatType,
        IsPinned,
        Picture, // Photo (in terms of Telegram)
        LastMessage,
        FormattedLastMessage,
        MuteUntil,
        MuteUntilDate,
        UnreadMessageCount,
        Count,
        Invalid
    };

    enum ChatType {
        ChatTypeSelfChat,
        ChatTypeDialog,
        ChatTypeGroup,
        ChatTypeMegaGroup,
        ChatTypeBroadcast,
    };
    Q_ENUM(ChatType)

    struct ChatEntry {
        Peer peer;
        ChatType chatType;
        QString name;
        QString formattedLastMessage;
    };

    explicit DialogsModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;

    bool hasPeer(const Peer peer) const override;
    QString getName(const Peer peer) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant getData(int index, Role role) const;

public slots:
    void populate();

Q_SIGNALS:
    void clientChanged();

private slots:
    void onListChanged(const PeerList &added, const PeerList &removed);
    void addPeer(const Peer &peer);

private:
    ChatType getChatType(const Peer &peer) const;
    QVariantMap getDialogLastMessageData(const ChatEntry &dialog) const;
    static Role intToRole(int value);
    static Column intToColumn(int value);
    static Role indexToRole(const QModelIndex &index, int role = Qt::DisplayRole);
    QVector<ChatEntry> m_dialogs;

};

} // BrainIM namespace

#endif // TELEGRAMQT_DIALOGS_MODEL_HPP

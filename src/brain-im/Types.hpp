#ifndef BRAIN_TYPES
#define BRAIN_TYPES

#include "global.h"

#ifdef USE_TELEPATHY
#include <TelepathyQt/Constants>
#include <TelepathyQt/Types>
#endif

#include <QList>
#include <QDate>

#include <QDebug>

namespace BrainIM
{
    BRAIN_IM_EXPORT Q_NAMESPACE

//    BRAIN_IM_EXPORT extern const QMetaObject staticMetaObject;
//    QT_ANNOTATE_CLASS(qt_qnamespace, "")

enum EntityType
{
    EntityTypeUnknown, /**< the current contact's type is unknown **/
    EntityTypeContact, /**< the the contact's type represents a user (buddy), but not
                         the account's owner for which EntityTypeSelf is used **/
    EntityTypeRoom,    /**< a named room **/
    EntityTypeSelf     /**< the contact's type represents the owner of the account
                         whose channel has been logged, as opposed to
                         EntityTypeContact which represents any other user */
};

enum class MessageAction {
    MessageActionNone, // Cancel
    MessageActionTyping,
    MessageActionRecordVideo,
    MessageActionRecordAudio,
    MessageActionUploadVideo,
    MessageActionUploadAudio,
    MessageActionUploadPhoto,
    MessageActionUploadDocument,
    MessageActionGeoLocation,
    MessageActionChooseContact
};
Q_ENUM_NS(MessageAction)

class Event;
class CallEvent;
class MessageEvent;

class Entity;

typedef QSharedPointer<CallEvent> CallEventPtr;
typedef QSharedPointer<MessageEvent> MessageEventPtr;
typedef QSharedPointer<Entity> EntityPtr;
typedef QSharedPointer<Event> EventPtr;

class BRAIN_IM_EXPORT PeerEnums : public QObject
{
    Q_OBJECT
public:
    enum Type {
#ifdef USE_TELEPATHY
        Invalid = Tp::HandleTypeNone,
        Contact = Tp::HandleTypeContact,
        Room = Tp::HandleTypeRoom,
#else
        Invalid,
        Contact,
        Room ,
#endif
    };
    Q_ENUM(Type)
};

struct BRAIN_IM_EXPORT Peer
{
    Q_GADGET
    Q_PROPERTY(int type READ typeInt WRITE setTypeInt)
    Q_PROPERTY(QString id MEMBER id)
public:
    using Type = PeerEnums::Type;

    Peer() = default;

    Peer(const QString &id, Type t) : type(t), id(id)
    {
    }

    Type type = Type::Invalid;
    QString id;

    Q_INVOKABLE bool isValid() const { return (type != Type::Invalid) && id.isEmpty(); }
    int typeInt() const { return static_cast<int>(type); }
    void setTypeInt(int t) { type = static_cast<Type>(t); }

    bool operator==(const Peer &p) const
    {
        return (p.type == type) && (p.id == id);
    }

    static Peer fromContactId(const QString &id)
    {
        return Peer(id, Type::Contact);
    }

    static Peer fromRoomId(const QString &id)
    {
        return Peer(id, Type::Room);
    }
};

struct BRAIN_IM_EXPORT Contact
{
    Q_GADGET
    Q_PROPERTY(QString alias READ alias)
public:
    explicit Contact(const QString &alias = QString()) :
        m_alias(alias)
    {
    }

    void setId(const QString &id) { m_id = id; }

    QString alias() const { return m_alias; }
    QString id() const { return m_id; }

protected:
    QString m_alias;
    QString m_id;
};

class BRAIN_IM_EXPORT Brain : public QObject
{
    Q_OBJECT
public:
    enum ContactStatus {
        ContactStatusUnknown,
        ContactStatusOffline,
        ContactStatusOnline
    };
    Q_ENUM(ContactStatus)

    enum AuthSignError {
        AuthSignErrorUnknown,
        AuthSignErrorAppIdIsInvalid,
        AuthSignErrorPhoneNumberIsInvalid,
        AuthSignErrorPhoneNumberIsOccupied,
        AuthSignErrorPhoneNumberIsUnoccupied,
        AuthSignErrorPhoneCodeIsInvalid,
        AuthSignErrorPhoneCodeIsExpired,
        AuthSignErrorPasswordHashInvalid,
        AuthSignErrorFirstNameIsInvalid,
        AuthSignErrorLastNameIsInvalid
    };
    Q_ENUM(AuthSignError)

    enum UnauthorizedError {
        UnauthorizedUnknownError,
        UnauthorizedErrorKeyUnregistered,
        UnauthorizedErrorKeyInvalid,
        UnauthorizedErrorUserDeactivated,
        UnauthorizedErrorUserSessionRevoked,
        UnauthorizedErrorUserSessionExpired,
        UnauthorizedErrorActiveUserRequired,
        UnauthorizedErrorNeedPermanentKey,
        UnauthorizedSessionPasswordNeeded,
    };
    Q_ENUM(UnauthorizedError)

    enum UserNameStatus {
        UserNameStatusUnknown,
        UserNameStatusIsInvalid,
        UserNameStatusIsOccupied,
        UserNameStatusIsNotModified,
        UserNameStatusCanBeUsed,
        UserNameStatusCanNotBeUsed,
        UserNameStatusResolved,
        UserNameStatusAccepted
    };
    Q_ENUM(UserNameStatus)

    enum ConnectionState {
        ConnectionStateDisconnected,
        ConnectionStateConnecting,
        ConnectionStateConnected,
        ConnectionStateAuthRequired,
        ConnectionStateAuthenticated,
        ConnectionStateReady // Initializated
    };
    Q_ENUM(ConnectionState)

    enum ContactLastOnline {
        ContactLastOnlineUnknown,
        ContactLastOnlineRecently,
        ContactLastOnlineLastWeek,
        ContactLastOnlineLastMonth,
        ContactLastOnlineMask = 0xf
    };

    explicit Brain(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    Q_INVOKABLE static BrainIM::Peer emptyPeer()
    {
        return Peer();
    }

    Q_INVOKABLE static BrainIM::Peer peer(const QString &id, int type)
    {
        switch(type) {
        case Peer::Type::Contact:
            return Peer(id, Peer::Type::Contact);
        case Peer::Type::Room:
            return Peer(id, Peer::Type::Room);
        default:
            break;
        }
        return Peer();
    }

    Q_INVOKABLE static BrainIM::Peer peerFromContactId(const QString &id)
    {
        return Peer(id, Peer::Type::Contact);
    }

    Q_INVOKABLE static BrainIM::Peer peerFromRoomId(const QString &id)
    {
        return Peer(id, Peer::Type::Room);
    }

private:
    Q_DISABLE_COPY(Brain)
};

} // BrainIM namespace

Q_DECLARE_METATYPE(BrainIM::Peer)
Q_DECLARE_METATYPE(BrainIM::Peer::Type)

#endif // BRAIN_TYPES

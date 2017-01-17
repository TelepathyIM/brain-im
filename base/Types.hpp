#pragma once

#include <TelepathyQt/Types>
#include <QtCore/QList>
#include <QtCore/QDate>

namespace BrainIM
{

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

class Event;
class CallEvent;
class MessageEvent;

class Entity;

typedef QSharedPointer<CallEvent> CallEventPtr;
typedef QSharedPointer<MessageEvent> MessageEventPtr;
typedef QSharedPointer<Entity> EntityPtr;
typedef QSharedPointer<Event> EventPtr;

}

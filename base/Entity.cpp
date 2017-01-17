#include "Entity.hpp"

QString BrainIM::Entity::alias() const
{
    return QString();
}

QString BrainIM::Entity::identifier() const
{
    return QString();
}

BrainIM::EntityType BrainIM::Entity::entityType() const
{
    return EntityTypeUnknown;
}

QString BrainIM::Entity::avatarToken() const
{
    return QString();
}

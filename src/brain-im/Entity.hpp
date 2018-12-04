#pragma once

#include "Types.hpp"

namespace BrainIM
{

class Entity
{
public:
    /**
     * \brief Returns entity's alias
     */
    QString alias() const;

    /**
     * \brief Returns entity's unique identifier
     */
    QString identifier() const;

    /**
     * \brief Returns whether the entity is a contact, a room or account owner
     */
    EntityType entityType() const;

    /**
     * \brief Returns entity's avatar token
     */
    QString avatarToken() const;

private:
};

} //namespace

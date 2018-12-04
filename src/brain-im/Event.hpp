/*
   Copyright (C) 2018 Alexandr Akulich <akulichalexander@gmail.com>

   This file is a part of TelegramQt library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

 */

#ifndef BRAIN_IM_EVENT_HPP
#define BRAIN_IM_EVENT_HPP

#include "global.h"

#include "Types.hpp"

namespace BrainIM
{

class BRAIN_IM_EXPORT EventEnums : public QObject
{
    Q_OBJECT
public:
    enum class Type {
        Invalid       = 0,
        Message       = 1 << 0,
        Call          = 1 << 1,
        FileTransfer  = 1 << 2,
        NewDay        = 1 << 3,
        ServiceAction = 1 << 4,
    };
    Q_ENUM(Type)
    Q_DECLARE_FLAGS(Types, Type)
};

class BRAIN_IM_EXPORT Event
{
    Q_GADGET
    Q_PROPERTY(Type type MEMBER type)
public:
    using Type = EventEnums::Type;

//    /*!
//     * \brief Returns timestamp of the event
//     */
//    QDateTime timestamp() const;

//    /*!
//     *\brief Returns account path of the account the event is associated to.
//     */
//    QString accountPath() const;

//    /*!
//     * \brief Returns the account the event is associated to.
//     */
//    Tp::AccountPtr account() const;

//    /*!
//     * \brief Returns the entity that originated the log event.
//     */
//    EntityPtr sender() const;

//    /*!
//     * \brief Returns the entity thas was destination for the log event.
//     */
//    EntityPtr receiver() const;

//    /*!
//     * \brief Equivalent to operator==().
//     */
//    bool equalTo(const EventPtr & rhs) const;

//    /*!
//     * \brief Comparison operator.
//     */
//    bool operator==(const EventPtr & rhs ) const;

    Type type = Type::Invalid;
};

} //namespace

#endif // BRAIN_IM_EVENT_HPP

/*
   Copyright (C) 2018 Alexandr Akulich <akulichalexander@gmail.com>

   This file is a part of BrainIM library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

 */

#ifndef BRAIN_IM_GLOBAL_H
#define BRAIN_IM_GLOBAL_H

#include <QtCore/QtGlobal>

#define BRAIN_IM_MAJOR_VERSION 0
#define BRAIN_IM_MINOR_VERSION 1
#define BRAIN_IM_MICRO_VERSION 0
#define BRAIN_IM_VERSION ((BRAIN_IM_MAJOR_VERSION<<16)|(BRAIN_IM_MINOR_VERSION<<8)|(BRAIN_IM_MICRO_VERSION))

/*!
    \macro BRAIN_IM_VERSION_CHECK
    The BRAIN_IM_VERSION_CHECK macro can be used to do condition compilation
    in BrainIM-based applications and libraries.
    Example:
    \snippet code/src_corelib_global_qglobal.cpp qt-version-check
    \code
#if (BRAIN_IM_VERSION < BRAIN_IM_VERSION_CHECK(0, 2, 0))
    model->getName(BrainIM::Peer::Contact, QStringLiteral("contact1"));
#else
    model->getName(BrainIM::Peer::fromContactId(QStringLiteral("contact1")));
#endif
    \endcode
    \sa QT_VERSION
*/

#define BRAIN_IM_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

#if defined(BRAIN_IM_LIBRARY)
#define BRAIN_IM_EXPORT Q_DECL_EXPORT
#else
#define BRAIN_IM_EXPORT Q_DECL_IMPORT
#endif

#endif // BRAIN_IM_GLOBAL_H

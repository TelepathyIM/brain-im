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

#include "TelepathyMessagesModel.hpp"
#include "Core.hpp"

#include <TelepathyQt/Account>
#include <TelepathyQt/AccountManager>
#include <TelepathyQt/Contact>
#include <TelepathyQt/ContactManager>
#include <TelepathyQt/PendingChannel>
#include <TelepathyQt/PendingChannelRequest>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/ReceivedMessage>
#include <TelepathyQt/TextChannel>

#include <QLoggingCategory>


namespace BrainIM {

TelepathyMessagesModel::TelepathyMessagesModel(QObject *parent) :
    MessagesModel(parent)
{
    connect(this, &MessagesModel::peerChanged, this, &TelepathyMessagesModel::onPeerChanged);
}

void TelepathyMessagesModel::setChannel(const Tp::ChannelPtr &channel)
{
    qWarning() << Q_FUNC_INFO << channel.data();
    if (!channel) {
        qWarning() << Q_FUNC_INFO << "no channel given";
        return;
    }
    m_channel = Tp::TextChannelPtr::dynamicCast(channel);
    if (!m_channel) {
        qWarning() << Q_FUNC_INFO << "the channel is not message channel";
        return;
    }
    connect(m_channel.data(), &Tp::TextChannel::messageReceived, this, &TelepathyMessagesModel::onMessageReceived);
    qWarning() << Q_FUNC_INFO << m_channel->supportedMessageTypes();
    if (channel->isReady()) {
        qWarning() << Q_FUNC_INFO << "Already ready";
        onChannelReady(nullptr);
    } else {
        qWarning() << Q_FUNC_INFO << "Become ready";
        connect(channel->becomeReady(Tp::TextChannel::FeatureMessageCapabilities), &Tp::PendingOperation::finished, this, &TelepathyMessagesModel::onChannelReady);
    }
}

void TelepathyMessagesModel::setPeerContact(Tp::ContactPtr contact)
{
    clear();

    qWarning() << Q_FUNC_INFO << contact.data() << contact->alias();

#if TP_QT_VERSION >= TP_QT_VERSION_CHECK(0, 9, 9)
    const QString accountId = contact->manager()->connection()->accountUniqueIdentifier();
#else
    const QString accountId = QStringLiteral("invalid");
#endif
    qWarning() << Q_FUNC_INFO << "needed acc" << accountId;

    const auto accounts = BrainIM::accountManager()->allAccounts();
    if (accounts.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "not accounts";
        return;
    }
    for (Tp::AccountPtr account : accounts) {
        if (account->uniqueIdentifier() != accountId) {
            continue;
        }
        Tp::PendingChannel *request = account->ensureAndHandleTextChat(contact);
        connect(request, &Tp::PendingOperation::finished, this, [this](Tp::PendingOperation *operation) {
            Tp::PendingChannel *request = qobject_cast<Tp::PendingChannel *>(operation);
            if (!request) {
                qWarning() << Q_FUNC_INFO << "Invalid call";
                return;
            }
            setChannel(request->channel());
        });

        // Tp::PendingChannelRequest *request = account->ensureTextChat(contact);

        // connect(request, &Tp::PendingOperation::finished, this, [this](Tp::PendingOperation *operation) {
        //     Tp::PendingChannelRequest *request = qobject_cast<Tp::PendingChannelRequest *>(operation);
        //     if (!request) {
        //         qWarning() << Q_FUNC_INFO << "Invalid call";
        //         return;
        //     }

        //     setChannelRequest(request->channelRequest());
        // });
        return;
    }
    qWarning() << Q_FUNC_INFO << "no needed account";
}

void TelepathyMessagesModel::setChannelRequest(Tp::ChannelRequestPtr request)
{
    connect(request.data(), &Tp::ChannelRequest::succeeded, this, &TelepathyMessagesModel::setChannel);
}

void TelepathyMessagesModel::onChannelReady(Tp::PendingOperation *op)
{
    Q_UNUSED(op)

    qWarning() << m_channel->targetId() << m_channel->interfaces();
    //m_channel->messageReceived();
}

void TelepathyMessagesModel::onPeerChanged(const Peer peer)
{
    qWarning() << Q_FUNC_INFO;
    Q_UNUSED(peer)
}

void TelepathyMessagesModel::onMessageReceived(const Tp::ReceivedMessage &message)
{
    qWarning() << Q_FUNC_INFO << message.text();
    {
        MessageEvent *event = new BrainIM::MessageEvent();
        event->setPeer(Peer(message.sender()->id(), Peer::Type::Contact));
        event->text = message.text();
#if TP_QT_VERSION >= TP_QT_VERSION_CHECK(0, 9, 9)
        event->receivedTimestamp = message.receivedTimestamp();
        event->sentTimestamp = message.sentTimestamp();
#endif
        beginInsertRows(QModelIndex(), m_events.count(), m_events.count());
        m_events << event;
        endInsertRows();
    }
}

} // BrainIM namespace

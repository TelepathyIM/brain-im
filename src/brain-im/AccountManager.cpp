#include "AccountManager.hpp"

static Tp::AccountManagerPtr s_instance;

namespace BrainIM
{

Tp::AccountManagerPtr getAccountManager()
{
    if (s_instance.isNull()) {

//        ChannelFactoryPtr channelFactory = ChannelFactory::create(
//                QDBusConnection::sessionBus());
//        ConnectionFactoryPtr connectionFactory = ConnectionFactory::create(
//                QDBusConnection::sessionBus(), Connection::FeatureConnected |
//                    Connection::FeatureRoster | Connection::FeatureRosterGroups);
//        ContactFactoryPtr contactFactory = ContactFactory::create(
//                Contact::FeatureAlias | Contact::FeatureSimplePresence);

//        KTp::accountFactory(),
//        KTp::connectionFactory(),
//        KTp::channelFactory(),
//        KTp::contactFactory());

//        s_instance = Tp::AccountManager::create(Tp::AccountFactory::create(QDBusConnection::sessionBus(),

//                                                                           Tp::Account::FeatureCore));
    }
    return s_instance;
}

} // BrainIM namespace

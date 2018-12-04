#include "Core.hpp"
#include <TelepathyQt/AccountFactory>
#include <TelepathyQt/AccountManager>
#include <TelepathyQt/ConnectionFactory>
#include <TelepathyQt/ContactFactory>

class CorePrivate
{
public:
    CorePrivate();
    bool m_kPeopleEnabled;
    Tp::AccountFactoryPtr m_accountFactory;
    Tp::ConnectionFactoryPtr m_connectionFactory;
    Tp::ContactFactoryPtr m_contactFactory;
    Tp::ChannelFactoryPtr m_channelFactory ;

    Tp::AccountManagerPtr m_accountManager;
    BrainIM::GlobalContactManager *m_contactManager = nullptr;
};

CorePrivate::CorePrivate()
{
    m_accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                  Tp::Features()
                                                  << Tp::Account::FeatureCore
                                                  );

    m_connectionFactory = Tp::ConnectionFactory::create(QDBusConnection::sessionBus(),
                                                        Tp::Features()
                                                        << Tp::Connection::FeatureCore
                                                        << Tp::Connection::FeatureRoster
                                                        << Tp::Connection::FeatureSelfContact);

    m_contactFactory = Tp::ContactFactory::create(Tp::Features()
                                                  << Tp::Contact::FeatureAlias
                                                  << Tp::Contact::FeatureSimplePresence
                                                  << Tp::Contact::FeatureCapabilities
                                                  << Tp::Contact::FeatureClientTypes
                                                  << Tp::Contact::FeatureAvatarData);

    m_channelFactory = Tp::ChannelFactory::create(QDBusConnection::sessionBus());
}

Q_GLOBAL_STATIC(CorePrivate, s_instance)

Tp::AccountFactoryConstPtr BrainIM::accountFactory()
{
    return s_instance->m_accountFactory;
}

Tp::ConnectionFactoryConstPtr BrainIM::connectionFactory()
{
    return s_instance->m_connectionFactory;
}

Tp::ChannelFactoryConstPtr BrainIM::channelFactory()
{
    return s_instance->m_channelFactory;
}

Tp::ContactFactoryConstPtr BrainIM::contactFactory()
{
    return s_instance->m_contactFactory;
}

Tp::AccountManagerPtr BrainIM::accountManager()
{
    if (!s_instance->m_accountManager) {
        s_instance->m_accountManager = Tp::AccountManager::create(QDBusConnection::sessionBus(),
                                                   accountFactory(),
                                                   connectionFactory(),
                                                   channelFactory(),
                                                   contactFactory());
    }
    return s_instance->m_accountManager;
}

BrainIM::GlobalContactManager *BrainIM::contactManager()
{
    return s_instance->m_contactManager;
}

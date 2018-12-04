#ifndef BRAIN_IM_CORE_HPP
#define BRAIN_IM_CORE_HPP

#include <TelepathyQt/Types>

namespace BrainIM
{

class GlobalContactManager;

Tp::AccountFactoryConstPtr accountFactory();
Tp::ConnectionFactoryConstPtr connectionFactory();
Tp::ChannelFactoryConstPtr channelFactory();
Tp::ContactFactoryConstPtr contactFactory();
Tp::AccountManagerPtr accountManager();
GlobalContactManager *contactManager();

} // BrainIM namespace

#endif // BRAIN_IM_CORE_HPP

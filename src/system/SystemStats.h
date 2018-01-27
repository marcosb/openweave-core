/*
 *
 *    Copyright (c) 2016-2017 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 * @file
 *  This file declares the Weave API to collect statistics
 *  on the state of Weave, Inet and System resources
 */

#ifndef SYSTEMSTATS_H
#define SYSTEMSTATS_H
// Include standard C library limit macros
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#include <stdint.h>

// Include configuration headers
#include <Weave/Core/WeaveConfig.h>
#include <Weave/Core/WeaveWRMPConfig.h>

// Include dependent headers
#include <Weave/Support/NLDLLUtil.h>

namespace nl {
namespace Weave {
namespace System {
namespace Stats {

enum
{
    kSystemLayer_NumPacketBufs,
    kSystemLayer_NumTimers,
#if INET_CONFIG_NUM_RAW_ENDPOINTS
    kInetLayer_NumRawEps,
#endif // INET_CONFIG_NUM_RAW_ENDPOINTS
#if INET_CONFIG_NUM_TCP_ENDPOINTS
    kInetLayer_NumTCPEps,
#endif // INET_CONFIG_NUM_TCP_ENDPOINTS
#if INET_CONFIG_NUM_UDP_ENDPOINTS
    kInetLayer_NumUDPEps,
#endif // INET_CONFIG_NUM_UDP_ENDPOINTS
#if INET_CONFIG_NUM_TUN_ENDPOINTS
    kInetLayer_NumTunEps,
#endif // INET_CONFIG_NUM_TUN_ENDPOINTS
#if INET_CONFIG_NUM_DNS_RESOLVERS
    kInetLayer_NumDNSResolvers,
#endif // INET_CONFIG_NUM_DNS_RESOLVERS
    kExchangeMgr_NumContexts,
    kExchangeMgr_NumUMHandlers,
#if WEAVE_CONFIG_ENABLE_RELIABLE_MESSAGING
    kExchangeMgr_NumBindings,
#endif // WEAVE_CONFIG_ENABLE_RELIABLE_MESSAGING
    kMessageLayer_NumConnections,
#if WEAVE_CONFIG_ENABLE_SERVICE_DIRECTORY
    kServiceMgr_NumRequests,
#endif // WEAVE_CONFIG_ENABLE_SERVICE_DIRECTORY
    kWDMLegacy_NumViews,
#if WEAVE_CONFIG_WDM_ALLOW_CLIENT_SUBSCRIPTION
    kWDMLegacy_NumSubscribes,
    kWDMLegacy_NumCancels,
#endif // WEAVE_CONFIG_WDM_ALLOW_CLIENT_SUBSCRIPTION
    kWDMLegacy_NumUpdates,
    kWDMLegacy_NumBindings,
    kWDMLegacy_NumTransactions,
#if WDM_ENABLE_SUBSCRIPTION_PUBLISHER
    kWDM_NumTraits,
#endif // WDM_ENABLE_SUBSCRIPTION_PUBLISHER
#if WDM_ENABLE_SUBSCRIPTION_CLIENT
    kWDM_NumSubscriptionClients,
#endif // WDM_ENABLE_SUBSCRIPTION_CLIENT
#if WDM_ENABLE_SUBSCRIPTION_PUBLISHER
    kWDM_NumSubscriptionHandlers,
#endif // WDM_ENABLE_SUBSCRIPTION_PUBLISHER
#if WDM_PUBLISHER_ENABLE_CUSTOM_COMMANDS
    kWDM_NumCommands,
#endif // WDM_PUBLISHER_ENABLE_CUSTOM_COMMANDS

    kNumEntries
};

typedef int8_t count_t;
#define PRI_WEAVE_SYS_STATS_COUNT PRId8

extern count_t ResourcesInUse[kNumEntries];
extern count_t HighWatermarks[kNumEntries];

class Snapshot
{
public:

    count_t mResourcesInUse[kNumEntries];
    count_t mHighWatermarks[kNumEntries];
};

bool Difference(Snapshot &result, Snapshot &after, Snapshot &before);
void UpdateSnapshot(Snapshot &aSnapshot);
count_t *GetResourcesInUse(void);
count_t *GetHighWatermarks(void);

typedef const char *Label;
const Label *GetStrings(void);

} // namespace Stats
} // namespace System
} // namespace Weave
} // namespace nl

#if WEAVE_SYSTEM_CONFIG_PROVIDE_STATISTICS

#define SYSTEM_STATS_INCREMENT(entry) \
    do { \
        nl::Weave::System::Stats::count_t new_value = ++(nl::Weave::System::Stats::GetResourcesInUse()[entry]); \
        if (nl::Weave::System::Stats::GetHighWatermarks()[entry] < new_value) \
        { \
            nl::Weave::System::Stats::GetHighWatermarks()[entry] = new_value; \
        } \
    } while (0);

#define SYSTEM_STATS_DECREMENT(entry) \
    do { \
        nl::Weave::System::Stats::GetResourcesInUse()[entry]--; \
    } while (0);

#define SYSTEM_STATS_DECREMENT_BY_N(entry, count) \
    do { \
        nl::Weave::System::Stats::GetResourcesInUse()[entry] -= (count); \
    } while (0);

#define SYSTEM_STATS_SET(entry, count) \
    do { \
        nl::Weave::System::Stats::count_t new_value = nl::Weave::System::Stats::GetResourcesInUse()[entry] = (count); \
        if (nl::Weave::System::Stats::GetHighWatermarks()[entry] < new_value) \
        { \
            nl::Weave::System::Stats::GetHighWatermarks()[entry] = new_value; \
        } \
    } while (0);

#define SYSTEM_STATS_RESET(entry) \
    do { \
        nl::Weave::System::Stats::GetResourcesInUse()[entry] = 0; \
    } while (0);



#else // WEAVE_SYSTEM_CONFIG_PROVIDE_STATISTICS

#define SYSTEM_STATS_INCREMENT(entry)

#define SYSTEM_STATS_DECREMENT(entry)

#define SYSTEM_STATS_DECREMENT_BY_N(entry, count)

#define SYSTEM_STATS_RESET(entry)

#endif // WEAVE_SYSTEM_CONFIG_PROVIDE_STATISTICS

#endif // defined(SYSTEMSTATS_H)

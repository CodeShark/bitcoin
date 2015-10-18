// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_BLOCKRULEINDEX_H
#define BITCOIN_BLOCKRULEINDEX_H

#include "versionbits.h"

#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>

class CBlockIndex;

namespace Consensus
{

namespace VersionBits
{

typedef std::map<int /*rule*/, RuleState> RuleStates;
typedef std::map<const CBlockIndex*, RuleStates> RuleStateMap;

class BlockRuleIndex
{
public:
    BlockRuleIndex(int activationInterval = 0, const SoftForkDeployments* deployments = NULL) :
        m_activationInterval(activationInterval), m_deployments(deployments) { }

    int GetActivationInterval() const { return m_activationInterval; }

    bool IsIntervalStart(const CBlockIndex* pblockIndex) const;

    const CBlockIndex* GetIntervalStart(const CBlockIndex* pblockIndex) const;

    // Clears the index, so should only be called upon initialization
    void SetSoftForkDeployments(int activationInterval, const SoftForkDeployments* deployments);

    // Constructs a new nVersion field with bits set for all deployments that have not yet activated or failed
    // The disabledRules parameter allows us to request that the bit for a rule be unset if it has not yet locked in
    int CreateBlockVersion(uint32_t nTime, CBlockIndex* pprev, const std::set<int>& disabledRules = std::set<int>()) const;

    RuleState GetRuleState(int rule, const CBlockIndex* pblockIndex) const;

    RuleStates GetRuleStates(const CBlockIndex* pblockIndex) const;

    // Returns false if the block does not connect or the version has set bits that shouldn't be set
    bool AreVersionBitsRecognized(const CBlockIndex* pblockIndex, const CBlockIndex* pprev = NULL) const;

    bool InsertBlockIndex(const CBlockIndex* pblockIndex, const CBlockIndex* pprev = NULL);

    void Clear();

#ifdef VERSIONBITS_UNIT_TEST
    // For testing only

    // Inserts arbitrary rule states 
    void InsertBlockIndexWithRuleStates(const CBlockIndex* pblockIndex, const RuleStates& ruleStates)
    {
        m_ruleStateMap[pblockIndex] = ruleStates;
    }
#endif

private:
    int m_activationInterval;
    const SoftForkDeployments* m_deployments;
    RuleStateMap m_ruleStateMap;
};

BlockRuleIndex& GetBlockRuleIndex();

}
}

#endif // BITCOIN_BLOCKRULEINDEX_H

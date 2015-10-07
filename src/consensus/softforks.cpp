// Copyright (c) 2009-2015 Eric Lombrozo, The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chain.h"
#include "consensus/params.h"
#include "softforks.h"

using namespace Consensus::SoftForks;

/**
 * Returns true if there are nRequired or more blocks of minVersion or above
 * in the last Consensus::Params::nMajorityWindow blocks, starting at pstart and going backwards.
 */
static bool IsSuperMajority(int minVersion, const CBlockIndex* pstart, unsigned nRequired, const Consensus::Params& consensusParams)
{
    unsigned int nFound = 0;
    for (int i = 0; i < consensusParams.nMajorityWindow && nFound < nRequired && pstart != NULL; i++)
    {
        if (pstart->nVersion >= minVersion)
            ++nFound;
        pstart = pstart->pprev;
    }
    return (nFound >= nRequired);
}

bool Consensus::SoftForks::IsValidVersion(const CBlockIndex& blockIndex, const Consensus::Params& consensusParams, CBlockIndex* pindexPrev)
{
    if (!pindexPrev)
        pindexPrev = blockIndex.pprev;

    // Reject blockIndex.nVersion=1 blocks when 95% (75% on testnet) of the network has upgraded:
    if (blockIndex.nVersion < 2 && IsSuperMajority(2, pindexPrev, consensusParams.nMajorityRejectBlockOutdated, consensusParams))
        return false;

    // Reject blockIndex.nVersion=2 blocks when 95% (75% on testnet) of the network has upgraded:
    if (blockIndex.nVersion < 3 && IsSuperMajority(3, pindexPrev, consensusParams.nMajorityRejectBlockOutdated, consensusParams))
        return false;

    // Reject blockIndex.nVersion=3 blocks when 95% (75% on testnet) of the network has upgraded:
    // DEPLOY BIP65 - Uncomment the following line to deploy
    //if (blockIndex.nVersion < 4 && IsSuperMajority(4, pindexPrev, consensusParams.nMajorityRejectBlockOutdated, consensusParams))
    //    return false;

    return true;
}

bool Consensus::SoftForks::UseRule(Rule rule, const CBlockIndex& blockIndex, const Consensus::Params& consensusParams, CBlockIndex* pindexPrev)
{
    if (!pindexPrev)
        pindexPrev = blockIndex.pprev;

    switch (rule)
    {
    case BIP16:
        // BIP16 didn't become active until Apr 1 2012
        return blockIndex.GetBlockTime() >= 1333238400;

    case BIP30:
        // Do not allow blocks that contain transactions which 'overwrite' older transactions,
        // unless those are already completely spent.
        // If such overwrites are allowed, coinbases and transactions depending upon those
        // can be duplicated to remove the ability to spend the first instance -- even after
        // being sent to another address.
        // See BIP30 and http://r6.ca/blog/20120206T005236Z.html for more information.
        // This logic is not necessary for memory pool transactions, as AcceptToMemoryPool
        // already refuses previously-known transaction ids entirely.
        // This rule was originally applied to all blocks with a timestamp after March 15, 2012, 0:00 UTC.
        // Now that the whole chain is irreversibly beyond that time it is applied to all blocks except the
        // two in the chain that violate it. This prevents exploiting the issue against nodes during their
        // initial block download.
        return (!blockIndex.phashBlock) ||
            !((blockIndex.nHeight == 91842 && blockIndex.GetBlockHash() == uint256S("0x00000000000a4d0a398161ffc163c503763b1f4360639393e0e4c8e300e0caec")) ||
             (blockIndex.nHeight == 91880 && blockIndex.GetBlockHash() == uint256S("0x00000000000743f190a18c5577a3c2d2a1f610ae9601ac046a38084ccb7cd721")));

    case BIP34:
        // Enforce block.nVersion=2 rule that the coinbase starts with serialized block height
        // if 750 of the last 1,000 blocks are version 2 or greater (51/100 if testnet):
        return blockIndex.nVersion >= 2 && IsSuperMajority(2, pindexPrev, consensusParams.nMajorityEnforceBlockUpgrade, consensusParams);
        
    case BIP42:
        return false;

    case BIP62:
        return false;
    
    case BIP65:
        // Start enforcing CHECKLOCKTIMEVERIFY, (BIP65) for blockIndex.nVersion=4, when 75% of the network has upgraded:
        // DEPLOY BIP65 - Remove the following line to deploy
        return false;
        return blockIndex.nVersion >= 4 && IsSuperMajority(4, pindexPrev, consensusParams.nMajorityEnforceBlockUpgrade, consensusParams);

    case BIP66:
        // Start enforcing the DERSIG (BIP66) rules, for block.nVersion=3 blocks, when 75% of the network has upgraded:
        return blockIndex.nVersion >= 3 && IsSuperMajority(3, pindexPrev, consensusParams.nMajorityEnforceBlockUpgrade, consensusParams);
        //return (blockIndex.nVersion >= 3) && (blockIndex.nHeight >= 358806 && blockIndex.GetAncestor(358806)->GetBlockHash() == uint256S("00000000000000000fb122c91c210b3060fff913e483711f25bfb35d1d63537b"));

    case BIP68:
        return false;

    case BIP112:
        return false;

    case BIP113:
        return false;

    default:
        return false;
    }
}

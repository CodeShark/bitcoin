// Copyright (c) 2009-2015 Eric Lombrozo, The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SOFTFORKS_H
#define BITCOIN_SOFTFORKS_H

class CBlockIndex;
namespace Consensus {

struct Params;
namespace SoftForks {

/**
 * Returns true if there are nRequired or more blocks of minVersion or above
 * in the last Consensus::Params::nMajorityWindow blocks, starting at pstart and going backwards.
 */
bool IsSuperMajority(int minVersion, const CBlockIndex* pstart, unsigned nRequired, const Consensus::Params& consensusParams);

enum Rule
{
    BIP16,
    BIP30,
    BIP34,
    BIP42,
    BIP62,
    BIP65,
    BIP66,
    BIP68,
    BIP112,
    BIP113
};

bool IsValidVersion(const CBlockIndex& blockIndex, const Consensus::Params& consensusParams, CBlockIndex* pindexPrev = NULL);
bool UseRule(Rule rule, const CBlockIndex& blockIndex, const Consensus::Params& consensusParams, CBlockIndex* pindexPrev = NULL);

}
}

#endif // BITCOIN_SOFTFORKS_H

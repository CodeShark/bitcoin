// Copyright (c) 2015 Eric Lombrozo
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SOFTFORKS_H
#define BITCOIN_SOFTFORKS_H

class CBlockIndex;
namespace Consensus {

struct Params;
namespace SoftForks {

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
    BIP113,
    BIP9999,
};

enum VersionStatus { VALID, UNRECOGNIZED, INVALID };

VersionStatus CheckVersion(const CBlockIndex& blockIndex, const Consensus::Params& consensusParams, CBlockIndex* pindexPrev = NULL);
bool UseRule(int rule, const CBlockIndex& blockIndex, const Consensus::Params& consensusParams, CBlockIndex* pindexPrev = NULL);
const char* GetRuleName(int rule);

}
}

#endif // BITCOIN_SOFTFORKS_H

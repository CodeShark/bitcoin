// Copyright (c) 2015 Eric Lombrozo
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chain.h"
#include "softforks.h"

using namespace SoftForks;

bool SoftForks::UseRule(Rule rule, const CBlockIndex& blockIndex) 
{
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
            !((blockIndex.nHeight == 91880 && blockIndex.GetBlockHash() == uint256S("0x00000000000743f190a18c5577a3c2d2a1f610ae9601ac046a38084ccb7cd721"))));

    case BIP34:
        return false;
        
    case BIP42:
        return false;

    case BIP62:
        return false;
    
    case BIP65:
        return false;

    case BIP66:
        // Start enforcing the DERSIG (BIP66) rules, for block.nVersion=3 blocks, when 75% of the network has upgraded:
        return (blockIndex.nHeight >= 358806 && blockIndex.GetAncestor(358806)->GetBlockHash() == uint256S("00000000000000000fb122c91c210b3060fff913e483711f25bfb35d1d63537b"));

    case BIP68:
        return false;

    default:
        return false;
    }
}

// Copyright (c) 2015 Eric Lombrozo
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SOFTFORKRULES_H
#define BITCOIN_SOFTFORKRULES_H

#include "softforkrules.h"

class CBlockIndex;

namespace SoftForks
{

const unsigned int ACTIVATION_OFFSET = 0;
const unsigned int ACTIVATION_INTERVAL = 2016;

enum Rule
{
    BIP16,
    BIP30,
    BIP34,
    BIP42,
    BIP62,
    BIP65,
    BIP66,
    BIP68
};

bool UseRule(Rule rule, const CBlockIndex& blockIndex);

}

#endif // BITCOIN_SOFTFORKRULES_H


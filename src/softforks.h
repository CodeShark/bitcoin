// Copyright (c) 2015 Eric Lombrozo
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SOFTFORKS_H
#define BITCOIN_SOFTFORKS_H

#include "softforkrules.h"

#include <stdexcept>
#include <stdint.h>

namespace SoftForks
{

class CSoftFork
{
public:
    enum State { DEFINED, LOCKED_IN, ACTIVE, FAILED };

    CSoftFork(Rule rule, unsigned int bit, unsigned int threshold, uint32_t timeout, State state = DEFINED);

    Rule GetRule() const { return m_rule; }
    unsigned int GetBit() const { return m_bit; }
    uint32_t GetBitMask() const { return (uint32_t)0x01 << m_bit; }
    unsigned int GetThreshold() const { return m_threshold; }
    uint32_t GetTimeout() const { return m_timeout; }
    State GetState() const { return m_state; }

private:
    Rule m_rule;
    unsigned int m_bit;
    unsigned int m_threshold;
    uint32_t m_timeout;
    State m_state;
};

}

#endif // BITCOIN_SOFTFORKS_H


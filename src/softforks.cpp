// Copyright (c) 2015 Eric Lombrozo
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "softforks.h"

using namespace SoftForks;

CSoftFork::CSoftFork(Rule rule, unsigned int bit, unsigned int threshold, uint32_t timeout, State state) :
    m_rule(rule),
    m_bit(bit),
    m_threshold(threshold),
    m_timeout(timeout),
    m_state(state)
{
    if (m_bit >= 29)
        throw std::runtime_error("CSoftFork::CSoftFork() - invalid bit.");

    if (m_threshold > ACTIVATION_INTERVAL)
        throw std::runtime_error("CSoftFork::CSoftFork() - invalid threshold.");
}

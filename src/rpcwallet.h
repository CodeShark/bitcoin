// Copyright (c) 2014 Eric Lombrozo
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef _BITCOIN_RPCWALLET
#define _BITCOIN_RPCWALLET

#include <string>

extern int64_t nWalletUnlockTime;
extern std::string HelpRequiringPassphrase();
extern void EnsureWalletIsUnlocked();

namespace RPCWallet
{
    void Register();
}

#endif // _BITCOIN_RPCWALLET

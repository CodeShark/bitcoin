// Copyright (c) 2014 Eric Lombrozo
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef _BITCOIN_RPCALL
#define _BITCOIN_RPCALL

#include "rpcblockchain.h"
#include "rpcmining.h"
#include "rpcmisc.h"
#include "rpcnet.h"
#include "rpcrawtransaction.h"

#ifdef ENABLE_WALLET
#include "rpcdump.h"
#include "rpcwallet.h"
#endif

namespace RPCAll
{
    inline void Register()
    {
        RPCBlockchain::Register();
        RPCMining::Register();
        RPCMisc::Register();
        RPCNet::Register();
        RPCRawTransaction::Register();
#ifdef ENABLE_WALLET
        RPCDump::Register();
        RPCWallet::Register();
#endif
    }
}

#endif // _BITCOIN_RPCALL

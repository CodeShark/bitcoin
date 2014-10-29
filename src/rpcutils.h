// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef _BITCOIN_RPCUTILS
#define _BITCOIN_RPCUTILS

#include "amount.h"
#include "core.h"
#include "uint256.h"
#include "rpcprotocol.h"

#include <list>
#include <map>
#include <stdint.h>
#include <string>

#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_utils.h"
#include "json/json_spirit_writer_template.h"

//
// Utilities: convert hex-encoded Values
// (throws error if not hex).
//
inline int64_t roundint64(double d)
{
    return (int64_t)(d > 0 ? d + 0.5 : d - 0.5);
}

inline CAmount AmountFromValue(const json_spirit::Value& value)
{
    double dAmount = value.get_real();
    if (dAmount <= 0.0 || dAmount > 21000000.0)
        throw JSONRPCError(RPC_TYPE_ERROR, "Invalid amount");
    CAmount nAmount = roundint64(dAmount * COIN);
    if (!MoneyRange(nAmount))
        throw JSONRPCError(RPC_TYPE_ERROR, "Invalid amount");
    return nAmount;
}

inline json_spirit::Value ValueFromAmount(const CAmount& amount)
{
    return (double)amount / (double)COIN;
}

inline uint256 ParseHashV(const json_spirit::Value& v, std::string strName)
{
    std::string strHex;
    if (v.type() == json_spirit::str_type)
        strHex = v.get_str();
    if (!IsHex(strHex)) // Note: IsHex("") is false
        throw JSONRPCError(RPC_INVALID_PARAMETER, strName+" must be hexadecimal string (not '"+strHex+"')");
    uint256 result;
    result.SetHex(strHex);
    return result;
}

inline uint256 ParseHashO(const json_spirit::Object& o, std::string strKey)
{
    return ParseHashV(json_spirit::find_value(o, strKey), strKey);
}

inline std::vector<unsigned char> ParseHexV(const json_spirit::Value& v, std::string strName)
{
    std::string strHex;
    if (v.type() == json_spirit::str_type)
        strHex = v.get_str();
    if (!IsHex(strHex))
        throw JSONRPCError(RPC_INVALID_PARAMETER, strName+" must be hexadecimal string (not '"+strHex+"')");
    return ParseHex(strHex);
}

inline std::vector<unsigned char> ParseHexO(const json_spirit::Object& o, std::string strKey)
{
    return ParseHexV(find_value(o, strKey), strKey);
}

//
// Help Examples
//

inline std::string HelpExampleCli(std::string methodname, std::string args)
{
    return "> bitcoin-cli " + methodname + " " + args + "\n";
}

inline std::string HelpExampleRpc(std::string methodname, std::string args)
{
    return "> curl --user myusername --data-binary '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", "
        "\"method\": \"" + methodname + "\", \"params\": [" + args + "] }' -H 'content-type: text/plain;' http://127.0.0.1:8332/\n";
}

#endif // _BITCOIN_RPC_UTILS

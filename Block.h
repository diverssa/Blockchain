#ifndef TESTCHAIN_BLOCK_H
#define TESTCHAIN_BLOCK_H

#include <cstdint>
#include <iostream>
#include <sstream>

using namespace std;

class Block {
public:
    string sHash;
    string sPrevHash;
    uint64_t stakeAmount;
    string stakeOwner;
    bool IsBlockValid() const;
    string _CalculateHash() const;
    void MineBlock(uint32_t nDifficulty, uint64_t stakeAmount, const string& stakeOwner);
    Block(uint32_t nIndexIn, const string& sDataIn, uint64_t stakeIn = 0, const string& owner = "");
    Block() = default;
    bool operator==(const Block& other) const {
        return sHash == other.sHash &&
            sPrevHash == other.sPrevHash &&
            stakeAmount == other.stakeAmount &&
            stakeOwner == other.stakeOwner;
    }
private:
    uint32_t _nIndex;
    uint32_t _nNonce;
    string _sData;
    time_t _tTime;
};

#endif

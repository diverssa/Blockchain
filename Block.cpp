
#include "Block.h"
#include "sha256.h"

Block::Block(uint32_t nIndexIn, const string& sDataIn, uint64_t stakeIn, const string& owner)
    : _nIndex(nIndexIn), _sData(sDataIn), stakeAmount(stakeIn), stakeOwner(owner) {
    _nNonce = 0;
    _tTime = time(nullptr);

    sHash = _CalculateHash();
}

void Block::MineBlock(uint32_t nDifficulty, uint64_t stakeAmount, const string& stakeOwner) {
    string str;
    for (uint32_t i = 0; i < nDifficulty; ++i) {
        str.append("0");
    }

    do {
        _nNonce++;
        sHash = _CalculateHash();
    } while (sHash.substr(0, nDifficulty) != str);

    cout << "Block mined with stake amount: " << stakeAmount << " by " << stakeOwner << endl;
}

bool Block::IsBlockValid() const {
    string calculatedHash = _CalculateHash();
    return sHash == calculatedHash;
}

string Block::_CalculateHash() const
{
    stringstream ss;
    ss << _nIndex << sPrevHash << _tTime << _sData << _nNonce << stakeAmount << stakeOwner;

    return sha256(ss.str());
}
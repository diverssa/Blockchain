#ifndef TESTCHAIN_BLOCKCHAIN_H
#define TESTCHAIN_BLOCKCHAIN_H

#include <cstdint>
#include <vector>
#include "Block.h"
#include <unordered_map>

using namespace std;

struct StakeBalance {
    uint64_t balance;
    vector<string> transactions; // История транзакций

    StakeBalance() : balance(0) {}
};

const uint64_t MIN_STAKE_AMOUNT = 100;

class Blockchain {
public:
    Blockchain();

    void AddBlock(Block bNew, uint64_t stakeAmount, const string& stakeOwner);
    void RemoveFromStakePool(Block blockToRemove);
    bool IsChainValid() const;

    bool MineBlock();
    bool StakeBlock(Block bNew, uint64_t stakeAmount, const string& stakeOwner); 
    bool WithdrawStake(uint64_t stakeAmount, const string& stakeOwner);
    const vector<Block>& GetStakePool() const {
        return _vStakePool;
    }
    const vector<Block>& GetChain() const {
        return _vChain;
    }
private:
    uint32_t _nDifficulty;
    vector<Block> _vChain;
    vector<Block> _vStakePool;
    unordered_map<string, uint64_t> _stakeBalances;
    Block _GetLastBlock() const;
    uint64_t stakeAmount = 100; // Пример значения стейка (ваше значение может быть другим)
    string stakeOwner = "Stake Owner"; // Пример владельца стейка (ваше значение может быть другим)

    bool TransferStake(const string& from, const string& to, uint64_t amount);
};
#endif
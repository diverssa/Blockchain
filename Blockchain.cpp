#include "Blockchain.h"

Blockchain::Blockchain()
{
    _vChain.emplace_back(Block(0, "Genesis Block", 0, ""));
    _nDifficulty = 6;
}

void Blockchain::AddBlock(Block bNew, uint64_t stakeAmount, const string& stakeOwner)
{
    if (!IsChainValid()) {
        cout << "Invalid blockchain. Block not added." << endl;
        return;
    }

    bNew.sPrevHash = _GetLastBlock().sHash;

}

bool Blockchain::IsChainValid() const
{
    for (size_t i = 1; i < _vChain.size(); ++i) {
        const Block& currentBlock = _vChain[i];
        const Block& previousBlock = _vChain[i - 1];

        if (currentBlock.sPrevHash != previousBlock.sHash) {
            return false;
        }

        if (!currentBlock.IsBlockValid()) {
            return false;
        }
    }

    return true;
}

Block Blockchain::_GetLastBlock() const
{
    return _vChain.back();
}
bool Blockchain::StakeBlock(Block bNew, uint64_t stakeAmount, const string& stakeOwner) {
    // Проверка стейка перед ставкой блока
    if (stakeAmount < MIN_STAKE_AMOUNT) {
        cout << "Stake amount is less than the minimum required amount. Block not added." << endl;
        return false;
    }

    // Добавление блока в пул ставок
    _vStakePool.push_back(bNew);

    // Обновление информации о стейке в блоке
    _vStakePool.back().stakeAmount = stakeAmount;
    _vStakePool.back().stakeOwner = stakeOwner;

    return true;
}
bool Blockchain::WithdrawStake(uint64_t stakeAmount, const string& stakeOwner) {
    // Проверка наличия стейка для отзыва
    bool found = false;
    for (auto& block : _vStakePool) {
        if (block.stakeOwner == stakeOwner && block.stakeAmount >= stakeAmount) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Stake not found or insufficient stake amount." << endl;
        return false;
    }

    // Выполнение отзыва стейка и возврат средств
    if (TransferStake(stakeOwner, "System", stakeAmount)) {
        // Если успешно выполнен перевод, удаляем блок из пула ставок
        auto it = std::remove_if(_vStakePool.begin(), _vStakePool.end(), [&](const Block& block) {
            return block.stakeOwner == stakeOwner && block.stakeAmount >= stakeAmount;
            });

        _vStakePool.erase(it, _vStakePool.end());

        cout << "Stake withdrawn: " << stakeAmount << " by " << stakeOwner << endl;
        return true;
    }
    else {
        cout << "Stake withdrawal failed." << endl;
        return false;
    }
}
bool Blockchain::MineBlock() {
    // Выбор блока для майнинга на основе стейка
    Block nextBlock;
    uint64_t maxStake = 0;

    for (const auto& block : _vStakePool) {
        if (block.stakeAmount > maxStake) {
            maxStake = block.stakeAmount;
            nextBlock = block;
        }
    }

    if (maxStake > 0) {
        // Майнинг выбранного блока
        nextBlock.MineBlock(_nDifficulty, stakeAmount, stakeOwner);
        _vChain.push_back(nextBlock);

        // Удаление выбранного блока из пула ставок
        _vStakePool.erase(std::remove(_vStakePool.begin(), _vStakePool.end(), nextBlock), _vStakePool.end());

        return true;
    }
    else {
        cout << "No blocks in the stake pool to mine." << endl;
        return false;
    }
}

void Blockchain::RemoveFromStakePool(Block blockToRemove)
{
    // Найдем и удалим успешно добытый блок из пула ставок
    auto it = std::remove_if(_vStakePool.begin(), _vStakePool.end(), [&](const Block& block) {
        return block.stakeOwner == blockToRemove.stakeOwner && block.stakeAmount == blockToRemove.stakeAmount;
        });

    _vStakePool.erase(it, _vStakePool.end());
}
bool Blockchain::TransferStake(const string& from, const string& to, uint64_t amount) {
    // Проверка наличия достаточного баланса у отправителя (from)
    auto fromBalanceIt = _stakeBalances.find(from);
    if (fromBalanceIt == _stakeBalances.end() || fromBalanceIt->second < amount) {
        cout << "Stake transfer failed. Insufficient balance for " << from << "." << endl;
        return false;
    }

    // Уменьшение баланса у отправителя
    fromBalanceIt->second -= amount;

    // Поиск баланса получателя (to)
    auto toBalanceIt = _stakeBalances.find(to);

    // Если получатель не найден, создаем новую запись
    if (toBalanceIt == _stakeBalances.end()) {
        _stakeBalances[to] = amount;
    }
    else {
        // Увеличиваем баланс получателя
        toBalanceIt->second += amount;
    }

    cout << "Stake transfer successful. " << amount << " STAKE transferred from " << from << " to " << to << "." << endl;

    return true;
}
#include "Blockchain.h"

int main()
{
    Blockchain bChain = Blockchain();

    cout << "Staking block 1..." << endl;
    bChain.StakeBlock(Block(1, "Block 1 Data"), 100, "Public Key 1");

    cout << "Staking block 2..." << endl;
    bChain.StakeBlock(Block(2, "Block 2 Data"), 200, "Public Key 2");

    cout << "Staking block 3..." << endl;
    bChain.StakeBlock(Block(3, "Block 3 Data"), 150, "Public Key 3");

    // Майнинг блоков из пула ставок
    while (bChain.MineBlock()) {
        cout << "Block mined!" << endl;
    }
    for (const Block& block : bChain.GetChain()) {
        cout << "Block: " << block.sHash << endl;
    }
    return 0;
}

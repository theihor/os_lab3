#include "ram.h"

RAM::RAM() {
    content.resize(RAM_SIZE);
    ram_block b;
    b.size = RAM_SIZE - sizeof(ram_block);
    b.loc = 0;
    b.free = '1';
    blocks.push_back(b);
    byte* c = (byte*)&b;
    fill(0, c, sizeof(ram_block));
}

void RAM::dump() {
    cout << "Memory dump" << endl;
    for (int i = 0; i < content.size(); i++) {
        if (content[i] < 16) cout << 0;
        cout << hex << uppercase << (short)content[i] << " ";
        if (i % 32 == 31) cout << endl;
    }
    cout << "End of memory dump" << endl;
    cout << "Blocks:" << endl;
    cout << std::dec;
    for (auto block : blocks) {
        cout << "Start: " << block.loc << endl;
        cout << "Size:  " << block.size << endl;
        cout << "Free:  " << block.free << endl;
        cout << endl;
    }
    cout << "sizeof(blocK) = " << sizeof(ram_block) << endl;
}

void RAM::fillRandom(uint loc, uint size) {
    if (loc + size >= content.size() || loc < 0) return;
    for (uint i = loc; i < loc + size; i++) {
        content[i] = rand() % 256; 
    }
}

void RAM::fill(uint loc, byte* what, uint size) {
    for (uint i = loc; i < loc + size; i++) {
        content[i] = what[i - loc];
    }
}

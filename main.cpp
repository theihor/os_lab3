#include "memory.h"
int main() {
    mem_init();
    mem_dump();

    block_info block;
    block = read_block_info(mem_alloc(8)); // first block at 0
    fill_block(block, 0x11);
    block = read_block_info(mem_alloc(11)); // second at 0 + 8 + 2 = 10
    fill_block(block, 0x22);
    block = read_block_info(mem_alloc(7)); // third at 10 + 2 + 11 = 23
    fill_block(block, 0x33);
    mem_dump();

    mem_free(10);
    mem_dump();

    block = read_block_info(mem_realloc(0, 15));
    mem_dump();

    mem_free(23);
    mem_dump();

    return 0;
}


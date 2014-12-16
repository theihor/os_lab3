#include <stdio.h>
#define MEM_SIZE 128 // max is 2^15 - 2 = 0x7FFF
#define HEADER_SIZE 2
#define THRESHOLD (HEADER_SIZE + 1)  // minimum memory size to locate a block

typedef unsigned char byte;
typedef unsigned short ushort;
typedef struct {
    ushort address;
    ushort size;
    bool busy;
} block_info;



block_info read_block_info(ushort address);
void mem_dump(); 
void fill_block(block_info block, byte value); 
ushort mem_alloc(ushort size);
void mem_clean();
void mem_free(ushort address);
ushort mem_realloc(ushort address, ushort new_size); 
void mem_init(); 

byte mem_read(ushort address);
void mem_write(ushort address, byte value);

#include <stdio.h>
#define MEM_SIZE 512 // max is 2^15 - 2 = 0x7FFF
#define HEADER_SIZE 2
#define THRESHOLD (HEADER_SIZE + 1)  // minimum memory size to locate a block

typedef unsigned char byte;
typedef unsigned short ushort;
typedef enum { false, true } bool;
typedef struct {
    ushort address;
    ushort size;
    bool busy;
} block_info;


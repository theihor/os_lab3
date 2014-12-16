#define RAM_SIZE 512 // should fit 'uint' 
#define CACHE_SIZE 32 // should be <= RAM_SIZE / 10

typedef unsigned char byte;
typedef unsigned int uint;

typedef struct ram_block {
    uint size;
    uint loc;
    char free;
} ram_block;


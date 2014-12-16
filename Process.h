#include "memory.h"

static byte next_id = 0;

class Process {
private:
    ushort size;
    ushort loc;
    byte id;

    void fill();

public:
    Process(ushort _address, ushort _size);
    byte read(ushort address); // read from virtual address
    void write(ushort address, byte value); // write value in virtual address
    void do_something(byte n); // read or write something n times
    
    bool log;
};

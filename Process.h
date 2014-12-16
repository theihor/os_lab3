#include "memory.h"

class Process {
private:
    ushort size;
    byte id;

    void fill();

public:
    Process(byte _id, ushort _address, ushort _size);
    byte read(ushort address); // read from virtual address
    void write(ushort address, byte value); // write value in virtual address
    void do_something(byte n); // read or write something n times
    
    bool log;
    ushort loc;
};

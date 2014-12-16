#include "Process.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

using std::cout;
using std::endl;
using std::hex;
using std::dec;

Process::Process(byte _id, ushort _address, ushort _size) {
    this->size = _size;
    this->loc = _address;
    this->id = _id;
    this->log = true;
    fill();
}

byte Process::read(ushort virtual_address) { // read from virtual address
    ushort address = this->loc + virtual_address + HEADER_SIZE;
    byte val = mem_read(address);
    if (this->log) cout << dec << "Process " << (int)id << " reads at " << virtual_address << " -> " << address << ": " << hex << (int)val << endl;
    return val;
}    

void Process::write(ushort virtual_address, byte value) { // write value in virtual address
    ushort address = this->loc + virtual_address + HEADER_SIZE;
    if (this->log) cout << dec << "Process " << (int)id << " writes at " << virtual_address << " -> " << address << ": " << hex << (int)value << endl;
    mem_write(address, value);
}

void Process::do_something(byte n) { // read or write something n times
    srand(time(NULL)); 
    byte val = 0;
    for (int i = 0; i < n; i++) {
       if (rand() % 2) {
           val = read(rand() % this->size);
       } else {
           write(rand() % this->size, val);
       }
    mem_dump();
    }
}

void Process::fill() {
    srand(time(NULL)); 
    log = false;
    for (int i = 0; i < this->size; i++) {
        write(i, rand() % 256);
    }
    log = true;
}

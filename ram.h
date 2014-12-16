#include <list>
#include <vector>
#include <iostream>
#include "lab3.h"

using std::cout;
using std::endl;
using std::hex;
using std::uppercase;
using std::list;
using std::vector;

class RAM {
private:
    vector<byte> content;
    list<ram_block> blocks;

    void fill(uint loc, byte* what, uint size); 

    void fillRandom(uint loc, uint size); 
public:
    RAM(); 

    void dump(); 
};

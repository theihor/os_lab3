#include "ram.h"

class Process {
    private:
        uint size;
        double time;
        RAM* ram;
    public:
        Process(uint s, double t) {
            this->size = s;
            this->time = t;
        }

        char* read(uint where, uint how_many) {
            
        }
};



int main () {
    RAM ram;
   
    ram.dump();
    return 0;
}

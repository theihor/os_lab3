#include "Scheduler.h"

int main() {
    mem_init();
    mem_dump();
    
    Scheduler s;
    
    s.generate(COUNT_OF_PROCESSES);    
    s.run();
    
    mem_dump();

    return 0;
}

